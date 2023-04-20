#include <iostream>
#include <math.h>
#include <cstdlib>
#include <csignal>

#include "igtlOSUtil.h"
#include "igtlMessageHeader.h"
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


void test_message_transform(igtl::Socket::Pointer& client_socket, int& test_success, int& test_failure) {
    float matrix[4][4] = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };

    std::cout << "Sending message" << std::endl;

    // send message of type TRANSFORM
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

    // receive modified TRANSFORM message from client
    bool timeout_status = false;

    std::cout << "Receiving message" << std::endl;
    
    auto header_message = igtl::MessageHeader::New();
    header_message->InitPack();
    if (!client_socket->Receive(header_message->GetPackPointer(), header_message->GetPackSize(), timeout_status)) {
        // ...
    }

    header_message->Unpack();
    if (strcmp(header_message->GetDeviceName(), "transform-modified") == 0) {
        test_success++;
    }
    else {
        test_failure++;
    }
    // TODO check if message type is correct

    transform_message->SetMessageHeader(header_message);
    transform_message->AllocatePack();
    if (client_socket->Receive(transform_message->GetPackBodyPointer(), transform_message->GetPackBodySize(), timeout_status)) {
        test_success++;

        transform_message->Unpack();
        transform_message->GetMatrix(matrix);
        if (matrix[0][0] == 42.0f) {
            test_success++;
        }
        else {
            test_failure++;
        }
    }
    else {
        test_failure++;
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
    app.add_flag("-v,--verbose", verbose, "Write command line output");
    CLI11_PARSE(app, argc, argv);

    auto server_socket = igtl::ServerSocket::New();
    int status = server_socket->CreateServer(port);

    if (status < 0) {
        std::cerr << "Cannot create a server socket." << std::endl;
        std::cerr << "    status: " << status << std::endl;
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, signal_handler);


    int test_success = 0;
    int test_failure = 0;

    running = true;

    std::cout << "IGTLink Server running on port " << port << "." << std::endl;

    igtl::Socket::Pointer client_socket;

    while (running)
    {
        client_socket = server_socket->WaitForConnection(timeout);

        if (client_socket.IsNotNull() && client_socket->GetConnected()) {
            std::cout << "Client connected." << std::endl;
            // TRANSFORM message type
            test_message_transform(client_socket, test_success, test_failure);
            running = false;
        }

        igtl::Sleep(10);
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
