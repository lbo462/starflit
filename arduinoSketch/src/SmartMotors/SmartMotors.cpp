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
        pidSetpoint(axelgyro.angle.z);
    }

    // Get clamped output between 0 and `speed` from the PID module
    const float output = min(pid.Run(axelgyro.angle.z), speed);

    // Update the command variables
    toldToForward = true;
    toldToBackward = false;
    toldToRight = false;
    toldToLeft = false;
    
    motors.turnRightWheel(true, (int)(speed + output));
    motors.turnLeftWheel(true, (int)(speed - output));
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
        pidSetpoint(axelgyro.angle.z);
    }

    // Get clamped output between 0 and `speed` from the PID module
    const float output = min(pid.Run(axelgyro.angle.z), speed);

    // Update the command variables
    toldToForward = false;
    toldToBackward = true;
    toldToRight = false;
    toldToLeft = false;

    motors.turnRightWheel(false, (int)(speed - output));
    motors.turnLeftWheel(false, (int)(speed + output));
}

void SmartMotors::turnRight(float angle, int speed, float allowedError)
{
    /** Since we're turning right, the aimed angle has the opposite sign. */
    angle = -angle;

    if(abs(axelgyro.angle.z - angle) < abs(allowedError))
    {
        /** Stop the motors and ignore the command if angle is the allowed error range */
        motors.stop();
        return;
    }
    
    if(!toldToRight)
    {
        /**
         * We could set the set point every time this function is called.
         * But that would be computations wasted computation power.
         * Instead, we set it up only once, when the order was first given.
         */
        pidSetpoint(axelgyro.angle.z + angle);
    }

    const float output = pid.Run(axelgyro.angle.z);

    // Update the command variables
    toldToForward = false;
    toldToBackward = false;
    toldToRight = true;
    toldToLeft = false;

    Serial.print(axelgyro.angle.z);
    Serial.print(" / ");
    Serial.print(pid.GetSetpoint());
    Serial.print(" -> ");
    Serial.print(output);
    Serial.print(" -> ");
    Serial.print((int)(speed * output));
    Serial.println();

    motors.turnRightWheel(false, (int)(speed * output));
    motors.turnLeftWheel(false, (int)(speed * output));
}

void SmartMotors::turnLeft(float angle, int speed, float allowedError)
{
}