#!/bin/bash

# Set resource limits
echo "Setting resource limits..."
ulimit -t 3600  # CPU time limit: 1 hour
ulimit -v $((6 * 1024 * 1024))  # Memory limit: 6GB
echo "Resource limits set."

# Configuration variables
SO_FILE="SimpleMomentumStrategy.so"
STRATEGY_NAME="SimpleMomentumStrategy"
INSTANCE_NAME="Toy"
ACCOUNT="UIUC"
USER="dlariviere"
START_BALANCE=1000000
SYMBOLS="AAPL,MSFT"
START_DATE="2021-11-05"
END_DATE="2021-11-05"
LATENCY=10
BASE_DIR="/home/vagrant/ss/bt"
DLLS_DIR="$BASE_DIR/strategies_dlls"
OUTPUT_DIR="$BASE_DIR/backtesting-results/csv_files"
LOG_FILE="$BASE_DIR/logs/main_log.txt"
WORK_DIR=$PWD

# Step 0: Build the .so file
echo "Step 0: Cleaning and building the strategy..."
BUILD_DIR="/home/vagrant/group_02_project/toy_strategy"  # Update this path
cd "$BUILD_DIR" || { echo "Build directory not found! Exiting."; exit 1; }
make clean
if make; then
    echo "Build successful!"
else
    echo "Build failed! Exiting..."
    exit 1
fi
echo "Step 0 completed."

# Step 1: Deploy the .so file
echo "Step 1: Deploying $SO_FILE to the DLLs directory..."
mkdir -p "$DLLS_DIR"
cp "$SO_FILE" "$DLLS_DIR/"
echo "Step 1 completed."

# Step 2: Start the Strategy Server
echo "Step 2: Starting the Strategy Server..."
cd "$BASE_DIR" || { echo "LocalDevServer directory not found! Exiting."; exit 1; }
./StrategyServerBacktesting &
SERVER_PID=$!
sleep 1  # Allow time for the server to start
echo "Strategy Server started with PID $SERVER_PID."
echo "Step 2 completed."

# Step 3: Create a strategy instance
echo "Step 3: Creating a strategy instance..."
cd "$BASE_DIR/utilities" || { echo "Utilities directory not found! Exiting."; exit 1; }

echo "Terminating existing instance (if any)..."
./StrategyCommandLine cmd terminate "$INSTANCE_NAME"

echo "Creating a new strategy instance..."
./StrategyCommandLine cmd create_instance "$INSTANCE_NAME" "$STRATEGY_NAME" "$ACCOUNT" "$USER" "$START_BALANCE" -symbols "$SYMBOLS"
./StrategyCommandLine cmd set_simulator_params -latency_to_exchange "$LATENCY"

echo "Listing all strategy instances..."
./StrategyCommandLine cmd strategy_instance_list
echo "Step 3 completed."

# Step 4: Start backtest
echo "Step 4: Starting the backtest..."
logFileNumLines=$(cat "$LOG_FILE" | wc -l)
foundFinishedLogFile=$(grep -nr "finished.$" "$LOG_FILE" | gawk '{print $1}' FS=":" | tail -n1)

./StrategyCommandLine cmd start_backtest "$START_DATE" "$END_DATE" "$INSTANCE_NAME" 0

# Monitor backtest completion
echo "Monitoring backtest progress..."
while ((logFileNumLines > foundFinishedLogFile)); do
    foundFinishedLogFile=$(grep -nr "finished.$" "$LOG_FILE" | gawk '{print $1}' FS=":" | tail -n1)
    echo "Waiting for strategy to finish... Current log line: $foundFinishedLogFile"
    sleep 10
done
echo "Backtest completed."
echo "Step 4 completed."

# Step 5: Export results
echo "Step 5: Exporting CRA file to CSV..."
latestCRA=$(ls "$BASE_DIR/backtesting-results/BACK_*.cra" -t | head -n1)
echo "Latest CRA file: $latestCRA"

./StrategyCommandLine cmd export_cra_file "$latestCRA" "$OUTPUT_DIR"
echo "CRA file exported to $OUTPUT_DIR."

# Move CSV files to output directory
echo "Moving CSV files to output directory..."
mkdir -p "$WORK_DIR/output"
mv "$OUTPUT_DIR"/* "$WORK_DIR/output/"
rm "$latestCRA"
echo "Step 5 completed."

# Step 6: Terminate the Strategy Server
echo "Step 6: Terminating the Strategy Server..."
kill $SERVER_PID
echo "Strategy Server terminated."
echo "Step 6 completed."

echo "Script execution completed successfully!"
