#include "RescueBot.h"


RescueBot::RescueBot() {}

RescueBot::~RescueBot() {}

void RescueBot::setup()
{
    smartMotors.setup();
    ultrasonicSensors.setup();
    radio.setup();
}

void RescueBot::update()
{    
    smartMotors.update();

    smartMotors.turnLeft(PI);

/*
    if(!ultrasonicSensors.collisionDetection(true, false))
        smartMotors.goForward(255);
    else
        smartMotors.stop();
*/
}

void RescueBot::scan() {}

void RescueBot::collisionAvoidance() {}

void RescueBot::explore() {}
