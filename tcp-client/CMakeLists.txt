cmake_minimum_required(VERSION 3.28)
project(tcp-client)

set(CMAKE_CXX_STANDARD 17)

add_executable(client)
target_sources(client
        PRIVATE
        client.cpp
        src/TcpClientSocket.cpp
        include/TcpClientSocket.h)

IF (WIN32)
    target_link_libraries(client Ws2_32.lib)
    target_link_libraries(client Mswsock.lib)
    target_link_libraries(client AdvApi32.lib)
    target_link_libraries(client -static)
ENDIF ()