cmake_minimum_required(VERSION 3.28)
project(tcp-server)

set(CMAKE_CXX_STANDARD 17)

add_executable(server)
target_sources(server
        PRIVATE
            server.cpp
            include/TcpSocketServer.h
            src/TcpSocketServer.cpp)

IF (WIN32)
    target_link_libraries(server Ws2_32.lib)
    target_link_libraries(server -static)
ENDIF ()