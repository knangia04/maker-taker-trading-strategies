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

This report provides a detailed overview of a **grid trading strategy** designed for use in volatile markets. The strategy aims to capture profits from price fluctuations within a defined range, minimizing the risk of major losses while profiting from frequent market moves. The report covers the basic concept, execution, and performance evaluation of the strategy.

## 2. Strategy Overview

### 2.1. Grid Trading Concept

The **grid trading strategy** is based on the idea of placing buy and sell orders at regular intervals above and below the current market price. The strategy is designed to capture profits from price fluctuations within a predefined range. It operates best in sideways or ranging markets, where the price oscillates between support and resistance levels, creating opportunities for multiple trades.

In grid trading, orders are placed ahead of time, ensuring that trades are executed automatically when the price hits the designated levels. The trader profits from both upward and downward price movements as positions are opened and closed within the grid levels.

### 2.2. Key Components

The main components of a grid trading strategy include:

1. **Grid Size**: The distance between each buy and sell order in the grid. For example, a grid size of 50 pips would place orders every 50 pips above and below the current market price.
2. **Grid Depth**: The total number of grid levels that are placed above and below the market price. A greater depth allows for more trading opportunities but increases exposure to larger price moves.
3. **Position Sizing**: The amount of capital allocated to each order in the grid. Position sizing can be fixed or variable, with some strategies increasing the position size at each level as the market moves.
4. **Buy and Sell Orders**: Orders are placed at predefined levels above and below the current market price. The strategy executes trades automatically as the price moves through these levels.

### 2.3. Strategy Execution

The implementation of the grid trading strategy involves the following steps:

1. **Initialization**: The strategy starts by identifying the current market price and calculating the grid levels based on predefined intervals.
2. **Order Placement**: Buy and sell limit orders are placed at regular intervals above and below the current price, creating a grid of orders. These orders are designed to be executed as the market price fluctuates.
3. **Order Execution**: When the market price reaches a grid level, the corresponding buy or sell order is executed, opening a new position. The strategy then looks for opportunities to close positions once the price reaches a predefined target or if the price moves back in the opposite direction.

### 2.4. Risk Management

To ensure the strategy operates efficiently and with minimal risk, **risk management techniques** are applied:
1. **Stop-Loss Orders**: A stop-loss order may be placed at certain grid levels to limit potential losses. For example, a stop-loss order could be placed at a level where the price moves against the position by a predetermined amount.
2. **Take-Profit Orders**: A take-profit order is used to close positions at predefined profit levels, locking in profits when the price moves in the trader's favor.
3. **Position Closing**: In some variations of the strategy, positions may be closed once a predetermined profit threshold has been reached, effectively locking in profits and protecting against market reversals.

## 3. Performance Evaluation

The grid trading strategy was tested on a selection of asset pairs to evaluate its effectiveness under different market conditions. The strategy's performance was assessed based on the following key metrics:

1. **Profitability**: The total return generated by the strategy over a specified period.
2. **Risk-Adjusted Return**: Metrics such as the **Sharpe Ratio** and **Sortino Ratio** were used to evaluate the strategy's return relative to its risk.
3. **Drawdown**: The maximum loss from a peak to a trough, which indicates the level of risk exposure for the strategy.
4. **Win Rate**: The percentage of profitable trades relative to the total number of trades executed.

## 4. Conclusion

The **grid trading strategy** is a versatile and efficient method for profiting from price fluctuations in sideways or volatile markets. Its systematic nature allows for fully automated trading, making it an attractive option for traders seeking a hands-off approach. While the strategy works well in range-bound markets, it is crucial to consider risk management techniques to prevent significant losses in trending markets.

Future work on the grid trading strategy will focus on refining the grid size and depth, implementing dynamic risk management features, and testing the strategy across a broader range of market conditions.
