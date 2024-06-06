/// @addtogroup Arduino
/// @{
#ifndef smart_motors_h
#define smart_motors_h

#include <Arduino.h>
#include <PID_v2.h>

#include "../Communication/Communication.h"
#include "Motors.h"
#include "AxelGyroSensor.h"


#define KP 4
#define KI 4
#define KD 0.5

/**
 * Allowed error for the angle when turning.
 * No move will occur if the angle lies in the allowed error range.
 */
#define ANGLE_ERROR_ALLOWED 0.05

/** Speed of the motors when turning */
#define TURNING_SPEED 128

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
         * Note that the turning process occurs in the `update()` method.
         * @param angle The angle that the robot should aim.
         */
        void turnRight(float angle);

        /**
         * Makes the robots turning left until something else stops it.
         * The "turning left" is from the point of view of "going forward"
         * "going forward" is going away from the dev cable port.
         * Note that the turning process occurs in the `update()` method.
         * @param angle The angle that the robot should aim.
         */
        void turnLeft(float angle);

        /**
         * Makes the robots going forward until something else stops it.
         * "going forward" is going away from the dev cable port.
         * @param speed Speed at which one should move. Default is 255.
         */
        void goForward(int speed = 255);

        /**
         * Makes the robots going backward until something else stops it.
         * "going backward" is going in the direction of the dev cable port.
         * @param speed Speed at which one should move. Default is 255.
         */
        void goBackward(int speed = 255);

        /**
         * Tells what the motors were told to do.
         * This allows to have some sort of "1-frame" history 
         */
        bool toldToForward=false, toldToBackward=false, toldToRight=false, toldToLeft=false;

        /**
         * Tells whether the motors are commanded to be stopped or not 
         */
        inline bool stopped() {return !toldToForward && !toldToBackward && !toldToRight && !toldToLeft;};

    private:
        /** Motors instance to activate the motors. */
        Motors motors = Motors();

        /** Controls the accel/gyro data to adapt movements. */
        AxelGyroSensor axelgyro = AxelGyroSensor();

        /** PID object to deal with control engineering for the translation movement. */
        PID_v2 pid = PID_v2(KP, KI, KD, DIRECT);

        /**
         * Resets the SetPoint for the PID.
         * This can't be done easily through `pid.SetPoint()`.
         * Instead, some weird code happens inside this function to
         * make it work properly.
         * It resets the internal sums of the PID object, then sets the setPoint.
         */
        void pidSetpoint(double setpoint);

        /**
         * Aimed angle persistent through multiple loop iterations when required to turn.
         * Default is `-1` and indicate that no angle is aimed (maybe the robot isn't turning).
         */
        float aimedAngle = -1;

        /** Comes with a radio module to output its things */
        Communication<CommunicationModule::radio> radio = Communication<CommunicationModule::radio>();

};

#endif

/// @}