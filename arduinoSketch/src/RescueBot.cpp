#include "RescueBot.h"


RescueBot::RescueBot() {}

RescueBot::~RescueBot() {}

void RescueBot::setup()
{
    smartMotors.setup();
    radio.setup();
}

void RescueBot::update()
{    
    smartMotors.update();
    smartMotors.goForward(156);
}

void RescueBot::scan() {}

void RescueBot::collisionAvoidance() {}

void RescueBot::explore() {}
