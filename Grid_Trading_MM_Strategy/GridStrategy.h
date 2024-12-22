// GridStrategy.h

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
#include <vector>
#include <set>

using namespace RCM::StrategyStudio;
using namespace RCM::StrategyStudio::MarketModels;

class GridStrategy : public Strategy {
public:
    GridStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName);
    ~GridStrategy();

public: // Strategy Events
    virtual void OnBar(const BarEventMsg& msg);
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);
    virtual void OnResetStrategyState();
    virtual void OnParamChanged(StrategyParam& param);
    
    // Unused but required virtual functions
    virtual void OnTrade(const TradeDataEventMsg& msg) {}
    virtual void OnTopQuote(const QuoteEventMsg& msg) {}
    virtual void OnQuote(const QuoteEventMsg& msg) {}
    virtual void OnDepth(const MarketDepthEventMsg& msg) {}
    virtual void OnMarketState(const MarketStateEventMsg& msg) {}
    virtual void OnStrategyControl(const StrategyStateControlEventMsg& msg) {}
    virtual void OnDataSubscription(const DataSubscriptionEventMsg& msg) {}

private: // Helper functions
    void InitializeGridLevels(double mid_price);
    void UpdateGridOrders(const Instrument* instrument, double current_price);
    bool HasActiveOrder(const Instrument* instrument, OrderSide side, double price);
    void SendOrder(const Instrument* instrument, OrderSide side, int size, double price);
    void RepriceAll();
    void Reprice(Order* order);

private: // Strategy interface implementation
    virtual void RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate);
    virtual void DefineStrategyParams();

private: // Member variables
    double initial_capital_;
    double grid_size_;
    int num_grids_;
    int position_size_;
    double transaction_cost_;
    bool debug_;
    
    double cash_;
    int current_position_;
    double portfolio_value_;
    std::vector<double> grid_levels_;
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
}

#endif