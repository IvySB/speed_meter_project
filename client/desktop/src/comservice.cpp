#include <climits>
#include "comservice.h"

void COMService::extract(uint32_t start, uint32_t length, uint32_t &value)
{
    value = 0;

    int pos = start % CHAR_BIT;
    int index = start / CHAR_BIT;

    std::scoped_lock<std::mutex> lock(mtx);

    for (uint32_t i = 0; i < length; i++)
    {
        if (((buffer[index] >> pos) & 1) != 0)
        {
            value |= (1 << i);
        }

        pos++;

        if (pos == CHAR_BIT)
        {
            pos = 0;
            index++;
        }
    }
}

void COMService::extract(uint32_t start, uint32_t length, int32_t &value)
{
    extract(start, length, reinterpret_cast<uint32_t &>(value));

    if ((value & (1 << (length - 1))) != 0)
    {
        value |= ~((1 << length) - 1);
    }
}

uint32_t COMService::getSpeed(void)
{
    uint32_t value{0};
    extract(Setting::Signal::Speed::START, Setting::Signal::Speed::LENGTH, value);
    return value;
}

uint32_t COMService::getTemperature(void)
{

    int32_t value{0};
    extract(Setting::Signal::Temperture::START, Setting::Signal::Temperture::LENGTH, value);
    return value;
}

uint32_t COMService::getBattryLevel(void)
{
    uint32_t value{0};
    extract(Setting::Signal::BatteryLevel::START, Setting::Signal::BatteryLevel::LENGTH, value);
    return value;
}

bool COMService::getLightLeft(void)
{
    uint32_t value{0};
    extract(Setting::Signal::Light::Left::START, Setting::Signal::Light::Left::LENGTH, value);
    return (value != 0);
}

bool COMService::getLightRight(void)
{
    uint32_t value{0};
    extract(Setting::Signal::Light::Right::START, Setting::Signal::Light::Right::LENGTH, value);
    return (value != 0);
}
