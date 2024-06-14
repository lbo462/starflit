#include "Leds.h"

Leds::Leds() {}

Leds::~Leds() {}

void Leds::setup()
{
    ledStrip.begin();
}

void Leds::blink(uint32_t color, int interval)
{
    while (1)
{
    ledStrip.setBrightness(brightness);
    ledStrip.show();
    uint32_t red = ledStrip.Color(0, 255 ,0);
    ledStrip.fill(red);
    ledStrip.show();
    delay(interval);
    ledStrip.setBrightness(0);
    ledStrip.show();
    delay(interval);
}

}

void Leds:: still(uint32_t color)
{
    ledStrip.setBrightness(brightness);
    ledStrip.show();
    uint32_t red = ledStrip.Color(0, 255 ,0);
    ledStrip.fill(red);
    ledStrip.show();

}