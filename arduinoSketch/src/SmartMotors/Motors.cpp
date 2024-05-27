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

bool Motors::turnRightWheel(bool forward, int voltage)
{
    const int appliedVoltage = voltage > 0 ? voltage : -voltage;
    if(voltage > MIN_POWER)
    {
        analogWrite(motorRP, forward && voltage > 0 ? LOW : appliedVoltage);
        analogWrite(motorRM, forward && voltage > 0 ? appliedVoltage : LOW);
        return true;
    }
    return false;
}

bool Motors::turnLeftWheel(bool forward, int voltage)
{
    const int appliedVoltage = voltage > 0 ? voltage : -voltage;
    if(voltage > MIN_POWER)
    {
        analogWrite(motorLP, forward && voltage > 0 ? max(voltage, 0) : LOW);
        analogWrite(motorLM, forward && voltage > 0 ? LOW : max(voltage, 0));
        return true;
    }
    return false;
}