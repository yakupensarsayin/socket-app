#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H


#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
typedef SOCKET SocketType;
typedef int AddrLenType;
#elif __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
typedef int SocketType;
typedef socklen_t AddrLenType;
#endif

#define PORT "1922"
#define BUFLEN 512
#define NODE "127.0.0.1"

class TcpClientSocket {

public:
    TcpClientSocket();

private:
    SocketType mSocket{};
    addrinfo* mResult{};

    static void InitializeSocket();
    void GetAddrInfoResult();
    void AttemptToConnect();
    void SendData() const;
    void SocketShutdown() const;
    void ReceiveUntilPeerClose() const;
    void CloseSocket() const;
    static void Cleanup();
};

#endif //TCPCLIENTSOCKET_H
