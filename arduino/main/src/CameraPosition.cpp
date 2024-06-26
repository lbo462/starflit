#include "CameraPosition.h"

CameraPosition::CameraPosition() {}

CameraPosition::~CameraPosition() {}

void CameraPosition::setup()
{
    pwm.begin();
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

    // Set the servos to their initial position.
    setDefault();
}

void CameraPosition::setDefault()
{
    setVertical(defaultZPosition());
    setHorizontal(defaultXPosition());
}

void CameraPosition::setVertical(int position)
{
    pwm.setPWM(
        verticalChannel, 0,
        round(max(min(position, zMax), zMin))
    );
}

void CameraPosition::setHorizontal(int position)
{
    pwm.setPWM(
        horizontalChannel, 0,
        round(max(min(position, xMax), xMin))
    );
}