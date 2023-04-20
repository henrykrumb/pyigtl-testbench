#include <iostream>
#include <math.h>
#include <cstdlib>
#include <csignal>

#include "igtlOSUtil.h"
#include "igtlPositionMessage.h"
#include "igtlServerSocket.h"
#include "igtlTrackingDataMessage.h"
#include "igtlTransformMessage.h"
#include "igtlPointMessage.h"

#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"


int main(int argc, char* argv[])
{
    int port = 18944;
    int timeout = 1000;

    // parse command line args
    CLI::App app{"IGTLink Testbench Client"};
    app.add_option("-p,--port", port, "Server port");
    app.add_option("-t,--timeout", timeout, "Connection timeout");
    CLI11_PARSE(app, argc, argv);
}
