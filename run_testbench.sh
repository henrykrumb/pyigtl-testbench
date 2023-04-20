#!/bin/bash

./bin/testbench_server -v &
python3 -m pytest python/testbench_client.py

#python3 -m pytest python/testbench_server.py &
#./bin/testbench_client
