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


static bool running;

void signal_handler(int signum) {
    if (signum == SIGINT) {
        running = false;
    }
}


int main(int argc, char* argv[])
{
    int port = 18944;
    int timeout = 1000;
    bool verbose = false;

    // parse command line args
    CLI::App app{"IGTLink Testbench Server"};
    app.add_option("-p,--port", port, "Server port");
    app.add_option("-t,--timeout", timeout, "Connection timeout");
    app.add_option("-v,--verbose", verbose, "Write command line output");
    CLI11_PARSE(app, argc, argv);

    auto server_socket = igtl::ServerSocket::New();
    int status = server_socket->CreateServer(port);
    igtl::Socket::Pointer client_socket;

    if (status < 0) {
        std::cerr << "Cannot create a server socket." << std::endl;
        std::cerr << "    status: " << status << std::endl;
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, signal_handler);

    float position[] = { 1.0, 2.0, 3.0 };
    float quaternion[] = {4.0, 5.0, 6.0, 7.0};
    float matrix[4][4] = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };
    double orientation[] = { 1.0, 1.0, 1.0 };


    int test_success = 0;
    int test_failure = 0;

    bool connected = false;
    running = true;

    std::cout << "IGTLink Server running on port " << port << "." << std::endl;

    while (running)
    {
        client_socket = server_socket->WaitForConnection(timeout);

        while (client_socket.IsNotNull() && client_socket->GetConnected()) {
            if (!connected) {
                std::cout << "Client connected." << std::endl;
                connected = true;
            }

            // TRANSFORM message type
            auto transform_message = igtl::TransformMessage::New();
            transform_message->SetDeviceName("transform");
            transform_message->SetMatrix(matrix);
            transform_message->Pack();

            if (client_socket->Send(transform_message->GetPackPointer(), transform_message->GetPackSize())) {
                test_success++;
            }
            else {
                test_failure++;
            }

            igtl::Sleep(10);

            break;
        }

        if (connected) {
            std::cout << "Client disconnected." << std::endl;
            running = false;
            connected = false;
        }
    }

    if (client_socket.IsNotNull()) {
        client_socket->CloseSocket();
    }

    if (verbose) {
        std::cout << "OpenIGTLink Server <-> pyigtl Client tests finished." << std::endl;
        std::cout << "    Success: " << test_success << std::endl;
        std::cout << "    Failure: " << test_failure << std::endl;
    }

    if (test_failure == 0) {
        return 0;
    }

    return EXIT_FAILURE;
}
