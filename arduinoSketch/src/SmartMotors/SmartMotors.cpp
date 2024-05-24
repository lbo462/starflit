#include "SmartMotors.h"


SmartMotors::SmartMotors() {}
SmartMotors::~SmartMotors() {}

void SmartMotors::setup()
{
    motors.setup();
    axelgyro.setup();
    pid.Start(axelgyro.angle.z, PIDoutput, PIDsetPoint);
    pid.SetMode(AUTOMATIC);
}

void SmartMotors::update()
{
    axelgyro.update();
    PIDoutput = pid.Run(axelgyro.angle.z);
}

void SmartMotors::goForward(int speed) 
{
    PIDsetPoint = axelgyro.getRawGyro().z;

    float yaw = axelgyro.getRawGyro().z;
    float yawRatio = yaw * 2;

    int rightSpeed, leftSpeed;
    if(yawRatio < -0.5)
    {
        rightSpeed = 0; leftSpeed = speed;
    }
    else if (yawRatio > 0.5)
    {
        rightSpeed = speed; leftSpeed = 0;
    }
    else
    {
        rightSpeed = speed * (0.5 - yawRatio);
        leftSpeed = speed * (0.5 + yawRatio);
    }
    
    motors.turnRightWheel(true, rightSpeed);
    motors.turnLeftWheel(true, leftSpeed);
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