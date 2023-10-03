#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <comservices.h>

class Window : public QDialog
{

    QGridLayout layout;
    QGridLayout layoutCheckbox;

    QSlider speedSlider;
    QSlider batterySlider;
    QSlider tempSlider;

    QCheckBox leftBlink{"Left"};
    QCheckBox rightBlink{"Right"};
    QCheckBox warningBlink{"Warning"};

    QLabel speedTextLable{"Speed:"}, speedValueLable{"0 Kph"};
    QLabel batteryTextLable{"BatteryLevel:"}, batteryValueLable{"0 %"};
    QLabel tempTextLable{"Temprature:"}, tempValueLable{"0 °C"};
    QLabel lightTextLable{"Light signals:"};

    COMService *communication{nullptr};

    bool status{false};

public:
    Window(COMService *coms);

private:
    void updateSpeedValue(uint32_t value);
    void updateBatteryValue(uint32_t value);
    void updateTempValue(int32_t value);
};
#endif
