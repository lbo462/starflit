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
    sampleMicros = (lastSampleMicros > 0) ? micros() - lastSampleMicros : 20000;
    lastSampleMicros = micros();

    // Get raw reading from sensors
    Vector3D rawAxel = getRawAxel();
    Vector3D rawGyro = getRawGyro();

    // Compute in-between vectors for the complementary filter
    Vector3D axel = getAxelAngle(rawAxel);
    Vector3D gyro = getGyroAngle(rawGyro);    

    // Compute the actual complementary filter
    complementaryFilterOutput = Vector3D(
        0.98 * (complementaryFilterOutput.x + degrees(gyro.x)) + 0.02 * degrees(axel.x),
        0.98 * (complementaryFilterOutput.y + degrees(gyro.y)) + 0.02 * degrees(axel.y),
        0.98 * (complementaryFilterOutput.z + degrees(gyro.z)) + 0.02 * degrees(axel.z)
    );

    Serial.print(angle.z);
    Serial.print(" + ");
    Serial.print(rawGyro.z);
    Serial.print(" * ");
    Serial.println(sampleMicros * 0.000001);

    // Update angle
    angle = Vector3D(
        angle.x + rawGyro.x * (sampleMicros * 0.000001),
        angle.y + rawGyro.y * (sampleMicros * 0.000001),
        angle.z + rawGyro.z * (sampleMicros * 0.000001)
    );
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
