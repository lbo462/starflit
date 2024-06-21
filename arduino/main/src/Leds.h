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
        void blink(char* color, int interval, unsigned long time);

        /**
         * Makes the LED strip on top of the robot light up a certain color.
         */
        void still(char* color);

        /**
         * Blinks only the forward half of the strip.
         */
        void forwardBlink(char* color, int interval, unsigned long time);

        /**
         * Blinks only the backward half of the strip.
         */
        void backwardBlink(char* color, int interval, unsigned long time);

        /**
         * Displays the battery voltage status of the robot (NOT IMPLEMENTED YET).
         */
        void batteryVoltage();
        
        /**
         * Initializing sequence that plays while the robot is waiting for the RPi to initialize.
         */
        void initializing(int interval, unsigned long time);

        /**
         * Given the color name as a string, returns it's `uint32_t` representation.
         * This method is used so that a simple string can be provided as a parameter
         * 
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
         * Params to control the LED strip.
         */
        const int ledPin=2, numLeds=33, brightness=50;

        /**
         * State of the LED strip used for the blinking effect.
         */
        int ledIsOn = 0;

        /**
         * Keeps track of the last 
         */
        unsigned long previousLedMillis = 0;

        /**
         * ledStrip object initialization.
         */
        Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

        /**
         * Structure for the Colors data type which associates a color name with its `uint32_t` representation.
         */
        struct Colors {
            const char* color;
            uint32_t colorValue;
        };

        /**
         * Key value pairs of all the colors you can use associated with their `uint32_t` representation.
         * Feel free to add additional colors based on your needs.
         */
        Colors colors[4] = {
            {"red", ledStrip.Color(255, 0, 0)},
            {"green", ledStrip.Color(0, 255, 0)},
            {"blue", ledStrip.Color(0, 0, 255)},
            {"magenta", ledStrip.Color(255, 0, 255)}
        };
};

#endif
/// @}