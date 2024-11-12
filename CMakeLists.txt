cmake_minimum_required(VERSION 3.16)
project(Ctcp_echo_server)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the shared header file
include_directories(${PROJECT_SOURCE_DIR})

# Create an executable for the server
add_executable(server
    server.cpp
    shared.h
)

# Create an executable for the client
add_executable(client
    client.cpp
    shared.h
)