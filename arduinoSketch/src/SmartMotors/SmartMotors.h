#ifndef smart_motors_h
#define smart_motors

#include <Arduino.h>
#include "Motors.h"
#include "UltrasonicSensors.h"
#include "AxelGyroSensor.h"


/**
 * Smart motors is abstract representation of motors monitored via control engineer
 * Instances of this class have a motor and an axelgyro module.
 * The `motor` module makes the motors move by applying voltage ;
 * The `axelgyro` module look out for any drifting behaviour and adapt the motor commands.
 */
class SmartMotors
{
    public:
        SmartMotors();
        ~SmartMotors();

        // Motors instance to activate the motors
        Motors motors = Motors();

        // Ultrasonic sensors used to detect incoming collisions
        UltrasonicSensors ultrasonicSensors = UltrasonicSensors();

        // Controls the accel/gyro data to adapt movements
        AxelGyroSensor axelgyro = AxelGyroSensor();

        /**
         * Setup function, to be called when setting all up,
         * required for the instance methods to work properly. 
         */
        void setup();

        /**
         * This method should be called at each frame iteration to update sub-modules
         */
        void update();

        /**
         * Makes the robots turning right until something else stops it.
         * The "turning right" is from the point of view of "going forward"
         * "going forward" is going away from the dev cable port.
         * @return true if the action could be done, false otherwise
         */
        bool turnRight(int speed);

        /**
         * Makes the robots turning left until something else stops it.
         * The "turning left" is from the point of view of "going forward"
         * "going forward" is going away from the dev cable port.
         * @return true if the action could be done, false otherwise
         */
        bool turnLeft(int speed);

        /**
         * Makes the robots going foward until something else stops it.
         * "going forward" is going away from the dev cable port.
         * @return true if the action could be done, false otherwise
         */
        bool goForward(int speed);

        /**
         * Makes the robots going backward until something else stops it.
         * "going backward" is going in the direction of the dev cable port..
         * @return true if the action could be done, false otherwise
         */
        bool goBackward(int speed);

    private:
};

#endif