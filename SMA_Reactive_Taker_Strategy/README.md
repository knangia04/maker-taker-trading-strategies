# SMA (Simple Moving Averages) based Reactive Taker Strategy

## About:

This strategy uses the Simple Moving Average (SMA) as a dynamic indicator to identify support and resistance levels for trading SPY (S&P 500 ETF). The strategy generates buy and sell signals based on price interactions with the SMA.

#### Main Idea:

1) Computes a 100-day SMA to identify medium-term price trends.
2) The SMA acts as a dynamic support level when the price is above it and a resistance level when the price is below it.
3) **Signal Logic**:
    * Buy Signal: Triggered when the price remains above the SMA for two consecutive days, indicating an upward trend continuation.
    * Sell Signal: Triggered when the price remains below the SMA for two consecutive days, signaling a downward trend continuation.
    * We also factor in momentum to ensure that the upward/downward direction is maintained for 2 consecutive days i.e. price[i] > price[i-1] (BUY) or price[i] < price[i-1] (SELL)
4) **Implementation**:
    * Currently the strategy is set to buy with all the available capital (for simplicity), but **this is a very risky proposition**.
    * **Future Work**: Use some sort of metric to evaluate the bet size, perhaps **Kelly Criterion**

* The ipynb file serves as a preliminary analysis file to test out the idea and see if it is profitable
* We then implement the same idea using Strategy Studio in order to backtest it rigorously!!

## Running:

#### Jupyter Notebook:
Once the environment is setup and activated, the necessary dependencies should already be ready to use. Just run each cell, and change code as required for additional testing.

#### Strategy Studio BackTest:

```
chmod +x run_strategy.sh
./run_strategy.sh
```

Then visualize the output using the scripts in the **jupyter notebook** for result_visualization

___