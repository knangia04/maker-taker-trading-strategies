#pragma once

#ifndef _STRATEGY_STUDIO_LIB_EXAMPLES_SMA_REACTIVE_STRATEGY_H_
#define _STRATEGY_STUDIO_LIB_EXAMPLES_SMA_REACTIVE_STRATEGY_H_

#ifdef _WIN32
    #define _STRATEGY_EXPORTS __declspec(dllexport)
#else
    #ifndef _STRATEGY_EXPORTS
    #define _STRATEGY_EXPORTS
    #endif
#endif

#include <Strategy.h>
#include <Analytics/ScalarRollingWindow.h>
#include <Analytics/InhomogeneousOperators.h>
#include <Analytics/IncrementalEstimation.h>
#include <MarketModels/Instrument.h>
#include <Utilities/ParseConfig.h>

#include <vector>
#include <map>
#include <iostream>

using namespace RCM::StrategyStudio;

class SMATrend {
public:
    SMATrend(int sma_window_size = 100) : sma_window_(sma_window_size), last_close_(0.0) {}

    void Reset() {
        sma_window_.clear();
        last_close_ = 0.0;
    }

    int Update(double current_close) {
        double prev_close = last_close_;
        last_close_ = current_close;
        sma_window_.push_back(current_close);
        
        if (!sma_window_.full()) return 0;

        double sma = sma_window_.Mean();
        
        // Buy signal: price crosses above SMA and continuing upward
        if (current_close > prev_close && prev_close > sma && current_close > sma) 
            return 1;
        // Sell signal: price crosses below SMA and continuing downward
        else if (current_close < prev_close && prev_close < sma && current_close < sma)
            return -1;
            
        return 0;
    }

    bool FullyInitialized() { return sma_window_.full(); }
    
private:
    Analytics::ScalarRollingWindow<double> sma_window_;
    double last_close_;
};

class SMAReactiveStrategy : public Strategy {
public:
    typedef boost::unordered_map<const Instrument*, SMATrend> SMATrendMap;
    typedef SMATrendMap::iterator SMATrendMapIterator;

public:
    SMAReactiveStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName);
    ~SMAReactiveStrategy();

public: /* from IEventCallback */
    virtual void OnTrade(const TradeDataEventMsg& msg){}
    virtual void OnTopQuote(const QuoteEventMsg& msg){}    
    virtual void OnQuote(const QuoteEventMsg& msg){}
    virtual void OnDepth(const MarketDepthEventMsg& msg){}
    virtual void OnBar(const BarEventMsg& msg);
    virtual void OnMarketState(const MarketStateEventMsg& msg){}
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);
    virtual void OnStrategyControl(const StrategyStateControlEventMsg& msg){}
    void OnResetStrategyState();
    void OnDataSubscription(const DataSubscriptionEventMsg& msg){}
    void OnStrategyCommand(const StrategyCommandEventMsg& msg);
    void OnParamChanged(StrategyParam& param);

private: // Helper functions
    void AdjustPortfolio(const Instrument* instrument, int signal);
    void SendOrder(const Instrument* instrument, int trade_size);
    void RepriceAll();
    void Reprice(Order* order);

private: /* from Strategy */
    virtual void RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate); 
    virtual void DefineStrategyParams();
    virtual void DefineStrategyCommands();

private:
    SMATrendMap sma_trend_map_;
    double aggressiveness_;
    int position_size_;
    int sma_window_size_;
    bool debug_;
};

extern "C" {
    _STRATEGY_EXPORTS const char* GetType() {
        return "SMAReactiveStrategy";
    }

    _STRATEGY_EXPORTS IStrategy* CreateStrategy(const char* strategyType,
                                   unsigned strategyID,
                                   const char* strategyName,
                                   const char* groupName) {
        if (strcmp(strategyType, GetType()) == 0) {
            return *(new SMAReactiveStrategy(strategyID, strategyName, groupName));
        } else {
            return NULL;
        }
    }

    _STRATEGY_EXPORTS const char* GetAuthor() {
        return ""; // Add your name here
    }

    _STRATEGY_EXPORTS const char* GetAuthorGroup() {
        return ""; // Add your group here
    }

    _STRATEGY_EXPORTS const char* GetReleaseVersion() {
        return Strategy::release_version();
    }
}

#endif
