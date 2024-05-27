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

void SmartMotors::stop()
{
    // Update the command variables
    toldToForward = false;
    toldToBackward = false;
    toldToRight = false;
    toldToLeft = false;

    motors.stop();

    pid.SetTunings(KP, 0, 0);
    pid.Run(axelgyro.angle.z);
    pid.SetTunings(KP, KI, KD);
}

void SmartMotors::goForward(int speed) 
{
    if(!toldToForward)
    {
        /**
         * Defines the setPoint for the "forward" direction.
         * This is done only when the smartMotors where first told to go forward.
         * The setPoint do not change until a new order arrives.
         * To set this setPoint properly, one needs to reset the PID since we couldn't
         * find a way to use the built-in `SetPoint()` method. 
         */
        Serial.print("New SetPoint : ");
        Serial.print(axelgyro.angle.z);
        Serial.print(" -> ");
        pid.Setpoint(axelgyro.angle.z);
        Serial.println(pid.GetSetpoint());
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
}

void SmartMotors::turnRight(int speed)
{
}

void SmartMotors::turnLeft(int speed)
{
}