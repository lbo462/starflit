#include "RescueBot.h"


RescueBot::RescueBot()
{
    /**
     * Function constructor is moved to the method setup()
     * This is not for no reason, please, leave it that way
     * `setup()` should be called during global setup
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
    axelgyro.setup();

    // Radio setup
    radio.setup();

    // Simple sanitize check, cut off the motors
    stop();

    // Set an initial pretty low speed
    setSpeed(192);
}

void RescueBot::update()
{    
    axelgyro.update();

    Serial.println(axelgyro.angle.toString());
    radio.sendString(axelgyro.angle.toString());
    
    //explore();
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

void RescueBot::turnRight_()
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

bool RescueBot::turnRight()
{
    turnRight_();
    return true;
}

void RescueBot::turnLeft_()
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

bool RescueBot::turnLeft()
{
    turnLeft_();
    return true;
}

void RescueBot::goForward_()
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

bool RescueBot::goForward()
{
    if(!ultrasonicSensors.collisionDetection(true, false))
    {
        goForward_();
        return true;
    }
    stop();
    return false;
}

void RescueBot::goBackward_()
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

bool RescueBot::goBackward()
{
    if(!ultrasonicSensors.collisionDetection(false, true))
    {
        goBackward_();
        return true;
    }
    stop();
    return false;
}

bool RescueBot::setRandomDirection()
{  
    bool couldTurn;

    if (random(2) == 0) {
        couldTurn = turnLeft();
    } else {
        couldTurn = turnRight();
    }
    return couldTurn;
}

void RescueBot::scan()
{

    if (ultrasonicSensors.collisionDetection(true, false,50)) {
        collisionAvoidance();
        return;
    }

    stop();
    delay(250);
    turnRight();
    delay(250);

    if (ultrasonicSensors.collisionDetection(true, false,50)) {
        collisionAvoidance();
        return;
    }

    stop();
    delay(250);
    turnLeft();
    delay(500);
    if (ultrasonicSensors.collisionDetection(true, false,50)) {
        collisionAvoidance();
        return;
    }
    stop();
    delay(250);
    turnRight();
    delay(250);
    stop();
}


void RescueBot::collisionAvoidance()
{
    // Go backward until the front object is 80 cm away  
    if(ultrasonicSensors.collisionDetection(true, false, 80)) {
        goBackward();
    }

    // Now, turn until there's no front object at 100 cm
    else if(ultrasonicSensors.collisionDetection(true, false, 100)) {
        if(!isTurningLeft() && !isTurningRight())
            setRandomDirection();
    }

    // Then continue forward
    else {
        goForward();
    }
}

void RescueBot::explore()
{

    unsigned long scanCurrentMillis = millis();  // Récupère le temps actuel

    if (scanCurrentMillis - scanPreviousMillis >= scanInterval) {  // Vérifie si l'intervalle est écoulé

        // Appelle la fonction scan()
        scan();

        // Réinitialise le temps de la dernière exécution de scan()
        scanPreviousMillis = millis(); 

    } else {
        if(!isGoingForward())
        {
            // If we're not going forward it means that we're a state that requires collision avoidance
            collisionAvoidance();
        }
        else
        {   
            goForward();
        }
    } 

}