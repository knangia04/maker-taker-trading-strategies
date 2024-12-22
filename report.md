# Final Report
### Team Members and Their Contributions

- **[Khushi Kalra]**:
  - Implementation of Maker Strategy in Python using ipynb
  - Implementation of Maker Strategy in Cpp 
  - Final Project Report

- **[Siddarth Ananath]**:
  - Initial Setup
  - Toy Strategy
  - Developed scripts for: result_visualization (SS output), bash scripts for automating backtests (run_strategy.sh) to simplify tasks across repo
  - Implementation and backtesting of Taker Strategy in Python using ipynb
  - Implementation and backtesting of Taker Strategy in Cpp using Strategy Studio
  - Github ReadMe's
  - Taker Demo Video

- **[Asuthosh Anandaram ]**:
  - Market Taker Ideation and Cpp implementation
  - Github cleanup + ReadMe's
  - Documeting taker and maker strategies

- **[Krish Nangia]**:
  - Maker Strategy Ideation
  - Implementation and succesful backtesting of Maker Strategy in Python using ipynb
  - Implementation and (attempted) backtesting of Maker Strategy in Cpp using Strategy Studio
  - Maker Demo Video


# SMA (Simple Moving Averages) Based Reactive Taker Strategy

## Abstract
This report outlines the **SMA (Simple Moving Average)-based Reactive Taker Strategy**, a quantitative trading algorithm designed to trade SPY (S&P 500 ETF) using the 100-day Simple Moving Average (SMA) as a dynamic support and resistance indicator. The strategy leverages moving averages to identify potential buy and sell signals, focusing on price interactions with the 100-day SMA. The core principle of this strategy is that market behavior, especially in medium-term trends, can be predicted using historical price averages. This report provides an in-depth look into the strategy, including its conceptual framework, implementation details, results, and areas for future development.

## Introduction
Algorithmic trading, or automated trading, has revolutionized financial markets by using mathematical models to make data-driven decisions at speeds and frequencies that are impossible for human traders. Among the many indicators used in algorithmic trading, **moving averages** are one of the simplest yet most powerful tools. The **Simple Moving Average (SMA)** smooths price data over a specific period and helps traders identify the direction of the trend, as well as potential reversals.

This report focuses on a strategy using the **100-day Simple Moving Average (SMA)** as a core indicator. The 100-day period is selected for its ability to capture medium-term market trends without being overly reactive to short-term price fluctuations. The strategy identifies trading signals based on price interactions with the SMA, and it aims to profit from sustained trends while minimizing exposure to short-term market noise.

The report discusses the logic behind the SMA-based strategy, details the current implementation and testing process, and outlines the next steps required to improve the strategy’s performance and risk management.

## Strategy Overview

### Moving Average Calculation
At the heart of this trading strategy lies the **100-day Simple Moving Average (SMA)**, which is calculated as the average of the closing prices of SPY over the past 100 trading days. The **SMA** serves as an indication of the market’s medium-term trend:

- **Bullish Trend**: When the current price is above the 100-day SMA, it suggests a bullish or upward trend, indicating that the market is in an uptrend.
- **Bearish Trend**: When the current price is below the 100-day SMA, it suggests a bearish or downward trend, indicating that the market is in a downtrend.

The primary assumption in this strategy is that the price is likely to continue following the trend established by the SMA until significant external factors intervene.

### Dynamic Support and Resistance
The 100-day SMA not only helps determine the trend direction but also acts as a dynamic level of **support** or **resistance**:
- **Support**: If the price is above the 100-day SMA, the SMA serves as a potential support level. A pullback toward this level could indicate a buying opportunity, as the price is expected to bounce back upwards.
- **Resistance**: If the price is below the 100-day SMA, the SMA serves as a resistance level. A rally toward this level may signal a sell opportunity, as the price could reverse downward once it hits resistance.

These dynamics of support and resistance are integral to identifying entry and exit points in the market, with price action around the SMA providing valuable information about potential trends.

## Signal Logic

### Buy Signal
A **Buy Signal** is generated when:
1. The price has been above the 100-day SMA for two consecutive trading days. This indicates that the market is in an uptrend.
2. The price has been increasing each day for two consecutive days. This is confirmed by checking the condition `price[i] > price[i-1]` for two consecutive days. This confirms the continuation of the upward trend and suggests that momentum is building.

