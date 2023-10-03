
#include <climits>
#include "comservices.h"

void COMService::insert(uint32_t start, uint32_t length, uint32_t value)
{
    uint32_t pos = start % CHAR_BIT;
    uint32_t index = start / CHAR_BIT;

    std::scoped_lock<std::mutex> lock(mtx);

    for (uint32_t i = 0; i < length; i++)
    {
        if (((value >> i) & 1) != 0)
        {
            buffer[index] |= (1 << pos);
        }
        else
        {
            buffer[index] &= ~(1 << pos);
        }

        pos++;

        if (pos == CHAR_BIT)
        {
            pos = 0;
            index++;
        }
    }
}

void COMService::setSpeed(uint32_t value)
{
    insert(Setting::Signal::Speed::START, Setting::Signal::Speed::LENGTH, value);
}

void COMService::setTemperature(int32_t value)
{

    insert(Setting::Signal::Temperture::START, Setting::Signal::Temperture::LENGTH, value);
}

void COMService::setBattryLevel(uint32_t value)
{
    insert(Setting::Signal::BatteryLevel::START, Setting::Signal::BatteryLevel::LENGTH, value);
}

void COMService::setLightLeft(bool value)
{
    insert(Setting::Signal::Light::Left::START, Setting::Signal::Light::Left::LENGTH, value ? 1U : 0);
}

void COMService::setLightRight(bool value)
{
    insert(Setting::Signal::Light::Right::START, Setting::Signal::Light::Right::LENGTH, value ? 1U : 0);
}