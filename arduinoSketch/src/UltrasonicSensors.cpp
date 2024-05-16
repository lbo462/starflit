#include "UltrasonicSensors.h"


UltrasonicSensors::UltrasonicSensors()
{
    /* Function constructor is moved to the method setup()
     * This is not for no reason, please, leave it that way
     * It should be called during global setup
     */
}

UltrasonicSensors::~UltrasonicSensors()
{

}

void UltrasonicSensors::setup()
{
    pinMode(trig1, OUTPUT);
    pinMode(trig2, OUTPUT);
    pinMode(echo, OUTPUT); // ECHO is used as output for RCB_LEDS_WS2812
}

unsigned int UltrasonicSensors::getDistance(int trigPin)
{
    digitalWrite(echo, 0); 
    pinMode(echo, INPUT);

    digitalWrite(trigPin, 1); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, 0);

    const long distance = pulseIn(echo, 1);
    Serial.println(distance / 58);
    pinMode(echo, OUTPUT);

    return(distance / 58);
}

unsigned int UltrasonicSensors::getFrontDistance()
{
    return getDistance(trig1);
}

unsigned int UltrasonicSensors::getRearDistance()
{
    return getDistance(trig2);
}

bool UltrasonicSensors::collisionDetection(bool goingForward, bool goingBackward)
{
    return (goingForward && getFrontDistance() < 30) || (goingBackward && getRearDistance() < 30);
}