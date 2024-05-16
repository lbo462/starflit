#include "Imu.h"
#include "I2C.h"

Imu::Imu()
{

};

Imu::~Imu()
{

};

bool Imu::isImuReady()
{
    uint8_t isReady;

    I2Cread(imu_addr, 58, 1, &isReady);
    return isReady & 1;
}

void Imu::readRawImu()
{
    uint8_t buff[14];

    I2Cread(imu_addr, 59, 14, buff);

    accelerometer.x = (buff[0] << 8 | buff[1]);
    accelerometer.y = (buff[2] << 8 | buff[3]);
    accelerometer.z = (buff[4] << 8 | buff[5]);

    gyroscope.x = (buff[8] << 8 | buff[9]);
    gyroscope.y = (buff[10] << 8 | buff[11]);
    gyroscope.z = (buff[12] << 8 | buff[13]);
}