In other words, the price must not only be above the SMA but also show a positive momentum over the last two trading days, ensuring that the trend is strong enough to warrant a buying decision.

### Sell Signal
A **Sell Signal** is generated when:
1. The price has been below the 100-day SMA for two consecutive trading days. This signals that the market is in a downtrend.
2. The price has been decreasing each day for two consecutive days. This is confirmed by checking the condition `price[i] < price[i-1]` for two consecutive days, which indicates that momentum is driving the price downward.

Similar to the buy signal, the sell signal is based on both the price being below the SMA (indicating a downtrend) and the momentum being negative for the last two days.

### Momentum Consideration
The strategy incorporates **momentum** as a key factor in confirming the strength of the trend. By ensuring that the price moves consistently in one direction over two consecutive days, the strategy minimizes the risk of reacting to market noise or short-term fluctuations. This method helps improve the robustness of the signals and reduces the likelihood of false positives.

## Current Implementation

#### **1. Introduction**
- The **SMA-Based Reactive Taker Strategy** is a trading algorithm designed to capitalize on price interactions with the Simple Moving Average (SMA) as a dynamic support and resistance indicator.
- Development progressed in two stages:
  1. **Prototype Implementation in Python**: For proof of concept using historical SPY data.
  2. **Advanced Implementation in C++**: For integration with **Strategy Studio**, enabling real-time market data analysis and live trading.
- The goal is to refine and deploy a systematic trading approach that is both profitable and resilient to market volatility.

---

#### **2. Initial Python Implementation**
##### **2.1 Methodology**
- **Data Source**:
  - Historical SPY data (January 1, 2020 – December 31, 2023) fetched using the `yfinance` library.
- **Indicators**:
  - **100-Day SMA**: Used as a benchmark for buy and sell signals.
  - **Momentum**: Incorporated to confirm the trend and filter out noise.
- **Signal Generation**:
  - **Buy Signal**: When the current price > previous price > SMA.
  - **Sell Signal**: When the current price < previous price < SMA.

##### **2.2 Backtesting Framework**
- **Capital Allocation**:
  - Entire portfolio invested when a buy signal was triggered.
  - Full liquidation when a sell signal occurred.
- **Portfolio Value Calculation**:
  - Tracked daily, accounting for the execution of trades.

##### **2.3 Results and Insights**
- **Initial Capital**: $10,000.
- **Final Portfolio Value**: $14,854.48.
- **Return**: 48.5% over three years.
- **Observations**:
  - Profitable during the testing period, effectively capturing medium-term trends.
  - Lacked diversification and advanced risk management mechanisms, leading to potential overexposure in volatile markets.

---

#### **3. Advanced Implementation in C++ with Strategy Studio**
##### **3.1 Objectives**
- Transition the Python implementation into **C++** to leverage **Strategy Studio**’s capabilities:
  - Real-time integration with market data (IEX).
  - Advanced execution logic and risk management.
  - Scalability for live deployment.

##### **3.2 Key Features**
1. **Momentum Indicators**:
   - Utilized rolling windows for:
     - **Short SMA (9-day)**: Capturing short-term trends.
     - **Long SMA (30-day)**: Capturing medium-term trends.
2. **Signal Logic**:
   - Similar to the Python version but with additional validation for momentum.
3. **Dynamic Position Sizing**:
   - Introduced the `position_size` parameter to adjust trade sizes dynamically based on market conditions.
4. **Risk Management**:
   - Implemented order repricing to minimize slippage and improve execution efficiency.
5. **Execution Logic**:
   - Orders placed at optimal bid/ask prices with continuous monitoring for market changes.

##### **3.3 Backtesting Results**
- Conducted in **Strategy Studio** using historical data from IEX.
- Results showed consistent profitability under varying market conditions.
- **Improvements Over Python Version**:
  - Enhanced position sizing reduced exposure during high volatility.
  - Optimized order placement minimized execution delays and slippage.

#### **4. Conclusion**
- The **SMA-Based Reactive Taker Strategy** has demonstrated significant potential:
  - **Python Prototype**: Validated the concept with a profitable backtest.
  - **C++ Implementation**: Enhanced scalability, execution, and risk management for real-world deployment.
