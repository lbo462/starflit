#ifndef rescue_bot_h
#define rescue_bot_h

#include <Arduino.h>
#include <Wire.h>

#include "SmartMotors/SmartMotors.h"
#include "UltrasonicSensors.h"
#include "Radio.h"

/** TODO */
#define SCAN_INTERVAL 3000


/**
 * Main class that holds the logic and glue every modules to control the robot efficiently.
 * One should call `setup()` once and `update()` at each frame
 */
class RescueBot 
{
    public:
        RescueBot();
        ~RescueBot();

        /** Smart motors instance to make the robot move in a smart way */
        SmartMotors smartMotors = SmartMotors();

        /** Ultrasonic sensors used to detect incoming collisions */
        UltrasonicSensors ultrasonicSensors = UltrasonicSensors();

        /** Plugged some RF24 antennas and use the radio class instance to send messages to space! */
        Radio radio = Radio();

        /**
         * Set-up the pins mode.
         * To be called on global setup.
         */ 
        void setup();

        /**
         * Updates the robot to make it move.
         * To be called at each frame.
         */
        void update();

        /** Scan the environnement to see if there are obstacles out of its detection cone */
        void scan();

        /** TODO */
        inline bool isScanning() { return scanning; }

        /** Avoid any collision by making some wiggy-jiggy moves */
        void collisionAvoidance();

        /**
         * Make random moves to explore the space.
         * Aware of collisions and won't break walls !
         */
        void explore();

    private:
        /** TODO */
        bool scanning = false;

        /** Previous scan timing in milliseconds */
        unsigned long previousScan = 0;

        /**
         * Get rewritten during the scan and tells if
         * the robot did scan on right or left. 
         */
        bool scannedRight, scannedLeft;
};

#endif