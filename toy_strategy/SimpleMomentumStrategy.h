#pragma once

#ifndef _STRATEGY_STUDIO_LIB_EXAMPLES_MARKET_KING_GRID_STRATEGY_H_
#define _STRATEGY_STUDIO_LIB_EXAMPLES_MARKET_KING_GRID_STRATEGY_H_

#ifdef _WIN32
    #define _STRATEGY_EXPORTS __declspec(dllexport)
#else
    #ifndef _STRATEGY_EXPORTS
    #define _STRATEGY_EXPORTS
    #endif
#endif

#include <Strategy.h>
#include <MarketModels/Instrument.h>
#include <Utilities/ParseConfig.h>
#include <vector>
#include <map>
#include <iostream>

using namespace RCM::StrategyStudio;

class Grid {
public:
    Grid(double grid_spacing, double lower_bound, double upper_bound)
        : grid_spacing_(grid_spacing), lower_bound_(lower_bound), upper_bound_(upper_bound) {
        InitializeGrid();
    }

    void InitializeGrid() {
        grid_levels_.clear();
        for (double price = lower_bound_; price <= upper_bound_; price += grid_spacing_) {
            grid_levels_.push_back(price);
        }
    }

    void Reset(double new_lower_bound, double new_upper_bound) {
        lower_bound_ = new_lower_bound;
        upper_bound_ = new_upper_bound;
        InitializeGrid();
    }

    const std::vector<double>& GetGridLevels() const { return grid_levels_; }

private:
    double grid_spacing_;
    double lower_bound_;
    double upper_bound_;
    std::vector<double> grid_levels_;
};

class MarketKingGrid : public Strategy {
public:
    MarketKingGrid(StrategyID strategyID, const std::string& strategyName, const std::string& groupName);
    ~MarketKingGrid();

public: /* from IEventCallback */
    virtual void OnBar(const BarEventMsg& msg);
    virtual void OnOrderUpdate(const OrderUpdateEventMsg& msg);
    void OnResetStrategyState();
    void OnParamChanged(StrategyParam& param);

private:
    void PlaceGridOrders(const Instrument* instrument);
    void AdjustPortfolio(const Instrument* instrument, double current_price);

private: /* from Strategy */
    virtual void RegisterForStrategyEvents(StrategyEventRegister* eventRegister, DateType currDate);
    virtual void DefineStrategyParams();
    virtual void DefineStrategyCommands();

private:
    boost::unordered_map<const Instrument*, Grid> grid_map_;
    double grid_spacing_;
    double upper_bound_;
    double lower_bound_;
    int order_size_;
    bool debug_;
};

extern "C" {
    _STRATEGY_EXPORTS const char* GetType()
    {
        return "MarketKingGridStrategy";
    }

    _STRATEGY_EXPORTS IStrategy* CreateStrategy(const char* strategyType,
                                   unsigned strategyID,
                                   const char* strategyName,
                                   const char* groupName) {
        if (strcmp(strategyType, GetType()) == 0) {
            return *(new MarketKingGrid(strategyID, strategyName, groupName));
        } else {
            return NULL;
        }
    }

    _STRATEGY_EXPORTS const char* GetAuthor() {
        return "YourName";
    }

    _STRATEGY_EXPORTS const char* GetAuthorGroup() {
        return "YourGroup";
    }

    _STRATEGY_EXPORTS const char* GetReleaseVersion() {
        return Strategy::release_version();
    }
}

#endif
