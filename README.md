# pyigtl-testbench

Testbench for pyigtl, evaluating pyigtl against the C++ reference implementation of IGTLink.
Although pyigtl includes its own unit tests, the module should also be tested against the
reference implementation (OpenIGTLink) to ensure it is compatible with other IGTLink applications.


## Architecture




## Usage

First, build the C++ part of the test bench.

```bash
mkdir build
cd build
cmake ..
make
```


## Message types

So far, the following message types are included:

  * TRANSFORM
