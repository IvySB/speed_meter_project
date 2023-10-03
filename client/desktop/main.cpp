#ifdef UARTCOM
#include "uartservice.h"
#else
#include "tcpclient.h"
#endif

#include "windows.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

#ifdef UARTCOM

    UARTService service;

#else
    TCPClient service;
#endif

    Windows windows(&service);
    windows.show();

    return app.exec();
}
