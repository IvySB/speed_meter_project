#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include "setting.h"
#include <QSoundEffect>

class Canvas : public QWidget
{
    QPen Pen;
    QBrush brush;
    QFont iconText{"Arial"};
    QFont iconFont{"Material Icons"};
    QPainter painter;
    QSoundEffect blinkersound;

    int batteryLevel{0};
    int speed{0};
    int tempreature{0};
    bool status{false};
    bool leftlight{false};
    bool rightlight{false};

public:
    Canvas();

    void setBatteryLevel(int level) { batteryLevel = level; }
    void setspeed(int spd) { speed = spd; }
    void settempreature(int tmp) { tempreature = tmp; }
    void statuses(int sts) { status = sts; }
    void setleftright(bool left, bool right)
    {
        leftlight = left,
        rightlight = right;
    }

private:
    void paintEvent(QPaintEvent *event) override;

    void fontBatteryLevel(void);
    void fontTmpLevel(void);
    void fontBlinkers(void);
    void fontSpeedometer(void);
};

#endif