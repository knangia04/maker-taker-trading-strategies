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
    grid_spacing_(1.0),
    position_size_(100),
    debug_(false)
{
}


GridStrategy::~GridStrategy()
{
}

void GridStrategy::OnResetStrategyState()
{
    active_orders_.clear();
}

void GridStrategy::DefineStrategyParams()
{
    params().CreateParam(CreateStrategyParamArgs("grid_spacing", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_DOUBLE, grid_spacing_));
    params().CreateParam(CreateStrategyParamArgs("position_size", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, position_size_));
    params().CreateParam(CreateStrategyParamArgs("debug", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_BOOL, debug_));
}


void GridStrategy::DefineStrategyCommands() {
    // Define any commands your strategy needs. Example:
    commands().AddCommand(StrategyCommand(1, "Reprice Existing Orders"));
    commands().AddCommand(StrategyCommand(2, "Cancel All Orders"));
}

void GridStrategy::RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate)
{
    for (SymbolSetConstIter it = symbols_begin(); it != symbols_end(); ++it) {
        eventRegister->RegisterForBars(*it, BAR_TYPE_TIME, 1);
    }
}

void GridStrategy::OnBar(const BarEventMsg& msg)
{
    if (debug_) {
        ostringstream str;
        str << msg.instrument().symbol() << ": " << msg.bar();
        logger().LogToClient(LOGLEVEL_DEBUG, str.str().c_str());
    }

    AdjustGridOrders(&msg.instrument(), msg.bar().close());
}

void GridStrategy::AdjustGridOrders(const Instrument* instrument, double current_price)
{
    double price_above = current_price + grid_spacing_;
    double price_below = current_price - grid_spacing_;

    // Check and create buy order at the lower grid level
    if (!HasActiveOrder(instrument, ORDER_SIDE_BUY, price_below)) {
        SendOrder(instrument, ORDER_SIDE_BUY, position_size_, price_below);
    }

    // Check and create sell order at the upper grid level
    if (!HasActiveOrder(instrument, ORDER_SIDE_SELL, price_above)) {
        SendOrder(instrument, ORDER_SIDE_SELL, position_size_, price_above);
    }
}

bool GridStrategy::HasActiveOrder(const Instrument* instrument, OrderSide side, double price)
{
    for (const auto& order : active_orders_) {
        if (order.instrument() == instrument && order.order_side() == side && fabs(order.params().price - price) < 1e-6) {
            return true;
        }
    }
    return false;
}

void GridStrategy::SendOrder(const Instrument* instrument, OrderSide side, int size, double price)
{
    OrderParams params(*instrument, size, price, MARKET_CENTER_ID_NASDAQ, side, ORDER_TIF_DAY, ORDER_TYPE_LIMIT);
    trade_actions()->SendNewOrder(params);

    if (debug_) {
        std::stringstream ss;
        ss << "Sent new order: "
           << "Side: " << (side == ORDER_SIDE_BUY ? "Buy" : "Sell")
           << ", Price: " << price
           << ", Size: " << size;
        logger().LogToClient(LOGLEVEL_DEBUG, ss.str());
    }
}

void GridStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg)
{
    if (msg.status() == ORDER_STATUS_FILLED) {
        active_orders_.erase(msg.order_id());
    } else if (msg.status() == ORDER_STATUS_WORKING) {
        active_orders_.insert(msg.order_id());
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
    if (param.param_name() == "grid_spacing") {
        if (!param.Get(&grid_spacing_)) {
            throw StrategyStudioException("Could not get grid_spacing");
        }
    } else if (param.param_name() == "position_size") {
        if (!param.Get(&position_size_)) {
            throw StrategyStudioException("Could not get position_size");
        }
    } else if (param.param_name() == "debug") {
        if (!param.Get(&debug_)) {
            throw StrategyStudioException("Could not get debug");
        }
    }
}


