#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
typedef SOCKET SocketType;
typedef const char* OptValType;
#elif __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
typedef int SocketType;
typedef const void* OptValType;
#endif

#define PORT "1922"

class TcpSocketServer {

public:
    TcpSocketServer();

private:
    SocketType mServerSocket;

    static void InitializeSocket();
    static addrinfo *GetAddrInfoResult();
    static SocketType CreateServerSocket(struct addrinfo* result);
    void SetSocketOptions(addrinfo *result) const;
    static void Cleanup();
};

#endif //TCPSOCKETSERVER_H
