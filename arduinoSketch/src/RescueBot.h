#ifndef rescue_bot_h
#define rescue_bot_h

#include <Arduino.h>
#include <Wire.h>
#include "UltrasonicSensors.h"


class RescueBot {
    public:
        RescueBot();
        ~RescueBot();

        /* Robots PINS, depends on the ways it's wired.
         * Written in three letters : m[x][y]
         * [x] can be r = right or l = left ;
         * [y] can be p = plus-pin or m = minus-pin
         */
        const int mrp=10, mrm=9, mlp=6, mlm=5;

        // Add some ultrasonic sensors
        UltrasonicSensors ultrasonicSensors = UltrasonicSensors();

        /* Set-up the pins mode
         * To be called on global setup
         * Note that pins can be modified before setup if needed
         */ 
        void setup();

        /**
         * Updates the robot to make it move.
         * To be called at each frame.
         */
        void update();

        // Reading of the ro private attributes for the robot state
        bool isGoingForward() { return isGoingForward_; };
        bool isGoingBackward() { return isGoingBackward_; };
        bool isTurningRight() { return isTurningRight_; };
        bool isTurningLeft() { return isTurningLeft_; };

        // Stop all the motors
        void stop();

        /* Sets a speed for all of the motors
        * Requires Wire.begin()
        * Works by writing 
        * As weird as it sounds, the higher the value is, 
        * the lower the motors will run :/
        */
        void setSpeed(int speed);

        /* Makes the robots turning right until something else stops it
        * The "turning right" is from the point of view of "going forward"
        * "going forward" is going away from the dev cable port.
        */
        void turnRight();

        /* Makes the robots turning left until something else stops it
        * The "turning left" is from the point of view of "going forward"
        * "going forward" is going away from the dev cable port.
        */
        void turnLeft();

        /* Makes the robots going foward until something else stops it
        * "going forward" is going away from the dev cable port.
        */
        void goForward();

        /* Makes the robots going backward until something else stops it
        * "going backward" is going in the direction of the dev cable port.
        */
        void goBackward();

        // Moves forward for one second in a random direction before stopping
        void randomDirection();

    private:
        // State of the robot as read-only
        bool isGoingForward_, isGoingBackward_, isTurningRight_, isTurningLeft_;
};

#endif