#include "setting.h"
#include "tcpserver.h"
#include <arpa/inet.h>
#include <QDebug>

void TCPServer::run(void)
{
    serversocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serversocket == -1)
    {
        qDebug() << "Failed to create socket.";
        std::exit(1);
    }

    int temp = 1;
    if (0 != setsockopt(serversocket, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int)))
    {
        qDebug() << "setsockopt(SO_REUSEADDR) failed";
        std::exit(1);
    }

    sockaddr_in server_address{0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(Setting::TCP::PORT);
    inet_pton(AF_INET, Setting::TCP::IP, &server_address.sin_addr);

    if (0 != bind(serversocket, (struct sockaddr *)&server_address, sizeof(server_address)))
    {
        qDebug() << "failed to bind socket";
        std::exit(1);
    }
    if (0 != listen(serversocket, 1))
    {
        qDebug() << "Failed to listen to the port...";
        std::exit(1);
    }

    while (!end) // Continue until 'end' flag is set
    {
        sockaddr_in cli{0};
        socklen_t len = sizeof(cli);

        int connfd = accept(serversocket, (sockaddr *)&cli, &len);

        if (connfd != -1)
        {
            while (!end) // Continue sending buffer until 'end' flag is set
            {
                status = true;
                uint8_t temp[sizeof(buffer)]{0};

                {
                    std::scoped_lock<std::mutex> locker{mtx};
                    memcpy(temp, buffer, sizeof(temp));
                }

                if (sizeof(temp) != write(connfd, temp, sizeof(temp)))
                {
                    status = false;
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(Setting::INTERVAL / 2));
            }

            close(connfd); // Close the client connection
        }
    }
}