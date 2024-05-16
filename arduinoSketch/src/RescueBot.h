#ifndef rescue_bot_h
#define rescue_bot_h

#include <Arduino.h>
#include <Wire.h>
#include "UltrasonicSensors.h"


class RescueBot {
    public:
        RescueBot();
        ~RescueBot();

        /**
         * Robots PINS, depends on the ways it's wired.
         * Written in three letters : m[x][y]
         * [x] can be r = right or l = left ;
         * [y] can be p = plus-pin or m = minus-pin
         */
        const int mrp=10, mrm=9, mlp=6, mlm=5;

        /**
         * Ultrasonic sensors so that the bot is able to detect collisions
         */
        UltrasonicSensors ultrasonicSensors = UltrasonicSensors();

        /**
         * Set-up the pins mode.
         * To be called on global setup.
         * Note that pins can be modified before setup if needed
         */ 
        void setup();

        /**
         * Updates the robot to make it move.
         * To be called at each frame.
         */
        void update();

        /**
         * @return true if the robot was commanded to go forward
         */
        bool isGoingForward() { return isGoingForward_; };

        /**
         * @return true if the robot was commanded to go backward
         */
        bool isGoingBackward() { return isGoingBackward_; };

        /**
         * @return true if the robot was commanded to turn to his right
         */
        bool isTurningRight() { return isTurningRight_; };

        /**
         * @return true if the robot was commanded to turn to his left
         */
        bool isTurningLeft() { return isTurningLeft_; };

        /**
         * Stop all the motors
         */
        void stop();

        /**
         * Sets a speed for all of the motors.
         * Requires that Wire.begin() was called 
         * As weird as it sounds, the higher the value is, 
         * the lower the motors will run :/
         */
        void setSpeed(int speed);

        /**
         * Makes the robots turning right until something else stops it.
         * The "turning right" is from the point of view of "going forward"
         * "going forward" is going away from the dev cable port.
         * @return true if the action could be done, false otherwise
         */
        bool turnRight();

        /**
         * Makes the robots turning left until something else stops it.
         * The "turning left" is from the point of view of "going forward"
         * "going forward" is going away from the dev cable port.
         * @return true if the action could be done, false otherwise
         */
        bool turnLeft();

        /**
         * Makes the robots going foward until something else stops it.
         * "going forward" is going away from the dev cable port.
         * @return true if the action could be done, false otherwise
         */
        bool goForward();

        /**
         * Makes the robots going backward until something else stops it.
         * "going backward" is going in the direction of the dev cable port..
         * @return true if the action could be done, false otherwise
         */
        bool goBackward();

        /**
         * Make the robot turn in a random direction (right or left)
         * for one second before stopping.
         * @return true if the action could be done, false otherwise
         */
        bool setRandomDirection();

        /**
         * Make random moves to explore the space.
         * Aware of collisions and won't break walls !
         */
        void explore();

        /**
         * Avoid the collisions and try to set a new direction 
         * for the RescueBot
         */
        void collisionAvoidance();

    private:
        /**
         * State of the robot as read-only
         */
        bool isGoingForward_, isGoingBackward_, isTurningRight_, isTurningLeft_;


        /**
         * Basic movements functions
         * Theses are private since we don't want to call them directly
         * Instead, we use the public functions that implements features 
         * such as collision detection.
        */
        void goForward_();
        void goBackward_();
        void turnRight_();
        void turnLeft_();


        /*
        * The value that delays the movements of the motors
        */
        int movementDelay;

        // The basic functions that set and decrease the movementDelay
        void decreaseMovementDelay();
        void setMovementDelay(int value);

        /* 
        * Tells, depending on the movementDelay if the movements
        * are available
        */ 
        bool isMovementAvailable();

};

#endif