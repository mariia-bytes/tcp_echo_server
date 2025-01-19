/************************************************
 * This server.cpp file includes all logic
 * behind server part of the network:
 *  1. create a socket
 *  2. bind the socket to the address
 *  3. listen for connection
 *  4. accept the connection
 *  5. receive and send messages
 *  6. close the socket
 * 
 * The main function takes as a parameter 
 * a port number for the  server to run.
************************************************/
#include "shared.h"

void run_server(int port);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <Port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);

    run_server(port);
    return 0;
}



void run_server(int port) {
    // create a listening socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    // check if the socket exists
    if (listening == -1) {
        std::cerr << "Can't create a socket";
        return;
    }

    // bind the socket to (any) IP/port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    hint.sin_addr.s_addr = INADDR_ANY;  // bind to any available network interface

    // check if bind command works
    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) {
        std::cerr << "Can't bind to IP/port";
        return;
    }

    // make the socket as being able to listen
    if (listen(listening, SOMAXCONN) == -1) {
        std::cerr << "Can't listen";
        return;
    }

    std::cout << "\nWaiting for client to connect...\n" << std::endl;

    // accept a call
    sockaddr_in client;
    socklen_t client_size = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int client_socket = accept(listening, (sockaddr*)&client, &client_size);

    if (client_socket == -1) {
        std::cerr << "Problem with client connecting";
        return;
    }

    // close the listening socket
    close(listening);

    // clean what garbage might be left in memory 
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if (result == 0) {
        std::cout << std::endl << host << " connected on " << svc << std::endl << std::endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << std::endl << host << " connected on " << ntohs(client.sin_port) << std::endl << std::endl;
    }

    // just for fun, let's keep track of the time
    auto start = std::chrono::high_resolution_clock::now();

    // while receiving display message, echo message
    char buff[4096];
    while (true) {
        // clear the buffer
        memset(buff, 0, 4096);
        // wait for the message
        int bytes_recv = recv(client_socket, buff, 4096, 0);
        if (bytes_recv == -1) {
            std::cout << "There was a connection issue" << std::endl;
            break;
        }
        if (bytes_recv == 0) {
            std::cout << "\nThe client's disconnected" << std::endl;
            break;
        }

        // display message
        std::cout << "Received: " << std::string(buff, 0, bytes_recv) << std::endl;

        // resend message
        send(client_socket, buff, bytes_recv, 0);       
    }

    // close the socket
    close(client_socket);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    
    std::cout << "\n*************** Session ***************" << std::endl;
    std::cout << "Elapsed time: " << elapsed << " secs" << std::endl;
    std::cout << "Connection closed..." << std::endl;
}