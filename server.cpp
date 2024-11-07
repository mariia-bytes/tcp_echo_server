/********************************************
 * This server.cpp file includes all logic
 * behide server part of the network:
 *          1. creating a socket
 *          2. binding to IP and port
 *          3. listenig for incoming messages
 *          4. receiving messages
 *          5. echoing messages
 *          6. closing the socket
 *          7. cleaning the memory
*********************************************/

#include "shared.h"

void run_server() {
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
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    // check if bind command works
    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) {
        std::cerr << "Can't bind to IP/port";
        return;
    }

    // make the socket as being able listening in
    if (listen(listening, SOMAXCONN) == -1) {
        std::cerr << "Can't listen";
        return;
    }

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
        std::cout << host << " connected on " << svc << std::endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
    }

    // while receiving display message, echo message
    char buff[4096];
    while (true) {
        // clear the buffer
        memset(buff, 0 ,4096);
        // wait for the message
        int bytes_recv = recv(client_socket, buff, 4096, 0);
        if (bytes_recv == -1) {
            std::cout << "There was a connection issue" << std::endl;
            break;
        }
        if (bytes_recv == 0) {
            std::cout << "The client disconnected" << std::endl;
            break;
        }

        // display message
        std::cout << "Received: " << std::string(buff, 0, bytes_recv) << std::endl;

        // resend message
        send(client_socket, buff, bytes_recv, 0);       
    }

    // close the socket
    close(client_socket);
}