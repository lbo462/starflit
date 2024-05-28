#include "AxelGyroSensor.h"


AxelGyroSensor::AxelGyroSensor() {}
AxelGyroSensor::~AxelGyroSensor() {}

void AxelGyroSensor::setup()
{
    if(!mpu.begin())
    {
        Serial.println("Impossible to load axelgyro, check wires.");
        exit(1);
    }

    mpu_temp = mpu.getTemperatureSensor();
    mpu_temp->printSensorDetails();

    mpu_accel = mpu.getAccelerometerSensor();
    mpu_accel->printSensorDetails();

    mpu_gyro = mpu.getGyroSensor();
    mpu_gyro->printSensorDetails();

    Serial.println("Calibrating sensors ... do not move!");
    if(calibrate() != 0)
    {
        Serial.println("Impossible to calibrate sensors but continuing anyway ...");
    }
    Serial.println("Calibrated sensors!");
}

bool AxelGyroSensor::calibrate(
    Vector3D allowedAccelError,
    Vector3D allowedGyroError,
    unsigned long maxDelay)
{
    unsigned long startTime = millis(); // Time at which the calibration started
    int sensorReady = 0;  // Amount of sensors ready

    while(sensorReady < 6)
    {
        if(millis() - startTime > maxDelay) return 1;  // Could not calibrate in a convenient delay

        // Get the mean values
        Vector3D meanAxel = getMeanAccel();
        Vector3D meanGyro = getMeanGyro();

        // Compute the values that would have been read taking the offset into account
        Vector3D axelValue = meanAxel - axelOffset;
        Vector3D gyroValue = meanGyro - gyroOffset;

        /**
         * Now, we verify that the value we read match the allowed errors
         * If this verification fails, we update the offset with the mean
         * If the verification passes, consider the sensor ready and calibrated!
         */

        Serial.println(axelValue.toString());

        if(abs(axelValue.x) > abs(allowedAccelError.x))
            axelOffset.x = meanAxel.x;
        else sensorReady++;

        if(abs(axelValue.y) > abs(allowedAccelError.y))
            axelOffset.y = meanAxel.y;
        else sensorReady++;

        if(abs(axelValue.z) > abs(allowedAccelError.z))
            axelOffset.z = meanAxel.z;
        else sensorReady++;

        if(abs(gyroValue.x) > abs(allowedGyroError.x))
            gyroOffset.x = meanGyro.x;
        else sensorReady++;

        if(abs(gyroValue.y) > abs(allowedGyroError.y))
            gyroOffset.y = meanGyro.y;
        else sensorReady++;

        if(abs(gyroValue.z) > abs(allowedGyroError.z))
            gyroOffset.z = meanGyro.z;
        else sensorReady++;
    }
    return 0;
}

Vector3D AxelGyroSensor::getMeanAccel(int bufferSize)
{
    // Sum of every values read from the sensor
    Vector3D sum = Vector3D();

    for(int i = 0; i < bufferSize; i++)
    {
        sum = sum + getRawAxel();
    }

    return sum / bufferSize;
}


Vector3D AxelGyroSensor::getMeanGyro(int bufferSize)
{
    // Sum of every values read from the sensor
    Vector3D sum = Vector3D();

    for(int i = 0; i < bufferSize; i++)
    {
        sum = sum + getRawGyro();
    }

    return sum / bufferSize;
}

void AxelGyroSensor::update()
{
    // Update frame time
    sampleMicros = (lastSampleMicros > 0) ? micros() - lastSampleMicros : 20000;
    lastSampleMicros = micros();

    // Get values from sensors (take offsets into account)
    Vector3D rawAxel = getOffsetedAxel();
    Vector3D rawGyro = getOffsetedGyro();

    // Compute in-between vectors for the complementary filter
    Vector3D axel = getAxelAngle(rawAxel);
    Vector3D gyro = getGyroAngle(rawGyro);    

    // Compute the actual complementary filter
    complementaryFilterOutput = Vector3D(
        0.98 * (complementaryFilterOutput.x + degrees(gyro.x)) + 0.02 * degrees(axel.x),
        0.98 * (complementaryFilterOutput.y + degrees(gyro.y)) + 0.02 * degrees(axel.y),
        0.98 * (complementaryFilterOutput.z + degrees(gyro.z)) + 0.02 * degrees(axel.z)
    );

    /** Actually update the angle */

    // Create a "pre-angle" from our calculations.
    // This pre-angle features negatives that will be removed next.
    preAngle = Vector3D(
        preAngle.x + rawGyro.x * (sampleMicros * 0.000001),
        preAngle.y + rawGyro.y * (sampleMicros * 0.000001),
        preAngle.z + rawGyro.z * (sampleMicros * 0.000001)
    );

    // Transform negative angles into positive angles by adding 2*PI
    // Not sure about the use of the ternary expression here since the raw modulo
    //   might do the job on its own ... TODO but definitely not urgent and not mandatory.
    angle = Vector3D(
        fmod(preAngle.x > 0 ? preAngle.x : preAngle.x + 2*PI, 2*PI),
        fmod(preAngle.y > 0 ? preAngle.y : preAngle.y + 2*PI, 2*PI),
        fmod(preAngle.z > 0 ? preAngle.z : preAngle.z + 2*PI, 2*PI)
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

Vector3D AxelGyroSensor::getOffsetedAxel()
{
    return getRawAxel() - axelOffset;
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

Vector3D AxelGyroSensor::getOffsetedGyro()
{
    return getRawGyro() - gyroOffset;
}

Vector3D AxelGyroSensor::getAxelAngle(Vector3D rawAxel)
{
    const Vector3D axel = rawAxel * (9.80665 / 16384);
    return Vector3D(
        atan(axel.y / sqrt(axel.x * axel.x + axel.z * axel.z)),
        atan(-1 * axel.x / sqrt(axel.y * axel.y + axel.z * axel.z)),
        atan2(rawAxel.y, rawAxel.x)
    );
}

Vector3D AxelGyroSensor::getGyroAngle(Vector3D rawGyro)
{
    return rawGyro * ((1.0 / 32.8) * (float(sampleMicros) / 1000000.0));
}
