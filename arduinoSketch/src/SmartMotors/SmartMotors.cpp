#include "SmartMotors.h"


SmartMotors::SmartMotors() {}
SmartMotors::~SmartMotors() {}

void SmartMotors::setup()
{
    motors.setup();
    axelgyro.setup();

    pid.SetMode(AUTOMATIC);
    pid.SetOutputLimits(-255, 255);
    pid.Start(axelgyro.angle.z, 0, axelgyro.angle.z);
}

void SmartMotors::update()
{
    axelgyro.update();
}

void SmartMotors::pidSetpoint(double setpoint)
{
    /**
     * This weird code resets the internal sums
     * of the PID object.
     * If not done, the setPoint won't work properly.
     */
    pid.SetTunings(KP, 0, 0);
    pid.Run(axelgyro.angle.z);
    pid.SetTunings(KP, KI, KD);

    /** Finally set the setpoint */
    pid.Setpoint(setpoint);
}

void SmartMotors::stop()
{
    // Update the command variables
    toldToForward = false;
    toldToBackward = false;
    toldToRight = false;
    toldToLeft = false;

    motors.stop();
}

void SmartMotors::goForward(int speed) 
{
    if(!toldToForward)
    {
        /**
         * Defines the setPoint for the "forward" direction.
         * This is done only when the smartMotors where first told to go forward.
         * The setPoint do not change until a new order arrives.
         */
        pidSetpoint(axelgyro.preAngle.z);
    }

    // Get clamped output between 0 and `speed` from the PID module
    const float output = min(pid.Run(axelgyro.preAngle.z), speed);

    // Update the command variables
    toldToForward = true;
    toldToBackward = false;
    toldToRight = false;
    toldToLeft = false;
    
    Serial.print(axelgyro.preAngle.z);
    Serial.print(" ");
    Serial.print((int)(speed - output));
    Serial.print(" ");
    Serial.print((int)(speed + output));
    Serial.println();

    motors.turnRightWheel(true, (int)(speed - output));
    motors.turnLeftWheel(true, (int)(speed + output));
}

void SmartMotors::goBackward(int speed)
{
    if(!toldToBackward)
    {
        /**
         * Defines the setPoint for the "backward" direction.
         * This is done only when the smartMotors where first told to go forward.
         * The setPoint do not change until a new order arrives.
         */
        pidSetpoint(axelgyro.preAngle.z);
    }

    // Get clamped output between 0 and `speed` from the PID module
    const float output = min(pid.Run(axelgyro.preAngle.z), speed);

    // Update the command variables
    toldToForward = false;
    toldToBackward = true;
    toldToRight = false;
    toldToLeft = false;

    motors.turnRightWheel(false, (int)(speed + output));
    motors.turnLeftWheel(false, (int)(speed - output));
}

void SmartMotors::turnRight(float angle, int speed, float allowedError)
{
    if(abs(axelgyro.angle.z - angle) < abs(allowedError))
    {
        /** Stop the motors and ignore the command if angle is the allowed error range */
        motors.stop();
        return;
    }

    // Update the command variables
    toldToForward = false;
    toldToBackward = false;
    toldToRight = true;
    toldToLeft = false;

    // Do turn
    motors.turnRightWheel(false, speed);
    motors.turnLeftWheel(true, speed);
}

void SmartMotors::turnLeft(float angle, int speed, float allowedError)
{
    if(abs(axelgyro.angle.z - angle) < abs(allowedError))
    {
        /** Stop the motors and ignore the command if angle is the allowed error range */
        motors.stop();
        return;
    }

    // Update the command variables
    toldToForward = false;
    toldToBackward = false;
    toldToRight = false;
    toldToLeft = true;

    // Do turn
    motors.turnRightWheel(true, speed);
    motors.turnLeftWheel(false, speed);
}