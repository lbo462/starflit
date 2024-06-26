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
    ledStrip.clear();
    // Gets the `uint32_t` color value based on the color provided as a parameter. 
    uint32_t colorValue = getColor(color);

    // Checking if the the time provided in the `interval` parameter has passed.
    // This avoids using delays and thus blocking the code.
    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        if (ledIsOn)
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
    ledStrip.clear();
    // Gets the `uint32_t` color value based on the color provided as a parameter.
    uint32_t colorValue = getColor(color);

    ledStrip.fill(colorValue);
    ledStrip.show();

}

void Leds::forwardBlink(char* color, int interval, unsigned long time)
{
    ledStrip.clear();
    // Gets the `uint32_t` color value based on the color provided as a parameter.
    uint32_t colorValue = getColor(color);

    // Checking if the the time provided in the `interval` parameter has passed.
    // This avoids using delays and thus blocking the code.
    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        if (ledIsOn)
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
    ledStrip.clear();
    // Gets the `uint32_t` color value based on the color provided as a parameter.
    uint32_t colorValue = getColor(color);

    // Checking if the the time provided in the `interval` parameter has passed.
    // This avoids using delays and thus blocking the code.
    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        if (ledIsOn)
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

void Leds::initializing(int interval)
{
    ledStrip.clear();
    // Gets the `uint32_t` color value based on the color provided as a parameter.
    uint32_t colorValue = getColor("magenta");

    if (ledIsOn)
    {
        // Lights up LEDs incrementally 
        for (int i = 0; i < ledStrip.numPixels() + 1; i++)
        {
            ledStrip.fill(colorValue, i, 1);
            ledStrip.show();
            delay(250);
        }
    }
}

void Leds::rainbow(int interval, unsigned long time)
{
    ledStrip.clear();
    if (time - previousLedMillis >= interval)
    {
        previousLedMillis = time;

        // Increments firstPixelHueState and reset if necessary
        firstPixelHueState += 256;
        if (firstPixelHueState >= 5 * 65536)
        {
            firstPixelHueState = 0;
        }

        // Updates the colors of the pixels
        for (int i = 0; i < ledStrip.numPixels(); i++)
        { 
            int pixelHue = firstPixelHueState + (i * 65536L / ledStrip.numPixels());
            ledStrip.setPixelColor(i, ledStrip.gamma32(ledStrip.ColorHSV(pixelHue)));
        }
        ledStrip.show();
    }
}

void Leds::starflitRedToBlue()
{
    ledStrip.clear();
    /*
    long startH = 63488;
    int startS = 225;
    int startV = 192;

    int endH = 0;
    int endS = 100;
    int endV = 100;

    int nbLeds = ledStrip.numPixels();

    for (int i = 0; i < nbLeds / 2 + 1; i++)
    {
        //ledStrip.setPixelColor(i, ledStrip.ColorHSV(startH -  i *(startH / nbLeds), startS -  i *(startS / nbLeds), startV -  i *(startV / nbLeds)));
        //ledStrip.setPixelColor(nbLeds - i, ledStrip.ColorHSV(startH -  i *(startH / nbLeds), startS -  i *(startS / nbLeds), startV -  i *(startV / nbLeds)));
        ledStrip.setPixelColor(i, ledStrip.gamma32(ledStrip.ColorHSV(startH, startS, startV)));
        ledStrip.setPixelColor(nbLeds - i, ledStrip.gamma32(ledStrip.ColorHSV(startH, startS, startV)));
        ledStrip.show();
        delay(200);

    */

    for (int i = 0; i < ledStrip.numPixels() / 2 + 1; i++)
    {
        float progress = i / ledStrip.numPixels();
        float factor = pow(progress, 0.5);
        uint32_t color = ledStrip.Color(43 + factor * (255 - 43),
                                        22 + factor * (0 - 22),
                                        190 + factor * (0 -190));
        ledStrip.setPixelColor(i, color);
        ledStrip.setPixelColor(ledStrip.numPixels() - i, color);
        ledStrip.show();
        delay(200);
    }
}