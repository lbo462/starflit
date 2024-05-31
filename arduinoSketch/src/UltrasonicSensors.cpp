#include "UltrasonicSensors.h"


UltrasonicSensors::UltrasonicSensors() {}

UltrasonicSensors::~UltrasonicSensors() {}

void UltrasonicSensors::setup()
{
    pinMode(trig1, OUTPUT);
    pinMode(trig2, OUTPUT);
    pinMode(echo, OUTPUT);
}

unsigned int UltrasonicSensors::getDistance(int trigPin)
{
    digitalWrite(echo, 0); 
    pinMode(echo, INPUT);

    digitalWrite(trigPin, 1); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, 0);

    const long distance = pulseIn(echo, 1);
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

bool UltrasonicSensors::collisionDetection(bool fromFront, bool fromRear, unsigned int distance)
{
    return (fromFront && getFrontDistance() < distance) || (fromRear && getRearDistance() < distance);
}