- Future work will focus on optimizing performance metrics, refining risk management, and adapting the strategy to a broader range of market conditions.
- With ongoing refinements, the strategy is positioned for successful deployment in both simulated and live trading environments.

**Appendix**: 
Future developments will explore ways to enhance the momentum filtering process and consider integrating additional technical indicators to further validate the trade signals. Moreover, the potential for using machine learning techniques to predict market trends will be investigated to improve the strategy's predictive accuracy.

---

# Final Project Report: Grid Trading Strategy

## 1. Introduction

In the world of automated trading, one of the most popular and robust strategies is **grid trading**. Grid trading involves placing buy and sell orders at predefined intervals above and below the current market price, creating a "grid" of orders that capitalize on market fluctuations. This strategy is particularly useful in markets characterized by high volatility, where prices oscillate within a range rather than trend in one direction.

The grid trading strategy doesn't require advanced market predictions. Instead, it assumes that price will fluctuate in a relatively stable range over time. By placing both buy and sell orders, the strategy aims to profit from these oscillations as the market moves up and down, filling orders at regular intervals.

## 2. Objective

The objective of this project is to implement and backtest a **grid trading strategy** on SPY (S&P 500 ETF) using historical data. This strategy places buy and sell orders at set intervals, creating a grid that captures market oscillations and profits from price fluctuations within a given range. The backtesting process will simulate real-market trading, providing insights into the performance and robustness of the strategy.

## 3. Strategy Overview

The **Grid Trading Strategy** operates by defining a grid of buy and sell orders around the current market price. The key components of this strategy include:

- **Grid Size**: The distance between buy and sell orders. This distance is measured in points or percentage terms and determines the width of the grid.
- **Number of Orders**: The total number of buy and sell orders placed in the grid.
- **Order Size**: The amount of SPY purchased or sold per order. This can be constant or dynamically adjusted based on account balance or risk parameters.

### Key Assumptions

- **Market Oscillation**: The strategy assumes that the market price will oscillate within a range over time, making it suitable for sideways or range-bound markets.
- **Profit from Reversals**: The goal is to profit from price reversals by capturing small price movements up and down within the grid.

### Trade Execution
1. **Buy Orders**: Buy orders are placed at intervals below the current market price.
2. **Sell Orders**: Sell orders are placed at intervals above the current market price.
3. **Order Refill**: Once an order is filled, it is immediately replaced with a new order at the next interval, keeping the grid intact.

## 4. Backtesting Process

The backtesting process involves testing the grid trading strategy against historical SPY data to evaluate its effectiveness. The following steps were taken to implement the backtest:

1. **Data Collection**: Historical SPY data (price and volume) for a specific period was collected.
2. **Grid Setup**: The grid size and number of orders were defined.
3. **Order Execution**: Simulated orders were placed at predefined intervals based on market movements.
4. **Performance Evaluation**: Key metrics such as total return, maximum drawdown, and win rate were calculated.

## 5. Performance Metrics

The performance of the grid trading strategy was evaluated using the following metrics:

- **Total Return**: The overall return generated by the strategy over the test period.
- **Maximum Drawdown**: The largest peak-to-trough loss during the testing period.
- **Win Rate**: The percentage of trades that resulted in a profit.

## 6. Results and Discussion

The results of the backtest indicate that the grid trading strategy performs well in a range-bound market with consistent oscillations. However, the strategy's performance is sensitive to the choice of grid size and the volatility of the market during the testing period. Larger grid sizes tend to perform better during high-volatility periods, while smaller grid sizes are more suited for low-volatility, range-bound markets.

## 7. Conclusion

The **Grid Trading Strategy** offers a straightforward approach to capitalizing on market oscillations. It works best in range-bound markets where prices are expected to fluctuate within a defined range. However, its performance can be highly sensitive to market volatility, and proper grid size selection is crucial to its success.

Future work will focus on optimizing the grid size, improving risk management techniques, and integrating advanced technical indicators to refine the grid trading strategy. Further backtesting across various market conditions will also be necessary to assess the robustness and adaptability of the strategy.
