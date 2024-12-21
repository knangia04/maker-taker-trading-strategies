/*================================================================================
*     Source: ../RCM/StrategyStudio/examples/strategies/GridTakerStrategy/GridTakerStrategy.h
*     Last Update: 2024/12/20 14:20:00
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

#pragma once

#ifndef _STRATEGY_STUDIO_LIB_EXAMPLES_GRID_STRATEGY_H_
#define _STRATEGY_STUDIO_LIB_EXAMPLES_GRID_STRATEGY_H_

#ifdef _WIN32
    #define _STRATEGY_EXPORTS __declspec(dllexport)
#else
    #ifndef _STRATEGY_EXPORTS
    #define _STRATEGY_EXPORTS
    #endif
#endif

#include <Strategy.h>
#include <MarketModels/Instrument.h>
#include <set>

using namespace RCM::StrategyStudio;
using namespace RCM::StrategyStudio::MarketModels;

class GridStrategy : public Strategy {
public:
    GridStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName);
    ~GridStrategy();

public: // Strategy Events
    virtual void OnTrade(const TradeDataEventMsg& msg) {}
    virtual void OnTopQuote(const QuoteEventMsg& msg) {}
    virtual void OnQuote(const QuoteEventMsg& msg) {}
    virtual void OnDepth(const MarketDepthEventMsg& msg) {}
    virtual void OnBar(const BarEventMsg& msg);
    virtual void OnMarketState(const MarketStateEventMsg& msg) {}
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);
    virtual void OnStrategyControl(const StrategyStateControlEventMsg& msg) {}
    virtual void OnResetStrategyState();
    virtual void OnDataSubscription(const DataSubscriptionEventMsg& msg) {}
    virtual void OnStrategyCommand(const StrategyCommandEventMsg& msg);
    virtual void OnParamChanged(StrategyParam& param);

private: // Helper functions
void AdjustGridOrders(const Instrument* instrument, double current_price);
    bool HasActiveOrder(const Instrument* instrument, OrderSide side, double price);
    void SendOrder(const Instrument* instrument, OrderSide side, int size, double price);
    void RepriceAll();
    void Reprice(Order* order);

private: // Strategy interface implementation
    virtual void RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate);
    virtual void DefineStrategyParams();
    virtual void DefineStrategyCommands();

private: // Member variables
    double grid_spacing_;
    int position_size_;
    bool debug_;
    double aggressiveness_;  // Added to match Reprice() implementation
    std::set<OrderID> active_orders_;
};

extern "C" {
    _STRATEGY_EXPORTS const char* GetType() {
        return "GridStrategy";
    }

    _STRATEGY_EXPORTS IStrategy* CreateStrategy(const char* strategyType,
                                              unsigned strategyID,
                                              const char* strategyName,
                                              const char* groupName) {
        if (strcmp(strategyType, GetType()) == 0) {
            return *(new GridStrategy(strategyID, strategyName, groupName));
        }
        return nullptr;
    }

    _STRATEGY_EXPORTS const char* GetAuthor() {
        return "dlariviere";
    }

    _STRATEGY_EXPORTS const char* GetAuthorGroup() {
        return "UIUC";
    }

    _STRATEGY_EXPORTS const char* GetReleaseVersion() {
        return Strategy::release_version();
    }
}

#endif
