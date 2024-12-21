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
    SMAReactiveTaker(int sma_window_size = 100) 
        : price_window_(sma_window_size), 
          sma_window_size_(sma_window_size), 
          previous_price_(0), 
          momentum_days_(0) {}

    void Reset() {
        price_window_.clear();
        previous_price_ = 0;
        momentum_days_ = 0;
    }

    SignalType Update(double current_price) {
        price_window_.push_back(current_price);

        if (!price_window_.full()) {
            return SIGNAL_NONE; // Wait until SMA is fully initialized
        }

        double sma = price_window_.Mean();
        SignalType signal = SIGNAL_NONE;

        if (current_price > sma) {
            if (previous_price_ > sma && current_price > previous_price_) {
                momentum_days_++;
            } else {
                momentum_days_ = 1; // Reset momentum count
            }

            if (momentum_days_ >= 2) {
                signal = SIGNAL_BUY;
            }
        } else if (current_price < sma) {
            if (previous_price_ < sma && current_price < previous_price_) {
                momentum_days_++;
            } else {
                momentum_days_ = 1; // Reset momentum count
            }

            if (momentum_days_ >= 2) {
                signal = SIGNAL_SELL;
            }
        }

        previous_price_ = current_price;
        return signal;
    }

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
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg) {}
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

SMAReactiveTakerStrategy::SMAReactiveTakerStrategy(StrategyID strategyID, const std::string& strategyName, const std::string& groupName)
    : Strategy(strategyID, strategyName, groupName), sma_reactive_map_(), position_size_(100), debug_(false), sma_window_size_(100) {}

SMAReactiveTakerStrategy::~SMAReactiveTakerStrategy() {}

void SMAReactiveTakerStrategy::OnResetStrategyState() {
    sma_reactive_map_.clear();
}

void SMAReactiveTakerStrategy::DefineStrategyParams() {
    params().CreateParam(CreateStrategyParamArgs("position_size", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_INT, position_size_));
    params().CreateParam(CreateStrategyParamArgs("sma_window_size", STRATEGY_PARAM_TYPE_STARTUP, VALUE_TYPE_INT, sma_window_size_));
    params().CreateParam(CreateStrategyParamArgs("debug", STRATEGY_PARAM_TYPE_RUNTIME, VALUE_TYPE_BOOL, debug_));
}

void SMAReactiveTakerStrategy::RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate) {
    for (SymbolSetConstIter it = symbols_begin(); it != symbols_end(); ++it) {
        eventRegister->RegisterForBars(*it, BAR_TYPE_TIME, 1);
    }
}

void SMAReactiveTakerStrategy::OnBar(const BarEventMsg& msg) {
    if (debug_) {
        ostringstream str;
        str << msg.instrument().symbol() << ": " << msg.bar();
        logger().LogToClient(LOGLEVEL_DEBUG, str.str().c_str());
    }

    // Check if this instrument has an SMA calculator; if not, create one
    SMAReactiveMapIterator sma_iter = sma_reactive_map_.find(&msg.instrument());
    if (sma_iter == sma_reactive_map_.end()) {
        sma_iter = sma_reactive_map_.insert(make_pair(&msg.instrument(), SMAReactiveTaker(sma_window_size_))).first;
    }

    SignalType signal = sma_iter->second.Update(msg.bar().close());

    // Adjust portfolio based on the generated signal
    if (signal == SIGNAL_BUY) {
        AdjustPortfolio(&msg.instrument(), position_size_);
    } else if (signal == SIGNAL_SELL) {
        AdjustPortfolio(&msg.instrument(), -position_size_);
    }
}

void SMAReactiveTakerStrategy::AdjustPortfolio(const Instrument* instrument, int desired_position) {
    int trade_size = desired_position - portfolio().position(instrument);

    if (trade_size != 0 && orders().num_working_orders(instrument) == 0) {
        SendOrder(instrument, trade_size);
    }
}

void SMAReactiveTakerStrategy::SendOrder(const Instrument* instrument, int trade_size) {
    double price = (trade_size > 0) ? instrument->top_quote().bid() : instrument->top_quote().ask();

    OrderParams params(*instrument,
                       abs(trade_size),
                       price,
                       MARKET_CENTER_ID_NASDAQ,
                       (trade_size > 0) ? ORDER_SIDE_BUY : ORDER_SIDE_SELL,
                       ORDER_TIF_DAY,
                       ORDER_TYPE_MARKET);

    trade_actions()->SendNewOrder(params);
}

#endif // _STRATEGY_STUDIO_LIB_EXAMPLES_SMA_REACTIVE_TAKER_STRATEGY_H_
