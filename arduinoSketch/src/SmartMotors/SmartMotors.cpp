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

    stop();
}

void SmartMotors::update()
{
    // Update angle and position
    axelgyro.update();

    // Now we do the moves we're required to do, such as turning
    if(toldToRight || toldToLeft)
    {
        // Stop the motors when the current angle matches the aimed one. 
        if(abs(axelgyro.preAngle.z - aimedAngle) < abs(ANGLE_ERROR_ALLOWED))
        {
            // This also resets the variables `toldToRight` and `toldToLeft`
            stop();
            aimedAngle = -1;
        }

        // Otherwise, turn right if told so ...
        else if(toldToRight)
        {
            if(axelgyro.preAngle.z > aimedAngle)
            {
                motors.turnRightWheel(false, TURNING_SPEED);
                motors.turnLeftWheel(true, TURNING_SPEED);
            }
            else
            {
                // Turn back left if we went too far
                motors.turnRightWheel(true, TURNING_SPEED);
                motors.turnLeftWheel(false, TURNING_SPEED);
            }
        }

        // ... or turn left if told so.
        else if(toldToLeft)
        {
            if(axelgyro.preAngle.z < aimedAngle)
            {
                motors.turnRightWheel(true, TURNING_SPEED);
                motors.turnLeftWheel(false, TURNING_SPEED);
            }
            else
            {
                // Turn back right if we went too far
                motors.turnRightWheel(false, TURNING_SPEED);
                motors.turnLeftWheel(true, TURNING_SPEED);
            }
        }
    }
}

void SmartMotors::pidSetpoint(double setpoint)
{
    /*
     * This weird code resets the internal sums
     * of the PID object.
     * If not done, the setPoint won't work properly.
     */
    pid.SetTunings(KP, 0, 0);
    pid.Run(axelgyro.angle.z);
    pid.SetTunings(KP, KI, KD);

    // Finally set the setpoint
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
        /*
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

    motors.turnRightWheel(true, (int)(speed - output));
    motors.turnLeftWheel(true, (int)(speed + output));
}

void SmartMotors::goBackward(int speed)
{
    if(!toldToBackward)
    {
        /*
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

void SmartMotors::turnRight(float angle)
{
    // Update the aimed angle only if the function wasn't already called previously
    if(!toldToRight)
    {
        aimedAngle = axelgyro.preAngle.z - angle;
    }

    // Update the command variables
    toldToForward = false;
    toldToBackward = false;
    toldToRight = true;
    toldToLeft = false;
}

void SmartMotors::turnLeft(float angle)
{
    // Update the aimed angle only if the function wasn't already called previously
    if(!toldToLeft)
    {
        aimedAngle = axelgyro.preAngle.z + angle;
    }

    // Update the command variables
    toldToForward = false;
    toldToBackward = false;
    toldToRight = false;
    toldToLeft = true;
}