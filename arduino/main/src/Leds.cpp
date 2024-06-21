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
    // Gets the `uint32_t` color value based on the color provided as a parameter. 
    uint32_t colorValue = getColor(color);

    // Checking if the the time provided in the `interval` parameter has passed.
    // This avoids using delays and thus blocking the code.
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

void Leds::still(char* color)
{
    // Gets the `uint32_t` color value based on the color provided as a parameter.
    uint32_t colorValue = getColor(color);

    ledStrip.fill(colorValue);
    ledStrip.show();

}

void Leds::forwardBlink(char* color, int interval, unsigned long time)
{
    // Gets the `uint32_t` color value based on the color provided as a parameter.
    uint32_t colorValue = getColor(color);

    // Checking if the the time provided in the `interval` parameter has passed.
    // This avoids using delays and thus blocking the code.
    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        if (ledIsOn == 0)
        {
            // Only lights up 17 LEDs starting from the 3rd one.
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
    // Gets the `uint32_t` color value based on the color provided as a parameter.
    uint32_t colorValue = getColor(color);

    // Checking if the the time provided in the `interval` parameter has passed.
    // This avoids using delays and thus blocking the code.
    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        if (ledIsOn == 0)
        {
            // Only lights up 13 LEDs starting from the 20th.
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
    // Gets the `uint32_t` color value based on the color provided as a parameter.
    uint32_t colorValue = getColor("magenta");

    // Checking if the the time provided in the `interval` parameter has passed.
    // This avoids using delays and thus blocking the code.
    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        if (ledIsOn == 0)
        {
            // Lights up LEDs incrementally 
            for (int i = 0; i < ledStrip.numPixels() + 1; i++)
            {
                ledStrip.fill(colorValue, 0, i);
                ledStrip.show();
                ledStrip.clear();
                // TEST: 
                // ledStrip.fill(colorValue, i, 1);
                // ledStrip.show();

                // TODO: light up speed depends on interval
            }
        }
    }
}