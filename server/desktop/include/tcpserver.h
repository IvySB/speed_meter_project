#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <QDebug>
#include <arpa/inet.h>
#include "comservices.h"
#include <thread>
#include <unistd.h>
#include <sys/socket.h>

class TCPServer : public COMService
{

    int serversocket;
    std::atomic<bool> end{false}; // Atomic flag to indicate when the service should stop.

    std::thread thrd{&TCPServer::run, this};

    void run(void) override;

public:
    TCPServer() = default;
    ~TCPServer()
    {

        end = true;
        shutdown(serversocket, SHUT_RDWR);
        close(serversocket);
        thrd.join();
    }
};
#endif // TCPSERVER_H