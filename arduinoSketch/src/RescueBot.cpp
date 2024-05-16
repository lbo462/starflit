#include "RescueBot.h"

RescueBot::RescueBot()
{
    /* Function constructor is moved to the method setup()
     * This is not for no reason, please, leave it that way
     * It should be called during global setup
     */
}

RescueBot::~RescueBot()
{

}

void RescueBot::setup()
{
    pinMode(mrp, OUTPUT);
    pinMode(mrm, OUTPUT);
    pinMode(mlp, OUTPUT);
    pinMode(mlm, OUTPUT);

    // Sensors set-up
    ultrasonicSensors.setup();

    // Simple sanitasize check, cut off the motors
    stop();

    // Set an initial pretty low speed
    setSpeed(192);
}

void RescueBot::update()
{
    if(ultrasonicSensors.collisionDetection(isGoingForward(), isGoingBackward()))
    {
        stop();
        delay(1000);
        goBackward();
        delay(1000);
        randomDirection();
    }
    else 
    {
        goForward();
    }
}

void RescueBot::stop()
{
    digitalWrite(mrp, 0);
    digitalWrite(mrm, 0);
    digitalWrite(mlp, 0);
    digitalWrite(mlm, 0);

    isGoingForward_ = false;
    isGoingBackward_ = false;
    isTurningRight_ = false;
    isTurningLeft_ = false;
}

void RescueBot::setSpeed(int speed)
{
    Wire.beginTransmission(40);
    Wire.write(17);
    Wire.write(speed);
    Wire.endTransmission();
}

void RescueBot::turnRight()
{
    digitalWrite(mrp, 0);
    digitalWrite(mrm, 1);
    digitalWrite(mlp, 0);
    digitalWrite(mlm, 1);

    isGoingForward_ = false;
    isGoingBackward_ = false;
    isTurningRight_ = true;
    isTurningLeft_ = false;
}

void RescueBot::turnLeft()
{
    digitalWrite(mrp, 1);
    digitalWrite(mrm, 0);
    digitalWrite(mlp, 1);
    digitalWrite(mlm, 0);

    isGoingForward_ = false;
    isGoingBackward_ = false;
    isTurningRight_ = false;
    isTurningLeft_ = true;
}

void RescueBot::goForward()
{
    digitalWrite(mrp, 0);
    digitalWrite(mrm, 1);
    digitalWrite(mlp, 1);
    digitalWrite(mlm, 0);

    isGoingForward_ = true;
    isGoingBackward_ = false;
    isTurningRight_ = false;
    isTurningLeft_ = false;
}

void RescueBot::goBackward()
{
    digitalWrite(mrp, 1);
    digitalWrite(mrm, 0);
    digitalWrite(mlp, 0);
    digitalWrite(mlm, 1);

    isGoingForward_ = false;
    isGoingBackward_ = true;
    isTurningRight_ = false;
    isTurningLeft_ = false;
}

void RescueBot::randomDirection()
{  
    if (random(2) == 0) {
        Serial.print("Direction : turnleft \n");
        turnLeft();
    } else {
        Serial.print("Direction : turnright \n");
        turnRight();
    }
    delay(1000);
    stop();
    Serial.print("randomDirection : sortie de la fonction \n");
}