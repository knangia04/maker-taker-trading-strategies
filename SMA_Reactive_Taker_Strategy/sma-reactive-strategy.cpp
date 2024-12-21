#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "SMAReactiveStrategy.h"

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

SMAReactiveStrategy::SMAReactiveStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName):
    Strategy(strategyID, strategyName, groupName),
    sma_trend_map_(),    
    aggressiveness_(0),
    position_size_(100),
    sma_window_size_(100),
    debug_(false)
{
}

SMAReactiveStrategy::~SMAReactiveStrategy()
{
}

void SMAReactiveStrategy::OnResetStrategyState()
{
    sma_trend_map_.clear();
}

void SMAReactiveStrategy::DefineStrategyParams()
{
    params().CreateParam(CreateStrategyParamArgs("aggressiveness", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_DOUBLE, aggressiveness_));
    params().CreateParam(CreateStrategyParamArgs("position_size", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, position_size_));
    params().CreateParam(CreateStrategyParamArgs("sma_window_size", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, sma_window_size_));
    params().CreateParam(CreateStrategyParamArgs("debug", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_BOOL, debug_));
}

void SMAReactiveStrategy::DefineStrategyCommands()
{
    commands().AddCommand(StrategyCommand(1, "Reprice Existing Orders"));
    commands().AddCommand(StrategyCommand(2, "Cancel All Orders"));
}

void SMAReactiveStrategy::RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate)
{    
    for (SymbolSetConstIter it = symbols_begin(); it != symbols_end(); ++it) {
        eventRegister->RegisterForBars(*it, BAR_TYPE_TIME, 1);
    }
}

void SMAReactiveStrategy::OnBar(const BarEventMsg& msg)
{ 
    if (debug_) {
        ostringstream str;
        str << msg.instrument().symbol() << ": " << msg.bar();
        logger().LogToClient(LOGLEVEL_DEBUG, str.str().c_str());
    }

    // Check if we're already tracking the SMA trend object for this instrument
    SMATrendMapIterator trend_iter = sma_trend_map_.find(&msg.instrument());
    if (trend_iter == sma_trend_map_.end()) {
        trend_iter = sma_trend_map_.insert(make_pair(&msg.instrument(), SMATrend(sma_window_size_))).first;
    }

    int signal = trend_iter->second.Update(msg.bar().close());

    if (trend_iter->second.FullyInitialized()) {
        AdjustPortfolio(&msg.instrument(), signal);
    }
}

void SMAReactiveStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg)  
{
}

void SMAReactiveStrategy::AdjustPortfolio(const Instrument* instrument, int signal)
{
    if (signal == 0) return;  // No action needed

    int desired_position = position_size_ * signal;
    int trade_size = desired_position - portfolio().position(instrument);

    if (trade_size != 0) {
        // If we're not working an order for the instrument already, place a new order
        if (orders().num_working_orders(instrument) == 0) {
            SendOrder(instrument, trade_size);
        } else {  
            // Otherwise find the order and cancel it if we're now trying to trade in the other direction
            const Order* order = *orders().working_orders_begin(instrument);
            if (order && ((IsBuySide(order->order_side()) && trade_size < 0) || 
                         ((IsSellSide(order->order_side()) && trade_size > 0)))) {
                trade_actions()->SendCancelOrder(order->order_id());
            }
        }
    }
}

void SMAReactiveStrategy::SendOrder(const Instrument* instrument, int trade_size)
{
    if (!instrument->top_quote().ask_side().IsValid() || !instrument->top_quote().ask_side().IsValid()) {
        std::stringstream ss;
        ss << "Skipping trade due to lack of two sided quote"; 
        logger().LogToClient(LOGLEVEL_DEBUG, ss.str());
        return;
    }

    double price = trade_size > 0 ? instrument->top_quote().bid() + aggressiveness_ : instrument->top_quote().ask() - aggressiveness_;

    OrderParams params(*instrument,
                       abs(trade_size),
                       price,
                       MARKET_CENTER_ID_NASDAQ,
                       (trade_size > 0) ? ORDER_SIDE_BUY : ORDER_SIDE_SELL,
                       ORDER_TIF_DAY,
                       ORDER_TYPE_LIMIT);

    if (debug_) {
        std::cout << "SendTradeOrder(): sending new order for size "
                << trade_size
                << " at $"
                << price
                << " for symbol "
                << instrument->symbol()
                << std::endl;
    }
        
    trade_actions()->SendNewOrder(params);
}

void SMAReactiveStrategy::RepriceAll()
{
    for (IOrderTracker::WorkingOrdersConstIter ordit = orders().working_orders_begin(); ordit != orders().working_orders_end(); ++ordit) {
        Reprice(*ordit);
    }
}

void SMAReactiveStrategy::Reprice(Order* order)
{
    OrderParams params = order->params();
    params.price = (order->order_side() == ORDER_SIDE_BUY) ? order->instrument()->top_quote().bid() + aggressiveness_ : order->instrument()->top_quote().ask() - aggressiveness_;
    trade_actions()->SendCancelReplaceOrder(order->order_id(), params);
}

void SMAReactiveStrategy::OnStrategyCommand(const StrategyCommandEventMsg& msg)
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

void SMAReactiveStrategy::OnParamChanged(StrategyParam& param)
{    
    if (param.param_name() == "aggressiveness") {
        if (!param.Get(&aggressiveness_))
            throw StrategyStudioException("Could not get aggressiveness");
    } else if (param.param_name() == "position_size") {
        if (!param.Get(&position_size_))
            throw StrategyStudioException("Could not get position_size");
    } else if (param.param_name() == "sma_window_size") {
        if (!param.Get(&sma_window_size_))
            throw StrategyStudioException("Could not get sma_window_size");
    } else if (param.param_name() == "debug") {
        if (!param.Get(&debug_))
            throw StrategyStudioException("Could not get debug");
    }
}
