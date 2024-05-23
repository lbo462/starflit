#include "Motors.h"


Motors::Motors() {}
Motors::~Motors() {}

void Motors::setup()
{
    pinMode(motorRP, OUTPUT);
    pinMode(motorRM, OUTPUT);
    pinMode(motorLP, OUTPUT);
    pinMode(motorLM, OUTPUT);

    stop();  // Simple sanitize check, cut off the motors
}

void Motors::stop()
{
    digitalWrite(motorRP, LOW);
    digitalWrite(motorRM, LOW);
    digitalWrite(motorLP, LOW);
    digitalWrite(motorLM, LOW);
}

void Motors::turnRightWheel(bool forward, int voltage)
{
    if(forward)
    {
        analogWrite(motorRP, LOW);
        analogWrite(motorRM, voltage);
    }
    else
    {
        analogWrite(motorRP, voltage);
        analogWrite(motorRM, LOW);
    }
}

void Motors::turnLeftWheel(bool forward, int voltage)
{
    if(forward)
    {
        analogWrite(motorLP, voltage);
        analogWrite(motorLM, LOW);
    }
    else
    {
        analogWrite(motorRP, LOW);
        analogWrite(motorRP, voltage);
    }
}