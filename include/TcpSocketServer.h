#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
typedef SOCKET SocketType;
typedef const char* OptValType;
typedef int AddrLenType;
#elif __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
typedef int SocketType;
typedef const void* OptValType;
typedef socklen_t AddrLenType;
#endif

#define PORT "1922"

class TcpSocketServer {

public:
    TcpSocketServer();

private:
    SocketType mServerSocket{};
    addrinfo *mResult{};

    static void InitializeSocket();
    void GetAddrInfoResult();
    void CreateServerSocket();
    void SetSocketOptions() const;
    void BindSocket() const;
    void ListenSocket() const;
    void CloseSocket() const;
    static void Cleanup();
};

#endif //TCPSOCKETSERVER_H
