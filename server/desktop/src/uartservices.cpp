#include "setting.h"
#include <QSerialPort>
#include "uartservices.h"
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
        if (serial.open(QIODevice::WriteOnly))
        {
            while (!end && serial.isWritable())
            {
                uint8_t temp[sizeof(buffer)]{0};

                {
                    std::scoped_lock<std::mutex> locker{mtx};
                    memcpy(temp, buffer, sizeof(temp));
                }

                if (sizeof(temp) == serial.write(reinterpret_cast<const char *>(temp), sizeof(temp)))
                {
                    if (serial.waitForBytesWritten(Setting::INTERVAL))
                    {
                        status = true;
                        msleep(Setting::INTERVAL / 2);
                    }
                    else
                    {
                        qDebug() << "Timeout error!";
                        status = false;
                        break; // Exit the inner loop if sending fails
                    }
                }
                else
                {
                    qDebug() << "Failed to write data";
                    status = false;
                    break; // Exit the inner loop if sending fails
                }
            }
        }
        else
        {
            qDebug() << "Failed to open the port";
        }

        if (serial.isOpen())
        {
            serial.close();
        }
    }
}
