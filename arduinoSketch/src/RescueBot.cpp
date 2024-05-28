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
    // The robots is currently turning, let it turn in peace until it stops!
    if(smartMotors.toldToRight || smartMotors.toldToLeft)
        return;

    // If we've stop, it means we're able to go forward and exit collision avoidance.
    if(smartMotors.stopped())
    {
        smartMotors.goForward();
        return;
    }

    // Back away until the front object is 80 cm away  
    if(ultrasonicSensors.collisionDetection(true, false, 80))
    {
        // ... but verify that we won't collide rear objects.
        if(!ultrasonicSensors.collisionDetection(false, true))
            smartMotors.goBackward();
        // If something's behind, just stop.
        else
            smartMotors.stop();
    }

    // If no object is to be seen in front, make a turn
    else
    {
        if(!(smartMotors.toldToRight || smartMotors.toldToLeft))
        {
            // Set a new random direction, and make a full turn
            if (random(2) == 0)
                smartMotors.turnRight(PI/2);
            else
                smartMotors.turnLeft(PI/2);
        }
        // else, do nothing and let it turn
    }
}

void RescueBot::explore()
{
    if(!smartMotors.toldToForward)
    {
        collisionAvoidance();
    }

    // It's possible that `collisionAvoidance()` changed the `toldToForward` state.
    // In that case, try to go forward.
    if(smartMotors.toldToForward)
    {
        if(!ultrasonicSensors.collisionDetection(true, false))
            smartMotors.goForward();
        else
            smartMotors.goBackward();
    }
}
