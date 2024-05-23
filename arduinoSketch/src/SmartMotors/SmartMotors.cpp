#include "SmartMotors.h"


SmartMotors::SmartMotors() {}
SmartMotors::~SmartMotors() {}

void SmartMotors::setup()
{
    motors.setup();
    ultrasonicSensors.setup();
    axelgyro.setup();
}

void SmartMotors::update()
{
    axelgyro.update();
}

bool SmartMotors::goForward(int speed) 
{
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
