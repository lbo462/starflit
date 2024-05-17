#include "Angles.h"
#include <math.h>


Angles::Angles()
{

};

Angles::~Angles()
{

};

angle Angles::calculateAccelerometerAngles()
{
    angle accelerometer;

    accelerometer.x = atan(normalized.accelerometer.y / sqrt(sq(normalized.accelerometer.x) + sq(normalized.accelerometer.z)));
    accelerometer.y = atan(-1 * normalized.accelerometer.x / sqrt(sq(normalized.accelerometer.y) + sq(normalized.accelerometer.z)));
    accelerometer.z = atan2(accelerometer.y, accelerometer.x);

    return accelerometer;
};

angle Angles::calculateGyroscopeAngles(unsigned long sampleMicros)
{
    angle gyroscope;

    gyroscope.x = normalized.gyroscope.x * sampleMicros / 1000000;
    gyroscope.y = normalized.gyroscope.y * sampleMicros / 1000000;
    gyroscope.z = normalized.gyroscope.z * sampleMicros / 1000000;

    return gyroscope;
};

void Angles::detectPitch(angle gyroscope, angle accelerometer)
{
  position.x = 0.98 * (position.x + degrees(gyroscope.x)) + 0.02 * degrees(accelerometer.x);
};

double Angles::getPitch()
{
  return position.x;
};

void Angles::detectRoll(angle gyroscope, angle accelerometer)
{
  position.y = 0.98 * (position.y + degrees(gyroscope.y)) + 0.02 * degrees(accelerometer.y);
};

double Angles::getRoll()
{
  return position.y;
};

void Angles::detectYaw(angle gyroscope, angle accelerometer)
{
  position.z = 0.98 * (position.z + degrees(gyroscope.z)) + 0.02 * degrees(accelerometer.z);
};

double Angles::getYaw()
{
  return position.z;
};