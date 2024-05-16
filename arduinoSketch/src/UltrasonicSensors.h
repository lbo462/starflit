/* UltrasonicSensors are compose of two sensors and hence, two pins
 * one for the front side and the other for the rear size.
 */

#ifndef ultrasonic_h
#define ultrasonic_h

#include <Arduino.h>


class UltrasonicSensors {
    public:
        UltrasonicSensors();
        ~UltrasonicSensors();

        /* Sensor pins
         * 
         */
        const int echo=2, trig1=7, trig2=8;

        /* Set-up the pins mode
         * To be called on global setup
         * Note that pins can be modified before setup if needed
         */ 
        void setup();

        // Get the distance with any front object
        unsigned int getFrontDistance();

        // Get the distance with any rear object
        unsigned int getRearDistance();

        /* Check that no collision is to occur
         * @return true if an object if too close from front or rear, else false
         */
        bool collisionDetection(bool goingForward, bool goingBackward);

    private:
        // Get the distance in centimeter for a given pin (ie, given sensor)
        unsigned int getDistance(int trigPin);
};

#endif