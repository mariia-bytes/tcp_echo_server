/************************************************
 * This server_main.cpp file includes executable
 * part for server.
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