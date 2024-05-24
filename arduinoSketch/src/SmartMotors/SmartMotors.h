#ifndef smart_motors_h
#define smart_motors

#define KP 4
#define KI 4
#define KD 0.5

#include <Arduino.h>
#include <PID_v2.h>

#include "Motors.h"
#include "AxelGyroSensor.h"


/**
 * Smart motors is abstract representation of motors monitored via control engineer.
 * Instances of this class have a motor and an axelgyro module.
 * The `motor` module makes the motors move by applying voltage ;
 * The `axelgyro` module look out for any drifting behaviour and adapt the motor commands.
 */
class SmartMotors
{
    public:
        SmartMotors();
        ~SmartMotors();

        /**
         * Setup function, to be called when setting all up,
         * required for the instance methods to work properly. 
         */
        void setup();

        /** This method should be called at each frame iteration to update sub-modules */
        void update();

        /** Stop every motors */
        void stop();

        /**
         * Makes the robots turning right until something else stops it.
         * The "turning right" is from the point of view of "going forward"
         * "going forward" is going away from the dev cable port.
         */
        void turnRight(int speed);

        /**
         * Makes the robots turning left until something else stops it.
         * The "turning left" is from the point of view of "going forward"
         * "going forward" is going away from the dev cable port.
         */
        void turnLeft(int speed);

        /**
         * Makes the robots going forward until something else stops it.
         * "going forward" is going away from the dev cable port.
         */
        void goForward(int speed);

        /**
         * Makes the robots going backward until something else stops it.
         * "going backward" is going in the direction of the dev cable port.
         */
        void goBackward(int speed);

    private:
        /** Motors instance to activate the motors */
        Motors motors = Motors();

        /** Controls the accel/gyro data to adapt movements */
        AxelGyroSensor axelgyro = AxelGyroSensor();

        /**
         * Get a new PID instance for our needs.
         * We need to create a new instance because
         * I couldn't find a way to reset the existent one.
         * Maybe the PID library suck, maybe I suck ... 
         */
        PID_v2 getPID();

        /**
         * Tells what the motors were told to do.
         * This allows to have some sort of "1-frame" history 
         */
        bool toldToForward=false, toldToBackward=false, toldToRight=false, toldToLeft=false;

        /** 
         * PID object to deal with control engineering for the translation movement;
         * This object will be rewritten multiple time // TODO
         */
        PID_v2 pid;

};

#endif