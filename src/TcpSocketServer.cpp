#include "../include/TcpSocketServer.h"

#include <iostream>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#elif __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#define PORT "1922"

/**
 * In order to use WSA on Windows
 * we need to initialize the Windows Sockets DLL
 */
void TcpSocketServer::InitializeSocket() {
#ifdef WIN32
    std::cout << "Initializing Windows Sockets DLL\n";

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cout << "Could not initialize Windows Sockets DLL\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Initialize successful.\n";
#endif
}

addrinfo * TcpSocketServer::GetAddrInfoResult() {
    struct addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *result = nullptr;
    if (getaddrinfo(nullptr, PORT, &hints, &result) != 0) {
        std::cout << "getaddrinfo failed.\n";
#ifdef WIN32
        WSACleanup();
#endif
        exit(EXIT_FAILURE);
    }

    return result;
}

TcpSocketServer::TcpSocketServer() {
    InitializeSocket();
}



