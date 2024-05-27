#ifndef motors_h
#define motors_h

#include <Arduino.h>


/**
 * This class is aimed to control motors, nothing more.
 * Remember to call its `setup()` method once.
 */
class Motors
{
    public:
        Motors();
        ~Motors();

        /**
         * Set-up the pins mode.
         * To be called on global setup.
         * Note that pins can be modified before setup if needed
         */ 
        void setup();

        /**
         * Makes the motor turn.
         * @param forward Tells whether the motors will turn forward (true) or backward (false)
         * @param voltage Voltage to apply on the motor, that will define its speed.
         * Note that a negative voltage will just be set to 0.
         */
        void turnRightWheel(bool forward, int voltage); void turnLeftWheel(bool forward, int voltage);

        /** Stop all motors */
        void stop();

    private:
        /**
         * Robots PINS, depends on the ways it's wired.
         * Written that way : motor[X][Y]
         * [X] can be R = right or L = left ;
         * [Y] can be P = plus-pin or M = minus-pin
         */
        const int motorRP=10, motorRM=9, motorLP=6, motorLM=5;
};

#endif