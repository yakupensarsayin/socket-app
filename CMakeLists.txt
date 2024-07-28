cmake_minimum_required(VERSION 3.28)
project(socket_app)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} tcp-server/server.cpp
        tcp-server/src/TcpSocketServer.cpp
        tcp-server/include/TcpSocketServer.h
        tcp-client/main.cpp)

IF (WIN32)
    target_link_libraries(${PROJECT_NAME} Ws2_32.lib)
    target_link_libraries(${PROJECT_NAME} -static)
ENDIF ()