# Toy Strategy - SMA

## About:

The goal of this strategy is to see if Strategy Studio is setup properly and if we can test our strategies as needed. 

**Note:** The Strategy here is the same as the example SMA strategy provided along with Strategy Studio, additional scripts developed execute it conveniently.

## Running:

*.h and .cpp files are the exact same as those in the examples dir of strategy studio, execute the bash script as shown below to test if everything works as expected*

Modifications:
1) Makefile: in order to ensure correct path
2) LocalDevServer (if executing here):
```
touch cmd_config.txt
```
and fill file with:

```
USERNAME=username
PASSWORD=password
SERVER=127.0.0.1
PORT=13001
#runmode: 1=Backtesting, 2=Live Simulation, 3=Production, 4=LocalDevelopment
RUN_MODE=1
```

**Note:** Set to execute on the backtesting server and not the LocalDevServer

```
chmod +x run_strategy.sh
./run_strategy.sh
```