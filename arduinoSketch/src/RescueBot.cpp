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

    explore();
}

void RescueBot::scan() {}

void RescueBot::collisionAvoidance()
{
    // Back away until the front object is 80 cm away  
    if(ultrasonicSensors.collisionDetection(true, false, 80))
    {
        smartMotors.goBackward(255);
    }

    // Now, turn until there's no front object at 100 cm
    else if(ultrasonicSensors.collisionDetection(true, false, 100))
    {
        if(!smartMotors.toldToRight && !smartMotors.toldToRight)
        {
            // Set a new random direction, and make a full turn
            if (random(2) == 0)
                smartMotors.turnRight(2*PI);
            else
                smartMotors.turnLeft(2*PI);
        }
    }

    // Collision avoidance exit
    else
    {
        smartMotors.goForward();
    }
}

void RescueBot::explore()
{
    if(!smartMotors.toldToForward && !ultrasonicSensors.collisionDetection(true, false))
        smartMotors.goForward();
    else
        collisionAvoidance();
}
