#include "AxelGyroSensor.h"

AxelGyroSensor::AxelGyroSensor()
{
    /**
     * Function constructor is moved to the method setup()
     * This is not for no reason, please, leave it that way
     * It should be called during global setup
     */
}

AxelGyroSensor::~AxelGyroSensor()
{

}

void AxelGyroSensor::setup()
{
    if(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
    {
        Serial.print("Impossible to load axelgyro, check wires");
        exit(1);
    }
}
