#include "../include/TcpSocketServer.h"

#include <iostream>

/**
 * In order to use WSA on Windows
 * we need to initialize the Windows Sockets DLL
 */
void TcpSocketServer::InitializeSocket() {
#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cout << "Could not initialize Windows Sockets DLL\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Windows DLL initialization is successful.\n";
#endif
}

void TcpSocketServer::GetAddrInfoResult() {
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

    mResult = result;
}

void TcpSocketServer::CreateServerSocket() {

    const SocketType serverSocket = socket(mResult->ai_family, mResult->ai_socktype, mResult->ai_protocol);

    // INVALID_SOCKET in Windows equals to ~0 hence -1
    if (serverSocket == -1) {
        std::cout << "Failed to create a socket.\n";
        freeaddrinfo(mResult);
        Cleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Server socket created.\n";

    mServerSocket = serverSocket;
}

void TcpSocketServer::SetSocketOptions() const {

    constexpr int iOptVal = 1;

    if (setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR,
        reinterpret_cast<OptValType>(&iOptVal), sizeof(int)) == -1) {
        std::cout << "Failed to set socket options.\n";
        freeaddrinfo(mResult);
        Cleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Server socket options successfully set.\n";
}

void TcpSocketServer::BindSocket() const {

    if (bind(mServerSocket, mResult->ai_addr, static_cast<AddrLenType>(mResult->ai_addrlen)) != 0) {
        std::cout << "Binding failed.\n";
        freeaddrinfo(mResult);
        CloseSocket();
        Cleanup();
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(mResult);

    std::cout << "Server socket succesfully bound.\n";
}

void TcpSocketServer::ListenSocket() const {

    if (listen(mServerSocket, SOMAXCONN) == -1) {
        std::cout << "Listen failed.\n";
        CloseSocket();
        Cleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Server socket is listening for new connections...\n";
}

void TcpSocketServer::CloseSocket() const {
#ifdef WIN32
    closesocket(mServerSocket);
#elif __linux__
    close(mServerSocket);
#endif
}

void TcpSocketServer::Cleanup() {
#ifdef WIN32
    WSACleanup();
#endif
}

TcpSocketServer::TcpSocketServer() {
    InitializeSocket();
    GetAddrInfoResult();
    CreateServerSocket();
    SetSocketOptions();
    BindSocket();
    ListenSocket();
}
