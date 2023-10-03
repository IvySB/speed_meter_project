#ifndef UARTSERVICES_H
#define UARTSERVICES_H

#include <QThread>
#include "comservices.h"

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
