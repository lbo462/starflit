#include "Leds.h"

Leds::Leds() {}

Leds::~Leds() {}

void Leds::setup()
{
    ledStrip.begin();
    ledStrip.setBrightness(brightness);
    ledStrip.show();
}

void Leds::blink(char* color, int interval)
{
    uint32_t colorValue = getColor(color);
    while (1)
{
    ledStrip.fill(colorValue);
    ledStrip.show();
    delay(interval);
    ledStrip.clear();
    ledStrip.show();
    delay(interval);
}

}

void Leds:: still(char* color)
{
    uint32_t colorValue = getColor(color);

    ledStrip.fill(colorValue);
    ledStrip.show();

}

void Leds::forwardBlink(char* color, int interval)
{
    uint32_t colorValue = getColor(color);

    ledStrip.fill(colorValue, 3, 17);
    ledStrip.show();
    delay(interval);
    ledStrip.clear();
    ledStrip.show();
    delay(interval);
}

void Leds::backwardBlink(char* color, int interval)
{
    uint32_t colorValue = getColor(color);

    ledStrip.fill(colorValue, 20, 13);
    // Needed because we reach the end of the strip with the line above so we need
    // to start over from 0 to complete the half circle.
    ledStrip.fill(colorValue, 0, 3);
    ledStrip.show();
    delay(interval);
    ledStrip.clear();
    ledStrip.show();
    delay(interval);
}