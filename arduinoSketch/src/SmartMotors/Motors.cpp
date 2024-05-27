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
    analogWrite(motorRP, forward ? LOW : max(voltage, 0));
    analogWrite(motorRM, forward ? max(voltage, 0) : LOW);
}

void Motors::turnLeftWheel(bool forward, int voltage)
{
    analogWrite(motorLP, forward ? max(voltage, 0) : LOW);
    analogWrite(motorLM, forward ? LOW : max(voltage, 0));
}