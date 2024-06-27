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

        if (!ledIsOn)
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

        if (!ledIsOn)
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

        if (!ledIsOn)
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

    uint32_t blue = ledStrip.Color(43, 22, 190);
    uint32_t red = ledStrip.Color(255, 0, 0);

    // Lights up LEDs incrementally 
    for (int i = 0; i < ledStrip.numPixels() / 2 + 1; i++)
    {
        ledStrip.fill(blue, i, 4);
        ledStrip.fill(red, i + ledStrip.numPixels() / 2, 4);
        ledStrip.show();
        delay(interval);
        ledStrip.clear();
        ledStrip.show();
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

    uint16_t startHue = 43690;
    uint16_t endHue = 0;
    uint8_t saturation = 255;
    uint8_t value = 255;
    
    long hue = startHue;

    int i = 0;

    while (i < (ledStrip.numPixels() + 1))
    {
        if (hue > 65536)
            hue = 0;
        else if (i < (ledStrip.numPixels() / 2) - 2)
            hue = startHue;
        else if (i > (ledStrip.numPixels() / 2) + 2)
            hue = endHue;
        else
            hue += 1024;

        uint32_t color = ledStrip.ColorHSV(hue, saturation, value);
    
        // Set all pixels to the new color
        ledStrip.setPixelColor(i, color);
        ledStrip.show();
        delay(50);
        i++;
    }
}

void Leds::batteryVoltage()
{
    ledStrip.clear();

    uint16_t startHue = 43690;
    uint16_t endHue = 0;
    
    long hue = (startHue - endHue) / 2;

    float batteryPercent = analogRead(batteryPin) / 1023.0;
    int ledsToLight = (ledStrip.numPixels()+1) * batteryPercent;

    for (int i = 0; i < ledsToLight; i++)
    {
        uint32_t color = ledStrip.ColorHSV(hue, 255, 255);

        ledStrip.clear();
        ledStrip.setPixelColor(i, color);
        ledStrip.show();
        delay(log(exp(2*(ledsToLight - i))));
    }

    for (int i = ledsToLight+1; i >= 0; i--)
    {
        if (i < ledsToLight / 2 + 2)
            hue = endHue;
        else if (i > ledsToLight / 2 - 2)
            hue = startHue;

        uint32_t color = ledStrip.ColorHSV(hue, 255, 255);

        ledStrip.setPixelColor(i, color);
        ledStrip.show();
        delay(2*i);
    }
}