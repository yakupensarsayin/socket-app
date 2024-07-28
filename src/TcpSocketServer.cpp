#include "../include/TcpSocketServer.h"

#include <iostream>

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
        Cleanup();
        exit(EXIT_FAILURE);
    }

    return result;
}

SocketType TcpSocketServer::CreateServerSocket(addrinfo* result) {
    std::cout << "Creating server socket.\n";

    const SocketType serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    // INVALID_SOCKET in Windows equals to ~0 hence -1
    if (serverSocket == -1) {
        std::cout << "Failed to create a socket.\n";
        freeaddrinfo(result);
        Cleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Server socket created.\n";

    return serverSocket;
}

void TcpSocketServer::SetSocketOptions(addrinfo* result) const {

    std::cout << "Setting server socket options.\n";

    constexpr int iOptVal = 1;

    if (setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR,
        reinterpret_cast<OptValType>(&iOptVal), sizeof(int)) == -1) {
        std::cout << "Failed to set socket options\n";
        freeaddrinfo(result);
        Cleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Server socket options successfully set.\n";
}

void TcpSocketServer::Cleanup() {
#ifdef WIN32
    WSACleanup();
#endif
}

TcpSocketServer::TcpSocketServer() {
    InitializeSocket();
    addrinfo *result = GetAddrInfoResult();
    mServerSocket = CreateServerSocket(result);
    SetSocketOptions(result);
}
