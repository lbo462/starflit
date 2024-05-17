#include "Imu.h"
#include "I2C.h"

Imu::Imu()
{

};

Imu::~Imu()
{

};

bool Imu::isReady()
{
    uint8_t isReady;

    I2Cread(imu_addr, 58, 1, &isReady);
    return isReady & 1;
}

void Imu::readRaw()
{
    uint8_t buff[14];

    I2Cread(imu_addr, 59, 14, buff);

    angles.accelerometer.x = (buff[0] << 8 | buff[1]);
    angles.accelerometer.y = (buff[2] << 8 | buff[3]);
    angles.accelerometer.z = (buff[4] << 8 | buff[5]);

    angles.gyroscope.x = (buff[8] << 8 | buff[9]);
    angles.gyroscope.y = (buff[10] << 8 | buff[11]);
    angles.gyroscope.z = (buff[12] << 8 | buff[13]);
}