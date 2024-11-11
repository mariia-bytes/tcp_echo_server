/************************************************
 * This client_main.cpp file includes executable
 * part for client. 
 * The main function takes as a run time parameter
 * the server adress (ip and port).
************************************************/

#include "shared.h"

void run_client(const std::string& ip_address, int port);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP Address> <Port>" << std::endl;
        return 1;
    }

    std::string ip_address = argv[1];
    int port = std::stoi(argv[2]);

    run_client(ip_address, port);
    return 0;
}