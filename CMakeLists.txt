cmake_minimum_required(VERSION 3.16)
project(ClientServerApp)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the shared header file
include_directories(${PROJECT_SOURCE_DIR})

# Create an executable for the server
add_executable(server
    server_main.cpp
    server.cpp
    shared.h
)

# Create an executable for the client
add_executable(client
    client_main.cpp
    client.cpp
    shared.h
)