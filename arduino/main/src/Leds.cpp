#include "Leds.h"

Leds::Leds() {}

Leds::~Leds() {}

void Leds::setup()
{
    ledStrip.begin();
    ledStrip.show();
    ledStrip.setBrightness(brightness);
}

void Leds::blink(char* color, int interval, unsigned long time)
{
    uint32_t colorValue = getColor(color);

    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        if (ledIsOn == 0)
        {
            ledStrip.fill(colorValue);
            ledStrip.show();
            ledIsOn = 1;
        }
        else
        {
            ledStrip.clear();
            ledStrip.show();
            ledIsOn = 0;
        }
    }
}

void Leds:: still(char* color)
{
    uint32_t colorValue = getColor(color);

    ledStrip.fill(colorValue);
    ledStrip.show();

}

void Leds::forwardBlink(char* color, int interval, unsigned long time)
{
    uint32_t colorValue = getColor(color);

    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        if (ledIsOn == 0)
        {
            ledStrip.fill(colorValue, 3, 17);
            ledStrip.show();
        }
        else
        {
            ledStrip.clear();
            ledStrip.show();
        }
    }
}

void Leds::backwardBlink(char* color, int interval, unsigned long time)
{
    uint32_t colorValue = getColor(color);

    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        if (ledIsOn == 0)
        {
            ledStrip.fill(colorValue, 20, 13);
            // Needed because we reach the end of the strip with the line above so we need
            // to start over from 0 to complete the half circle.
            ledStrip.fill(colorValue, 0, 3);
            ledStrip.show();
        }
        else
        {
            ledStrip.clear();
            ledStrip.show();
        }
    }
}

void Leds::initializing(int interval, unsigned long time)
{
    uint32_t colorValue = getColor("magenta");

    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        if (ledIsOn == 0)
        {
            for (int i = 0; i < ledStrip.numPixels() + 1; i++)
            {
                ledStrip.fill(colorValue, 0, i);
                ledStrip.show();
                ledStrip.clear();
            }
        }
    }
}