#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "comservice.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <atomic>

class TCPClient : public COMService
{

    int clientSocket;
    std::atomic<bool> end{false};
    std::thread thrd{&TCPClient::run, this};

    void run(void) override;

public:
    TCPClient() = default;

    ~TCPClient()
    {
        end = true;
        close(clientSocket);
        thrd.join();
    };
};

#endif // TCPCLIENT_H