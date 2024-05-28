#ifndef motors_h
#define motors_h

#include <Arduino.h>


/**
 * Defines the minimum voltage that the motors need to really turn.
 * This depends on the mechanical friction on our system.
 * If the voltage applied is less than this value, the command won't be sent.
 * Even if it was sent, the motors wouldn't be able to move and would make a sad noise :'(
 */
#define MIN_POWER 100


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
         * Note that a negative voltage will make the motors turn in the opposite director of `forward`.
         * @return true the voltage was big enough so that the command truly made the motors turn.
         * Returns false if the required voltage was too low.
         */
        bool turnRightWheel(bool forward, int voltage); bool turnLeftWheel(bool forward, int voltage);

        /** Stop all motors */
        void stop();

    private:
        /**
         * Robots PINS, depends on the ways it's wired.
         * Written that way : motor[X][Y]
         * [X] can be R = right or L = left ;
         * [Y] can be P = plus-pin or M = minus-pin
         */
        const int motorRP=6, motorRM=5, motorLP=10, motorLM=9;
};

#endif