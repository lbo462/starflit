#ifndef ultrasonic_h
#define ultrasonic_h

#include <Arduino.h>


/**
 * These sensors are composed of two sensors that are used to detect collisions.
 * One is used for the front side and the other for the rear size.
 * One should call `setup()` once.
 */
class UltrasonicSensors
{
    public:
        UltrasonicSensors();
        ~UltrasonicSensors();

        /**
         * Set-up the pins mode
         * To be called on global setup
         * Note that pins can be modified before setup if needed
         */ 
        void setup();

        /** Get the distance with any front object */
        unsigned int getFrontDistance();

        /** Get the distance with any rear object */
        unsigned int getRearDistance();

        /** 
         * Check that no collision is to occur
         * @param fromFront Check front collision 
         * @param fromRear Check rear collision
         * @param distance Distance in cm for which the collision will be detected, default is 30 cm
         * which correspond to the robot width
         * @return true if an object if too close from front or rear, else false
         */
        bool collisionDetection(bool fromFront, bool fromRear, unsigned int distance = 30);

    private:
        /** Sensor pins */
        const int echo=2, trig1=7, trig2=8;

        /** Get the distance in centimeter for a given pin (ie, given sensor) */
        unsigned int getDistance(int trigPin);
};

#endif