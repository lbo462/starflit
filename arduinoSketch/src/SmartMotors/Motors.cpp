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

    /**
     * Avoid falling forward.
     * Does not work as well as expected for high speed value ...
     * TODO This should be revised.
     */
    delay(200);
}

bool Motors::turnRightWheel(bool forward, int voltage)
{
    const int appliedVoltage = voltage > 0 ? voltage : -voltage;
    if(appliedVoltage >= MIN_POWER)
    {
        analogWrite(motorRP, forward == voltage > 0 ? appliedVoltage : LOW);
        analogWrite(motorRM, forward == voltage > 0 ? LOW : appliedVoltage);
        return true;
    }
    return false;
}

bool Motors::turnLeftWheel(bool forward, int voltage)
{
    const int appliedVoltage = voltage > 0 ? voltage : -voltage;
    if(appliedVoltage >= MIN_POWER)
    {
        analogWrite(motorLP, forward == voltage > 0 ? LOW : appliedVoltage);
        analogWrite(motorLM, forward == voltage > 0 ? appliedVoltage : LOW);
        return true;
    }
    return false;
}