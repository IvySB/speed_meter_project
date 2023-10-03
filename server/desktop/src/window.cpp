#include "window.h"
#include "setting.h"
#include "comservices.h"

Window::Window(COMService *coms) : communication{coms}
{

    layout.addWidget(&speedTextLable, 0, 0);
    speedTextLable.setAlignment(Qt::AlignRight);

    speedValueLable.setFixedWidth(60);
    layout.addWidget(&speedSlider, 0, 1);
    layout.addWidget(&speedValueLable, 0, 2);

    layout.addWidget(&batteryTextLable, 1, 0);
    layout.addWidget(&batterySlider, 1, 1);
    layout.addWidget(&batteryValueLable, 1, 2);

    layout.addWidget(&tempTextLable, 2, 0);
    tempTextLable.setAlignment(Qt::AlignRight);

    layout.addWidget(&tempSlider, 2, 1);
    layout.addWidget(&tempValueLable, 2, 2);

    layout.addWidget(&lightTextLable, 3, 0);

    layoutCheckbox.addWidget(&leftBlink, 0, 0);
    layoutCheckbox.addWidget(&rightBlink, 0, 1);
    layoutCheckbox.addWidget(&warningBlink, 0, 2);

    connect(&leftBlink, &QCheckBox::stateChanged, this, [this](int state)
            { rightBlink.setEnabled(state == 0); 
           if(warningBlink.isChecked()) {
                    communication->setLightLeft(true);
                } else {
                    communication->setLightLeft(leftBlink.isChecked());
                } });

    connect(&rightBlink, &QCheckBox::stateChanged, this, [this](int state)
            { leftBlink.setEnabled(state == 0);
                if(warningBlink.isChecked()) {
                    communication->setLightRight(true);
                } else {
                    communication->setLightRight(rightBlink.isChecked());
                } });

    connect(&warningBlink, &QCheckBox::stateChanged, this, [this](int state)
            { 
                if(warningBlink.isChecked()) {
                    communication->setLightLeft(true);
                    communication->setLightRight(true);
                } else {
                    communication->setLightLeft(leftBlink.isChecked());
                    communication->setLightRight(rightBlink.isChecked());
                } });

    layout.addLayout(&layoutCheckbox, 3, 1);

    setLayout(&layout);

    speedSlider.setOrientation(Qt::Horizontal);
    speedSlider.setRange(0, 240);
    speedSlider.setValue(0);

    batterySlider.setOrientation(Qt::Horizontal);
    batterySlider.setRange(0, 100);
    batterySlider.setValue(0);

    tempSlider.setOrientation(Qt::Horizontal);
    tempSlider.setRange(-60, 60);
    tempSlider.setValue(0);

    setWindowTitle("Server");
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setFixedHeight(Setting::Server::Window::Height);
    setFixedWidth(Setting::Server::Window::Width);

    connect(&speedSlider, &QSlider::valueChanged, this, &Window::updateSpeedValue);
    connect(&batterySlider, &QSlider::valueChanged, this, &Window::updateBatteryValue);
    connect(&tempSlider, &QSlider::valueChanged, this, &Window::updateTempValue);
}

void Window::updateSpeedValue(uint32_t value)
{

    communication->setSpeed(value);
    speedValueLable.setText(QString::number(value) + " Kph");
}

void Window::updateTempValue(int32_t value)
{
    communication->setTemperature(value);
    tempValueLable.setText(QString::number(value) + " °C");
}

void Window::updateBatteryValue(uint32_t value)
{
    communication->setBattryLevel(value);
    batteryValueLable.setText(QString::number(value) + " %");
}
