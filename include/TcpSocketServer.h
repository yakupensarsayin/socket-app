#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

#define PORT "1783"

class TcpSocketServer {

public:
    TcpSocketServer();

private:
    static void InitializeSocket();
    static struct addrinfo *GetAddrInfoResult();
};

#endif //TCPSOCKETSERVER_H
