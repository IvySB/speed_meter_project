#ifndef UARTSERVICE_H
#define UARTSERVICE_H

#include <QThread>
#include "comservice.h"

class UARTService : public COMService, public QThread
{
    std ::atomic<bool> end{false};

    void run(void) override;

public:
    UARTService()
    {
        start();
    };

    ~UARTService()
    {
        end = true;
        wait();
    };
};

#endif
