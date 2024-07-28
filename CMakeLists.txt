cmake_minimum_required(VERSION 3.28)
project(socket_app)

set(CMAKE_CXX_STANDARD 17)

add_executable(socket_app main.cpp
        src/TcpSocketServer.cpp
        include/TcpSocketServer.h)

IF (WIN32)
    target_link_libraries(socket_app Ws2_32.lib)
ENDIF ()