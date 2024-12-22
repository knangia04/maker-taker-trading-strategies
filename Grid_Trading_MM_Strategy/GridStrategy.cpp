#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "GridStrategy.h"

#include "FillInfo.h"
#include "AllEventMsg.h"
#include "ExecutionTypes.h"
#include <Utilities/Cast.h>
#include <Utilities/utils.h>

#include <math.h>
#include <iostream>
#include <cassert>

using namespace RCM::StrategyStudio;
using namespace RCM::StrategyStudio::MarketModels;
using namespace RCM::StrategyStudio::Utilities;

using namespace std;

GridStrategy::GridStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName) :
    Strategy(strategyID, strategyName, groupName),
    initial_capital_(100000),
    grid_size_(0.005),
    num_grids_(20),
    position_size_(100),
    cash_(initial_capital_),
    current_position_(0),
    transaction_cost_(0.0001),
    debug_(false)
{
}


GridStrategy::~GridStrategy()
{
}

void GridStrategy::OnResetStrategyState()
{
    active_orders_.clear();
    cash_ = initial_capital_;
    current_position_ = 0;
    portfolio_value_ = initial_capital_;
}

void GridStrategy::DefineStrategyParams()
{
    // Add configurable parameters
    params().CreateParam(CreateStrategyParamArgs("initial_capital", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, initial_capital_));
    params().CreateParam(CreateStrategyParamArgs("grid_size", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, grid_size_));
    params().CreateParam(CreateStrategyParamArgs("num_grids", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, num_grids_));
    params().CreateParam(CreateStrategyParamArgs("position_size", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, position_size_));
    params().CreateParam(CreateStrategyParamArgs("transaction_cost", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, transaction_cost_));
    params().CreateParam(CreateStrategyParamArgs("debug", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_BOOL, debug_));
}

void GridStrategy::DefineStrategyCommands() {
    // Define any commands your strategy needs. Example:
    commands().AddCommand(StrategyCommand(1, "Reprice Existing Orders"));
    commands().AddCommand(StrategyCommand(2, "Cancel All Orders"));
}

void GridStrategy::RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate)
{
    // Register for bar updates
    for (SymbolSetConstIter it = symbols_begin(); it != symbols_end(); ++it) {
        eventRegister->RegisterForBars(*it, BAR_TYPE_TIME, 1);
    }
}

void GridStrategy::OnBar(const BarEventMsg& msg)
{
    const Instrument* instrument = &msg.instrument();
    double current_price = msg.bar().close();
    
    // Initialize grid levels if not yet set
    if (grid_levels_.empty()) {
        InitializeGridLevels(current_price);
    }
    
    // Update grid orders based on current price
    UpdateGridOrders(instrument, current_price);
    
    // Update portfolio value
    portfolio_value_ = cash_ + (current_position_ * current_price);
    
    if (debug_) {
        std::stringstream ss;
        ss << "Price: " << current_price 
           << ", Position: " << current_position_
           << ", Portfolio Value: " << portfolio_value_;
        logger().LogToClient(LOGLEVEL_DEBUG, ss.str());
    }
}

void GridStrategy::InitializeGridLevels(double mid_price)
{
    grid_levels_.clear();
    for (int i = -num_grids_; i <= num_grids_; ++i) {
        grid_levels_.push_back(mid_price * (1.0 + i * grid_size_));
    }
}

void GridStrategy::UpdateGridOrders(const Instrument* instrument, double current_price)
{
    // Cancel all existing orders
    trade_actions()->SendCancelAll();
    active_orders_.clear();
    
    for (const auto& level : grid_levels_) {
        if (level > current_price) {
            // Place sell orders above current price
            if (!HasActiveOrder(instrument, ORDER_SIDE_SELL, level)) {
                SendOrder(instrument, ORDER_SIDE_SELL, position_size_, level);
            }
        } else if (level < current_price) {
            // Place buy orders below current price
            if (!HasActiveOrder(instrument, ORDER_SIDE_BUY, level)) {
                SendOrder(instrument, ORDER_SIDE_BUY, position_size_, level);
            }
        }
    }
}

void GridStrategy::UpdateGridOrders(const Instrument* instrument, double current_price)
{
    // Cancel all existing orders
    trade_actions()->SendCancelAll();
    active_orders_.clear();
    
    for (const auto& level : grid_levels_) {
        if (level > current_price) {
            // Place sell orders above current price
            if (!HasActiveOrder(instrument, ORDER_SIDE_SELL, level)) {
                SendOrder(instrument, ORDER_SIDE_SELL, position_size_, level);
            }
        } else if (level < current_price) {
            // Place buy orders below current price
            if (!HasActiveOrder(instrument, ORDER_SIDE_BUY, level)) {
                SendOrder(instrument, ORDER_SIDE_BUY, position_size_, level);
            }
        }
    }
}


void GridStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg)
{
    if (msg.status() == ORDER_STATUS_FILLED) {
        // Update position and cash based on fill
        if (msg.order_side() == ORDER_SIDE_BUY) {
            current_position_ += msg.last_fill_size();
            cash_ -= msg.last_fill_size() * msg.last_fill_price() * (1.0 + transaction_cost_);
        } else {
            current_position_ -= msg.last_fill_size();
            cash_ += msg.last_fill_size() * msg.last_fill_price() * (1.0 - transaction_cost_);
        }
        
        if (debug_) {
            std::stringstream ss;
            ss << "Order filled - Side: " << (msg.order_side() == ORDER_SIDE_BUY ? "Buy" : "Sell")
               << ", Size: " << msg.last_fill_size()
               << ", Price: " << msg.last_fill_price()
               << ", New Position: " << current_position_
               << ", Cash: " << cash_;
            logger().LogToClient(LOGLEVEL_DEBUG, ss.str());
        }
    }
}



void GridStrategy::RepriceAll()
{
    for (IOrderTracker::WorkingOrdersConstIter ordit = orders().working_orders_begin(); ordit != orders().working_orders_end(); ++ordit) {
        Reprice(*ordit);
    }
}

void GridStrategy::Reprice(Order* order)
{
    OrderParams params = order->params();
    params.price = (order->order_side() == ORDER_SIDE_BUY) ? order->instrument()->top_quote().bid() + aggressiveness_ : order->instrument()->top_quote().ask() - aggressiveness_;
    trade_actions()->SendCancelReplaceOrder(order->order_id(), params);
}

void GridStrategy::OnStrategyCommand(const StrategyCommandEventMsg& msg)
{
    switch (msg.command_id()) {
        case 1:
            RepriceAll();
            break;
        case 2:
            trade_actions()->SendCancelAll();
            break;
        default:
            logger().LogToClient(LOGLEVEL_DEBUG, "Unknown strategy command received");
            break;
    }
}


void GridStrategy::OnParamChanged(StrategyParam& param)
{
    if (param.param_name() == "initial_capital") {
        param.Get(&initial_capital_);
    } else if (param.param_name() == "grid_size") {
        param.Get(&grid_size_);
    } else if (param.param_name() == "num_grids") {
        param.Get(&num_grids_);
    } else if (param.param_name() == "position_size") {
        param.Get(&position_size_);
    } else if (param.param_name() == "transaction_cost") {
        param.Get(&transaction_cost_);
    } else if (param.param_name() == "debug") {
        param.Get(&debug_);
    }
}


