// create a socket
// bind the socket to the IP/port
// mark the socket for listening in
// accept a call
// close the listening socket
// while receiving display message, echo message
// close the socket

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int main() {
    // create a listening socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    // check if the socket exists
    if (listening == -1) {
        std::cerr << "Can't create a socket";
        return -1;
    }

    // bind the socket to (any) IP/port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    // check if bind command works
    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) {
        std::cerr << "Can't bind to IP/port";
        return -2;
    }

    // make the socket as being able listening in
    if (listen(listening, SOMAXCONN) == -1) {
        std::cerr << "Can't listen";
        return -3;
    }

    // accept a call
    sockaddr_in client;
    socklen_t client_size = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int client_socket = accept(listening, (sockaddr*)&client, &client_size);

    if (client_socket == -1) {
        std::cerr << "Problem with client connecting";
        return -4;
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

    return 0;
}
