# pyigtl-testbench

Testbench for pyigtl, evaluating pyigtl against the C++ reference implementation of IGTLink.
Although pyigtl includes its own unit tests, the module should also be tested against the
reference implementation (OpenIGTLink) to ensure it is compatible with other IGTLink applications.


## Architecture

Both OpenIGTLink and pyigtl allow users to easily create server or client connections for bidirectional data
transfer.
Which means that per message type, we have four routes to test:
  * OpenIGTLink Server --> pyigtl Client
  * OpenIGTLink Server --> pyigtl Client
  * OpenIGTLink Client <-- pyigtl Server
  * OpenIGTLink Client <-- pyigtl Server

## Usage

First, build the C++ part of the test bench.

```bash
mkdir build
cd build
cmake ..
make
```

Then, execute the run_testbench.sh shell script.


## Message types

So far, the following message types are included:

  * TRANSFORM
