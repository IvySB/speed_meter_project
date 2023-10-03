#ifdef UARTCOM
#include "uartservices.h"
#else
#include "tcpserver.h"
#endif

#include "window.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

#ifdef UARTCOM

    UARTService service;

#else
    TCPServer service;
#endif

    Window window(&service);
    window.show();

    return app.exec();
}
