# Final Report
### Team Members and Their Contributions

- **[Khushi Kalra]**:
  - Wrote initial code for the backtesting framework, allowing historical data to be analyzed in C++for the Market Making Strategy.
  - Final Project Report

- **[Soddharth Ananath]**:
  - Wrote initial code for the backtesting framework, allowing historical data to be analyzed in C++for the Market Taking Strategy.
  - Demo Video
  

- **[Asuthosh Anandaram ]**:
  - Worked on the python code for the Market Taking Strategy
  - Github cleanup and adding ReadMe sections for both srategies

- **[Krish Nagia]**:
  - Worked on the python code for the Grid Strategy


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

### Basic Strategy
The initial implementation of the SMA-based Reactive Taker Strategy involves buying SPY when a buy signal is triggered. This is a simplified version of the strategy where the entire available capital is allocated to buying SPY upon a buy signal. The strategy, as it stands, does not include any risk management techniques or position sizing strategies.

The current goal is to test the fundamental concept of the strategy—whether the interaction of price with the 100-day SMA provides meaningful buy and sell signals. A larger portion of the development work will focus on refining the strategy to include position sizing and risk management.

### Preliminary Analysis
Testing was carried out using historical SPY data within a Jupyter Notebook. The results have provided valuable insights into how the strategy performs under different market conditions. Early tests suggest that the SMA-based strategy captures medium-term trends quite effectively but requires optimization in terms of capital allocation, risk management, and backtesting.

### Next Steps
While the basic implementation successfully identifies potential buy and sell signals, future versions of the strategy will focus on improving its ability to manage risk and optimize capital usage. As it stands, the strategy buys with all available capital, which may result in significant exposure and risk.

## Future Work

### Risk Management and Position Sizing
The current implementation does not include any risk management techniques, making it vulnerable to large losses in highly volatile market conditions. To improve this, the strategy will incorporate the **Kelly Criterion** for position sizing, which will help determine the optimal amount of capital to allocate to each trade based on the expected probability of winning and the potential risk involved.

The **Kelly Criterion** provides a mathematical formula that maximizes the expected logarithmic growth of capital, ensuring that the risk of losing all capital is minimized while still achieving long-term growth. Using this formula, the strategy can dynamically adjust the size of each position, increasing exposure when the probabilities of success are high and reducing exposure when the market is uncertain.

### Backtesting with Strategy Studio
The next step is to backtest the strategy using a more advanced simulation platform, **Strategy Studio**. This platform will allow for more comprehensive testing, including running the strategy on large historical datasets and evaluating its performance across different market conditions. Backtesting will help identify any issues with the strategy's assumptions, and it will provide a clearer picture of its potential profitability.

- **Backtesting Process**: 
  - Once the code is ready, it will be executed within the Strategy Studio environment, where historical data will be fed into the system, simulating real-market conditions.
  - Results from the backtests will be analyzed, and visualizations will be created in a Jupyter Notebook to assess the strategy's performance.

### Performance Metrics and Evaluation
The strategy will be evaluated using various performance metrics, including:
1. **Sharpe Ratio**: Measures the risk-adjusted return of the strategy, comparing the strategy's return to its volatility. A higher Sharpe Ratio indicates better risk-adjusted performance.
2. **Maximum Drawdown**: Measures the maximum loss from a peak to a trough in the strategy’s equity curve. This metric is essential for understanding the risk exposure of the strategy.
3. **Total Return**: The overall return generated by the strategy over the testing period, providing a clear picture of its profitability.
4. **Win Rate**: The percentage of profitable trades relative to the total number of trades, indicating how often the strategy successfully captures trends.

These metrics will provide a comprehensive view of the strategy’s potential for consistent profitability and risk management.

## Conclusion
The **SMA-based Reactive Taker Strategy** offers a promising approach to algorithmic trading by leveraging the **100-day Simple Moving Average (SMA)** as a dynamic support and resistance level. The strategy identifies buy and sell signals based on price interactions with the SMA and incorporates momentum to reduce noise from short-term price fluctuations.

Although the strategy demonstrates potential, further work is needed to incorporate risk management techniques, such as position sizing and the **Kelly Criterion**, to improve its effectiveness. Additionally, comprehensive backtesting and performance evaluation will be essential to refine the strategy and ensure its robustness in a variety of market conditions.

Ultimately, the **SMA-based Reactive Taker Strategy** offers a systematic approach to trading SPY, but further development and testing are necessary to optimize risk management and maximize profitability.

---

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
