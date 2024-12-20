/*================================================================================
*     Source: ../RCM/StrategyStudio/examples/strategies/GridStrategy/GridStrategy.cpp
*     Last Update: 2024/12/20
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2011 - 2024.
*     All rights reserved.
*
*     This software is part of Licensed material, which is the property of RCM-X ("Company"), 
*     and constitutes Confidential Information of the Company.
*     Unauthorized use, modification, duplication or distribution is strictly prohibited by Federal law.
*     No title to or ownership of this software is hereby transferred.
*
*     The software is provided "as is", and in no event shall the Company or any of its affiliates or successors be liable for any 
*     damages, including any lost profits or other incidental or consequential damages relating to the use of this software.       
*     The Company makes no representations or warranties, express or implied, with regards to this software.                        
/*================================================================================*/

#ifdef _WIN32
    #include "stdafx.h"
#endif

#include "GridStrategy.h"

#include "FillInfo.h"
#include "AllEventMsg.h"
#include "ExecutionTypes.h"
#include <Utilities/Cast.h>
#include <Utilities/utils.h>

#include <cmath>
#include <iostream>
#include <cassert>

using namespace RCM::StrategyStudio;
using namespace RCM::StrategyStudio::MarketModels;
using namespace RCM::StrategyStudio::Utilities;

using namespace std;

GridStrategy::GridStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName):
    Strategy(strategyID, strategyName, groupName),
    grid_levels_(),    
    grid_spacing_(0.5),
    position_size_(100),
    debug_(false)
{
}

GridStrategy::~GridStrategy()
{
}

void GridStrategy::OnResetStrategyState()
{
    grid_levels_.clear();
}

void GridStrategy::DefineStrategyParams()
{
    params().CreateParam(CreateStrategyParamArgs("grid_spacing", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_DOUBLE, grid_spacing_));
    params().CreateParam(CreateStrategyParamArgs("position_size", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, position_size_));
    params().CreateParam(CreateStrategyParamArgs("debug", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_BOOL, debug_));
}

void GridStrategy::DefineStrategyCommands()
{
    commands().AddCommand(StrategyCommand(1, "Cancel All Orders"));
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

    const Instrument& instrument = msg.instrument();
    double price = msg.bar().close();

    // Initialize grid levels if not already present
    if (grid_levels_.find(&instrument) == grid_levels_.end()) {
        InitializeGridLevels(&instrument, price);
    }

    // Execute grid logic
    ExecuteGridLogic(&instrument, price);
}

void GridStrategy::OnOrderUpdate(const OrderUpdateEventMsg& msg)  
{
}

void GridStrategy::InitializeGridLevels(const Instrument* instrument, double price)
{
    GridLevels& levels = grid_levels_[instrument];

    // Create grid levels around the current price
    for (int i = -10; i <= 10; ++i) {
        levels.push_back(price + i * grid_spacing_);
    }

    if (debug_) {
        ostringstream str;
        str << "Initialized grid levels for " << instrument->symbol() << " at price " << price;
        logger().LogToClient(LOGLEVEL_DEBUG, str.str().c_str());
    }
}

void GridStrategy::ExecuteGridLogic(const Instrument* instrument, double price)
{
    GridLevels& levels = grid_levels_[instrument];

    for (double level : levels) {
        int position = portfolio().position(instrument);

        if (price <= level && position < position_size_) {
            // Buy at this grid level
            SendOrder(instrument, position_size_ - position, level);
        } else if (price >= level && position > -position_size_) {
            // Sell at this grid level
            SendOrder(instrument, -position_size_ - position, level);
        }
    }
}

void GridStrategy::SendOrder(const Instrument* instrument, int trade_size, double price)
{
    OrderParams params(*instrument,
                       abs(trade_size),
                       price,
                       MARKET_CENTER_ID_NASDAQ,
                       (trade_size > 0) ? ORDER_SIDE_BUY : ORDER_SIDE_SELL,
                       ORDER_TIF_DAY,
                       ORDER_TYPE_LIMIT);

    if (debug_) {
        std::stringstream ss;
        ss << "SendOrder(): Sending order for size "
           << trade_size
           << " at $"
           << price
           << " for symbol "
           << instrument->symbol();
        logger().LogToClient(LOGLEVEL_DEBUG, ss.str().c_str());
    }

    trade_actions()->SendNewOrder(params);
}

void GridStrategy::OnStrategyCommand(const StrategyCommandEventMsg& msg)
{
    switch (msg.command_id()) {
        case 1:
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
        if (!param.Get(&grid_spacing_))
            throw StrategyStudioException("Could not get grid_spacing");
    } else if (param.param_name() == "position_size") {
        if (!param.Get(&position_size_))
            throw StrategyStudioException("Could not get position_size");
    } else if (param.param_name() == "debug") {
        if (!param.Get(&debug_))
            throw StrategyStudioException("Could not get debug");
    } 
}
