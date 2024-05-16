#include "Normalizer.h"

Normalizer::Normalizer()
{

};

Normalizer::~Normalizer()
{

};

void Normalizer::normalizeGyro(gyroscope_raw gyroscope)
{
    // Sensitivity Scale Factor (MPU datasheet page 8)
    normalized.gyroscope.x = gyroscope.x / 32.8;
    normalized.gyroscope.y = gyroscope.y / 32.8;
    normalized.gyroscope.z = gyroscope.z / 32.8;
};

void Normalizer::normalizeAcc(accelerometer_raw accelerometer)
{
    // Sensitivity Scale Factor (MPU datasheet page 9)
    normalized.accelerometer.x = accelerometer.x * g / 16384;
    normalized.accelerometer.y = accelerometer.y * g / 16384;
    normalized.accelerometer.z = accelerometer.z * g / 16384;
}

