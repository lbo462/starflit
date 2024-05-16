#include "DriftCompensator.h"
#include "imu.h"
#include "Wire.h"
#include "I2C.h"

DriftCompensator::DriftCompensator()
{

};

DriftCompensator::~DriftCompensator()
{

};

void DriftCompensator::setup()
{
    Wire.begin();
    Serial.begin(115200);
    delay(1000);

    I2CwriteByte(imu_addr, 25, 1);
    I2CwriteByte(imu_addr, 26, 1);
    I2CwriteByte(imu_addr, 27, gyro_fs_1000_dps);
    I2CwriteByte(imu_addr, 28, acc_fs_2g);
    I2CwriteByte(imu_addr, 56, 1);
}

void DriftCompensator::readSample()
{

}