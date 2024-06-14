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
         * Makes the LED strip on top of the robot blink a certain color at a certain speed.
         */
        void blink(char* color, int interval);

        /**
         * Makes the LED strip on top of the robot light up a certain color.
         */
        void still(char* color);

        /**
         * Blinks only the forward half of the strip
         */
        void forwardBlink(char* color, int interval);

        /**
         * Blinks only the backward half of the strip
         */
        void backwardBlink(char* color, int interval);

        /**
         * TODO
         */
        void batteryVoltage();

        void initializing();

        /**
         * Given the color name, returns it's `uint32_t` representation.
         * Call this method to access the read only color definitions.
         */
        inline uint32_t getColor(const char* color) 
        {
            for (int i; sizeof(colors) / sizeof(colors[0]); ++i)
            {
                if (strcmp(colors[i].color, color) == 0)
                    return colors[i].colorValue;
            }
        }

    private:

        /** 
         * Params to control the LEDs.
         */
        const int ledPin=2, numLeds=33, brightness=50;

        Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

        struct Colors {
            const char* color;
            uint32_t colorValue;
        };

        /**
         * Key value pair of all the colors associated with their `uint32_t` representation.
         */
        Colors colors[4] = {
            {"red", ledStrip.Color(255, 0, 0)},
            {"green", ledStrip.Color(0, 255, 0)},
            {"blue", ledStrip.Color(0, 0, 255)},
            {"magenta", ledStrip.Color(255, 0, 255)},
        };

};

#endif
/// @}