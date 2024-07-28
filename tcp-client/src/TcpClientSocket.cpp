#include "../include/TcpClientSocket.h"

#include <iostream>
#include <cstring>

TcpClientSocket::TcpClientSocket() {
    InitializeSocket();
    GetAddrInfoResult();
    AttemptToConnect();
    SendData();
    SocketShutdown();
    ReceiveUntilPeerClose();
    CloseSocket();
    Cleanup();
}

void TcpClientSocket::InitializeSocket() {
#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cout << "Could not initialize Windows Sockets DLL\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Windows DLL initialization is successful.\n";
#endif
}

void TcpClientSocket::GetAddrInfoResult() {
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo *result = nullptr;
    if (getaddrinfo(NODE, PORT, &hints, &result) != 0) {
        std::cout << "getaddrinfo failed.\n";
        Cleanup();
        exit(EXIT_FAILURE);
    }

    mResult = result;
}

void TcpClientSocket::AttemptToConnect() {

    for(const addrinfo* ptr = mResult; ptr != nullptr; ptr = ptr->ai_next) {

        mSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (mSocket == -1) {
            std::cout << "Failed to create a socket.\n";
            freeaddrinfo(mResult);
            Cleanup();
            exit(EXIT_FAILURE);
        }

        if (connect(mSocket, ptr->ai_addr, static_cast<AddrLenType>(mResult->ai_addrlen)) == -1) {
            CloseSocket();
            mSocket = -1;
            continue;
        }
        break;
    }

    freeaddrinfo(mResult);

    if (mSocket == -1) {
        std::cout << "Unable to connect to the Server.\n";
        Cleanup();
        exit(EXIT_FAILURE);
    }
}

void TcpClientSocket::SendData() const {

    std::string input;

    do {
        std::cout << "Message:";
        std::getline(std::cin, input);

        if (const char* sendbuf = input.c_str();
            send(mSocket, sendbuf, static_cast<int>(strlen(sendbuf)), 0) == -1) {
                std::cout << "Send failed.\n";
                CloseSocket();
                Cleanup();
                exit(EXIT_FAILURE);
        }

    } while (input != "exit");


}

void TcpClientSocket::SocketShutdown() const {

    // SD_SEND for Windows = 1
    // SHUT_WR for Linux = 1
    // Both disallow further sends.
    if (shutdown(mSocket, 1) == -1) {
        std::cout << "Shutdown failed.\n";
        CloseSocket();
        Cleanup();
        exit(EXIT_FAILURE);
    }

}

void TcpClientSocket::ReceiveUntilPeerClose() const {

    int iResult;

    do {
        constexpr int recvlen = BUFLEN;
        char recvbuf[BUFLEN];

        iResult = recv(mSocket, recvbuf, recvlen, 0);

        if (iResult > 0)
            std::cout << "Bytes received: " << iResult << "\n";
        else if ( iResult == 0 )
            std::cout << "Connection closed!\n";

    } while( iResult > 0 );
}

void TcpClientSocket::CloseSocket() const {
#ifdef WIN32
    closesocket(mSocket);
#elif __linux__
    close(mSocket);
#endif
}

void TcpClientSocket::Cleanup() {
#ifdef WIN32
    WSACleanup();
#endif
}
