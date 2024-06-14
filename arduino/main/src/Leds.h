/// @addtogroup Arduino
/// @{
#ifndef Leds_h
#define Leds_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class Leds
{
    public:
        Leds();
        ~Leds();


        /**
         * Does the basic setup of the LED strip.
         */
        void setup();

        /**
         * Makes the LED strip on top of the robot blink a certain color.
         */
        void blink(uint32_t color, int interval);

        /**
         * Makes the LED strip on top of the robot light up a certain color.
         */
        void still(uint32_t color);

    private:

        Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

        /** 
         * Params to control the LEDs.
         */
        const int ledPin=2, numLeds=33, brightness=50;

};

#endif
/// @}