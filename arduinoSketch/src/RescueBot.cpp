#include "RescueBot.h"

RescueBot::RescueBot() : movementDelay(0)
{
    /**
     * Function constructor is moved to the method setup()
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
    explore();
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
    if(isMovementAvailable()){
        turnRight_();
        return true;
    }
    return false;

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
    if(isMovementAvailable()){
        turnLeft_();
        return true;
    }
    return false;

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
    
    if(!ultrasonicSensors.collisionDetection(true, false) && isMovementAvailable())
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
    if(!ultrasonicSensors.collisionDetection(false, true) && isMovementAvailable())
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

void RescueBot::collisionAvoidance(){

    // Si on avance, on s'arrête et on bloque le mouvement pendant 200 ms 
    if(isGoingForward()){
        stop();
        setMovementDelay(20);
    }

    // Si on n'avance pas, si on ne tourne pas ni à droite ni à gauche on lance le mouvement reculer et on le bloque pendant 200 ms
    else if(!isTurningLeft() && !isTurningRight()){
        goBackward();
        setMovementDelay(20);    
    }

    // Si on recule, on tente de lancer randomDirection()
    if (goBackward())
    {
        setRandomDirection();
    }

}

void RescueBot::explore(){

    // On avance, si on ne peut pas avance on appelle collisionAvoidance
    if(!goForward()){
        collisionAvoidance();
    }
    decreaseMovementDelay();

}


// Si movementDelay n'est pas déjà initialisé, on lui donne une valeur. Cela bloquera les mouvements pendant la duré définie
void RescueBot::setMovementDelay(int value){
    if(movementDelay==0){
        movementDelay=value;
    }
}

void RescueBot::decreaseMovementDelay(){
    if(movementDelay!=0){
        movementDelay-=1;
    }
    
}

bool RescueBot::isMovementAvailable(){
    if(movementDelay==0){
        return true;
    }
    return false;
}
