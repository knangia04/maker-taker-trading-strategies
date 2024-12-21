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
#include <Analytics/ScalarRollingWindow.h>
#include <MarketModels/Instrument.h>
#include <Utilities/ParseConfig.h>

#include <vector>
#include <map>
#include <iostream>

using namespace RCM::StrategyStudio;

class GridStrategyGrid {
public:
    GridStrategyGrid(double grid_interval = 0.5, int max_position = 100) 
        : grid_interval_(grid_interval), max_position_(max_position) {}

    void Reset() {
        active_orders_.clear();
    }

    void Update(double price) {
        current_price_ = price;
    }

    double NextGridLevelUp() const {
        return current_price_ + grid_interval_;
    }

    double NextGridLevelDown() const {
        return current_price_ - grid_interval_;
    }

    int GetMaxPosition() const {
        return max_position_;
    }

private:
    double grid_interval_;
    double current_price_;
    int max_position_;
    std::vector<double> active_orders_;
};

class GridStrategy : public Strategy {
public:
    typedef boost::unordered_map<const Instrument*, GridStrategyGrid> GridMap;
    typedef GridMap::iterator GridMapIterator;

public:
    GridStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName);
    ~GridStrategy();

public: /* from IEventCallback */
    virtual void OnTrade(const TradeDataEventMsg& msg) {}
    virtual void OnTopQuote(const QuoteEventMsg& msg) {}
    virtual void OnQuote(const QuoteEventMsg& msg) {}
    virtual void OnDepth(const MarketDepthEventMsg& msg) {}
    virtual void OnBar(const BarEventMsg& msg);
    virtual void OnMarketState(const MarketStateEventMsg& msg) {}
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);
    virtual void OnStrategyControl(const StrategyStateControlEventMsg& msg) {}
    void OnResetStrategyState();
    void OnDataSubscription(const DataSubscriptionEventMsg& msg) {}
    void OnStrategyCommand(const StrategyCommandEventMsg& msg);
    void OnParamChanged(StrategyParam& param);

private: // Helper functions specific to this strategy
    void AdjustGrid(const Instrument* instrument);
    void PlaceGridOrders(const Instrument* instrument);

private: /* from Strategy */
    virtual void RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate);
    virtual void DefineStrategyParams();
    virtual void DefineStrategyCommands();

private:
    GridMap grid_map_;
    double grid_interval_;
    int max_position_;
    bool debug_;
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
        } else {
            return NULL;
        }
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
