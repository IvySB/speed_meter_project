#include "setting.h"
#include <QSerialPort>
#include "uartservice.h"
#include <QDebug>

void UARTService::run(void)
{
    QSerialPort serial;

    serial.setPortName(Setting::UART::PORT);
    serial.setBaudRate(Setting::UART::BAUDRATE);
    serial.setParity(QSerialPort::Parity::NoParity);
    serial.setDataBits(QSerialPort::DataBits::Data8);
    serial.setStopBits(QSerialPort::StopBits::OneStop);

    while (!end)
    {

        if (serial.open(QIODevice::ReadOnly))
        {
            while (!end && serial.isReadable())
            {
                serial.clear();
                uint8_t temp[sizeof(buffer)]{0};

                if (serial.waitForReadyRead(Setting::INTERVAL))
                {
                    if (sizeof(temp) == serial.read(reinterpret_cast<char *>(temp), sizeof(temp)))
                    {
                        std::scoped_lock<std::mutex> lock(mtx);
                        memcpy(buffer, temp, sizeof(buffer));
                        status = true;
                    }
                    else

                    {
                        qDebug() << "Error Read";
                        status = false;
                        break;
                    }
                }
                else
                {
                    qDebug() << "Error Timeout";
                    status = false;
                    break;
                }
            }
        }
        else
        {
            qDebug() << "Failed to open serial port.";
        }

        if (serial.isOpen())
        {
            serial.close();
        }
    }
}
