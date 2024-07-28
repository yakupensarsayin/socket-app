cmake_minimum_required(VERSION 3.28)
project(socket_app)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tcp-client)
add_subdirectory(tcp-server)