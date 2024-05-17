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
    I2CwriteByte(imuAddr, 25, 0x01); // Set the SRD to 1
    I2CwriteByte(imuAddr, 26, 0x01); // Set the DLPF to 184HZ by default
    I2CwriteByte(imuAddr, 27, gyro_fs_1000_dps); // Configure gyroscope range
    I2CwriteByte(imuAddr, 28, acc_fs_2g); // Configure accelerometer range
    I2CwriteByte(imuAddr, 56, 0x01); // Enable interrupt pin for raw data
}

void AxelGyroSensor::update()
{
    // Update frame time
    sampleMicros = (lastSampleMicros > 0) ? micros() - lastSampleMicros : 0;
    lastSampleMicros = micros();

    // Get angles required for computation
    Vector3D axel = getAxelAngle(getRawAxel());
    Vector3D gyro = getGyroAngle(getRawGyro());

    // Update position
    position = Vector3D(
        0.98 * (position.x + degrees(gyro.x)) + 0.02 * degrees(axel.x),
        0.98 * (position.y + degrees(gyro.y)) + 0.02 * degrees(axel.y),
        0.98 * (position.z + degrees(gyro.z)) + 0.02 * degrees(axel.z)
    );
}

Vector3D AxelGyroSensor::getRawAxel()
{
    uint8_t buff[14];
    I2Cread(imuAddr, 59, 14, buff);

    return Vector3D(
        buff[0] << 8 | buff[1],
        buff[2] << 8 | buff[3],
        buff[4] << 8 | buff[5]
    );
}

Vector3D AxelGyroSensor::getRawGyro()
{
    uint8_t buff[14];
    I2Cread(imuAddr, 59, 14, buff);

    return Vector3D(
        buff[8] << 8 | buff[9],
        buff[10] << 8 | buff[11],
        buff[12] << 8 | buff[13]
    );
}

Vector3D AxelGyroSensor::getAxelAngle(Vector3D rawAxel)
{
    Vector3D axel = rawAxel.normalize(9.80665 / 16384);
    return Vector3D(
        atan(axel.y / sqrt(axel.x * axel.x + axel.z * axel.z)),
        atan(-1 * axel.x / sqrt(axel.y * axel.y + axel.z * axel.z)),
        atan2(rawAxel.y, rawAxel.x)
    ); 
}

Vector3D AxelGyroSensor::getGyroAngle(Vector3D rawGyro)
{
    Vector3D gyro = rawGyro.normalize((1 / 32.8));
    return gyro.normalize(sampleMicros / 1000000);
}
