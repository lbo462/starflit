#include "imu.h"
#include "I2C.h"
#include <cstdint>

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

}