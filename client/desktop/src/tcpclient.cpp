#include "tcpclient.h"
#include "setting.h"
#include <QDebug>

void TCPClient::run(void)
{

    sockaddr_in server_address{0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(Setting::TCP::PORT);
    server_address.sin_addr.s_addr = inet_addr(Setting::TCP::IP);
    while (!end)
    {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket != -1)
        {
            if (0 == connect(clientSocket, (struct sockaddr *)&server_address, sizeof(server_address)))
            {
                status = true;
                while (!end)
                {
                    uint8_t temp[sizeof(buffer)]{0};
                    if (sizeof(temp) == read(clientSocket, temp, sizeof(temp)))
                    {
                        std::scoped_lock<std::mutex> lock(mtx);
                        memcpy(buffer, temp, sizeof(buffer));
                    }
                    else
                    {
                        status = false;
                        close(clientSocket);
                        break;
                    }
                }
            }
        }
    }
}