#ifndef axelgyro_h
#define axelgyro_h

#include <Arduino.h>
#include <math.h>
#include "I2C.h"
#include "utils/Vector3D.h"


/**
 * AxelGyro stands for Accelerometer + Gyroscope
 * It's used to control the Accelerometer and the Gyroscope of the robot
 * One should call `setup()` once and `update()` at each frame
 */
class AxelGyroSensor
{
    public:
        AxelGyroSensor();
        ~AxelGyroSensor();

        /**
         * Get the angle speed of the sensor.
         * Require that the AxelGyro instance was updated beforehand
         * through the `update()` method.
         * The x-coordinate is the pitch,
         * The y-coordinate is the roll,
         * The z-coordinate is the yaw,
         */
        Vector3D angleSpeed = Vector3D(0.0, 0.0, 0.00);

        /**
         * Absolute angle of the sensor, obtained by integrating during `update()` 
         */
        Vector3D angle = Vector3D(0.0, 0.0, 0.0);

        /**
         * Absolute position of the sensor, obtained by integrating twice during `update()` 
         */
        Vector3D position = Vector3D(0.0, 0.0, 0.0);

        /**
         * Set-up the pins mode.
         * To be called on global setup.
         * Note that pins can be modified before setup if needed
         */ 
        void setup();

        /**
         * Update the sensors sense of time so that is's able
         * to correctly compute positions and speed 
         */
        void update();

    private:
        /**
         * Address of the IMU
         */
        const int imuAddr = 0x68;

        /**
         * Define the gyroscope precision
         */
        const int gyro_fs_250_dps = 0, gyro_fs_500_dps = 8, gyro_fs_1000_dps = 10, gyro_fs_2000_dps = 24;

        /**
         * Define the accelerometer precision
         */
        const int acc_fs_2g = 0, acc_fs_4g = 8, acc_fs_8g = 10, acc_fs_16g = 24;

        /**
         * Correspond to the frame duration.
         * Is updated via the method update() and is required
         * to compute angles and positions correctly
         */
        unsigned long sampleMicros, lastSampleMicros;

        /**
         * Read raw values from the sensors 
         */
        Vector3D getRawAxel(); Vector3D getRawGyro();

        /**
         * Fetch the accelerometer angles from raw axel data.
         * Used to compute the yaw roll and pitch. 
         */
        Vector3D getAxelAngle(Vector3D rawAxel);

        /**
         * Fetch the gyroscope angles from raw axel data.
         * Used to compute the yaw roll and pitch. 
         */
        Vector3D getGyroAngle(Vector3D rawGyro);
};

#endif