#pragma once

#ifndef _STRATEGY_STUDIO_LIB_EXAMPLES_SMA_REACTIVE_TAKER_STRATEGY_H_
#define _STRATEGY_STUDIO_LIB_EXAMPLES_SMA_REACTIVE_TAKER_STRATEGY_H_

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

enum SignalType {
    SIGNAL_BUY = 1,
    SIGNAL_SELL = -1,
    SIGNAL_NONE = 0
};

class SMAReactiveTaker {
public:
    SMAReactiveTaker(int sma_window_size = 100);

    void Reset();

    SignalType Update(double current_price);

private:
    Analytics::ScalarRollingWindow<double> price_window_;
    int sma_window_size_;
    double previous_price_;
    int momentum_days_;
};

class SMAReactiveTakerStrategy : public Strategy {
public:
    typedef boost::unordered_map<const Instrument*, SMAReactiveTaker> SMAReactiveMap;
    typedef SMAReactiveMap::iterator SMAReactiveMapIterator;

public:
    SMAReactiveTakerStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName);
    ~SMAReactiveTakerStrategy();

public:
    virtual void OnBar(const BarEventMsg& msg);
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);
    void OnResetStrategyState();
    void DefineStrategyParams();
    void RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate);

private:
    void AdjustPortfolio(const Instrument* instrument, int desired_position);
    void SendOrder(const Instrument* instrument, int trade_size);

private:
    SMAReactiveMap sma_reactive_map_;
    int position_size_;
    bool debug_;
    int sma_window_size_;
};

#endif // _STRATEGY_STUDIO_LIB_EXAMPLES_SMA_REACTIVE_TAKER_STRATEGY_H_
