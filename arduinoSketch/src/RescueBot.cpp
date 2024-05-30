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
    unsigned long currentMillis = millis();
    smartMotors.update();

    /** 
     * Do the moves !
     * 
     *   _O/
     *     \ 
     *     /\_ 
     *     \  ` 
     *     `   
     */

    /** Check if it's time to scan or if we're actually scanning */
    if(isScanning() || currentMillis - previousScan > SCAN_INTERVAL)
    {
        scan();
    }

    else
    {
        explore();
    }
}

void RescueBot::scan()
{
    /** Set appropriate state */
    if(!scanning)
        scanning = true;

    if(ultrasonicSensors.collisionDetection(true, false))
    {
        /** Exit scanning to enter collision avoidance at next iteration */
        smartMotors.goBackward();
        scanning = false, scannedRight = false, scannedLeft = false;
        previousScan = millis();
        return;
    }

    /**
     * The robots is currently turning, let it turn in peace until it stops! 
     * Note that we verified previously that the robot won't collide with any front object.
     */
    if(smartMotors.toldToRight || smartMotors.toldToLeft)
        return;

    if(scannedRight && scannedLeft)
    {
        /**
         * We finished the scan, hence we can go back to normal state.
         * But first, one needs to get back to its initial position
         */
        smartMotors.turnRight(PI/4);

        /** Exit scanning */
        scanning = false, scannedRight = false, scannedLeft = false;
        previousScan = millis();
        return;
    }

    if(!(scannedRight || scannedLeft))
    {
        /**
         * Here, we enter the scanning process.
         * We never scanned anything, hence we scan right (arbitrary choice)
         */
        smartMotors.turnRight(PI/4);

        /**
         * We just commanded the scan but we already set the `scannedRight` to true.
         * This is because we won't check its value until it stopped turning.
         */
        scannedRight = true;
    }
    else if(scannedRight)
    {
        // We scanned right so now, we scan left
        smartMotors.turnLeft(PI/2);
        scannedLeft = true;
    }
}

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

    /**
     * It's possible that `collisionAvoidance()` changed the `toldToForward` state.
     * In that case, try to go forward.
     */
    if(smartMotors.toldToForward)
    {
        if(!ultrasonicSensors.collisionDetection(true, false))
            smartMotors.goForward();
        else
            smartMotors.goBackward();
    }
}
