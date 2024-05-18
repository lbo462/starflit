#ifndef rescue_bot_h
#define rescue_bot_h

#include <Arduino.h>
#include <Wire.h>
#include "UltrasonicSensors.h"
#include "AxelGyroSensor.h"
#include "Radio.h"


/**
 * Main class that holds the logic and glue every modules to control the robot efficiently
 * One should call `setup()` once and `update()` at each frame
 */
class RescueBot 
{
    public:
        RescueBot();
        ~RescueBot();

        /**
         * Ultrasonic sensors so that the bot is able to detect collisions
         */
        UltrasonicSensors ultrasonicSensors = UltrasonicSensors();

        /**
         * Add an axelgyro sensor to our beloved bot 
         */
        AxelGyroSensor axelgyro = AxelGyroSensor();

        /**
         * Plugged some RF24 antennas and use the radio class instance to send messages to space!  
         */
        Radio radio = Radio();

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
         * Scan the environnement to see if there are obstacles out of its detection cone
         */
        void scan();

        /**
         * Avoid any collision by making some wiggy-jiggy moves 
         */
        void collisionAvoidance();

        /**
         * Make random moves to explore the space.
         * Aware of collisions and won't break walls !
         */
        void explore();

    private:
        /**
         * Robots PINS, depends on the ways it's wired.
         * Written in three letters : m[x][y]
         * [x] can be r = right or l = left ;
         * [y] can be p = plus-pin or m = minus-pin
         */
        const int mrp=10, mrm=9, mlp=6, mlm=5;

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
};

#endif