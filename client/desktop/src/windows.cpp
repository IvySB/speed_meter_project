#include "windows.h"
#include "setting.h"

Windows::Windows(COMService *com) : communication{com}
{

    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle("Client");
    setFixedHeight(Setting::Client::Window::Height);
    setFixedWidth(Setting::Client::Window::Width);
    setContentsMargins(0, 0, 0, 0);

    layout.addWidget(&canvas, 0, 0, 1, 3);
    layout.setContentsMargins(0, 0, 0, 0);

    setLayout(&layout);

    canvas.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    canvas.setFixedSize(800, 560);

    connect(&timer, &QTimer::timeout, this, &Windows::refresh);
    timer.start(Setting::INTERVAL);
}

void Windows::refresh()
{
    if (communication->getStatus())
    {
        canvas.statuses(true);
        canvas.setBatteryLevel(communication->getBattryLevel());
        canvas.settempreature(communication->getTemperature());
        canvas.setspeed(communication->getSpeed());
        canvas.setleftright(communication->getLightLeft(), communication->getLightRight());
    }
    else
    {
        canvas.statuses(false);
        canvas.setBatteryLevel(0);
        canvas.settempreature(0);
        canvas.setspeed(0);
        canvas.setleftright(false, false);
    }

    canvas.update();
}
