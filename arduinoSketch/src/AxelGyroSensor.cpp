#include "AxelGyroSensor.h"


AxelGyroSensor::AxelGyroSensor()
{
    /**
     * Function constructor is moved to the method setup()
     * This is not for no reason, please, leave it that way
     * `setup()` should be called during global setup
     */
}

AxelGyroSensor::~AxelGyroSensor()
{

}

void AxelGyroSensor::setup()
{
    if(!mpu.begin())
    {
        Serial.print("Impossible to load axelgyro, check wires");
        exit(1);
    }
  
    mpu_temp = mpu.getTemperatureSensor();
    mpu_temp->printSensorDetails();

    mpu_accel = mpu.getAccelerometerSensor();
    mpu_accel->printSensorDetails();

    mpu_gyro = mpu.getGyroSensor();
    mpu_gyro->printSensorDetails();
}

void AxelGyroSensor::update()
{
    // Update frame time
    sampleMicros = (lastSampleMicros > 0) ? micros() - lastSampleMicros : 0;
    lastSampleMicros = micros();

    // Get angles required for computation
    Vector3D rawAxel = getRawAxel();
    Vector3D rawGyro = getRawGyro();

    Vector3D axel = getAxelAngle(rawAxel);
    Vector3D gyro = getGyroAngle(rawGyro);    

    // Update angle speed
    angleSpeed = Vector3D(
        0.98 * (angleSpeed.x + degrees(gyro.x)) + 0.02 * degrees(axel.x),
        0.98 * (angleSpeed.y + degrees(gyro.y)) + 0.02 * degrees(axel.y),
        0.98 * (angleSpeed.z + degrees(gyro.z)) + 0.02 * degrees(axel.z)
    );

    // Update angle
    //angle = Vector3D(
    //    angle.x + angleSpeed.x / (sampleMicros * 0.001),
    //    angle.y + angleSpeed.y / (sampleMicros * 0.001),
    //    angle.z + angleSpeed.z / (sampleMicros * 0.001)
    //);
}

Vector3D AxelGyroSensor::getRawAxel()
{
    sensors_event_t a;
    mpu_accel->getEvent(&a);

    return Vector3D(
        a.acceleration.x,
        a.acceleration.y,
        a.acceleration.z
    );
}

Vector3D AxelGyroSensor::getRawGyro()
{
    sensors_event_t g;
    mpu_gyro->getEvent(&g);

    return Vector3D(
        g.gyro.x,
        g.gyro.y,
        g.gyro.z
    );
}

Vector3D AxelGyroSensor::getAxelAngle(Vector3D rawAxel)
{
    const Vector3D axel = rawAxel.normalize(9.80665 / 16384);
    return Vector3D(
        atan(axel.y / sqrt(axel.x * axel.x + axel.z * axel.z)),
        atan(-1 * axel.x / sqrt(axel.y * axel.y + axel.z * axel.z)),
        atan2(rawAxel.y, rawAxel.x)
    );
}

Vector3D AxelGyroSensor::getGyroAngle(Vector3D rawGyro)
{
    return rawGyro.normalize((1.0 / 32.8) * (float(sampleMicros) / 1000000.0));
}
