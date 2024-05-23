#ifndef axelgyro_h
#define axelgyro_h

#include <Arduino.h>
#include <math.h>
#include <Adafruit_MPU6050.h>

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
         * Get the angle speed of the sensor through the complementary filter
         * Require that the AxelGyro instance was updated beforehand
         * through the `update()` method.
         * The x-coordinate is the pitch,
         * The y-coordinate is the roll,
         * The z-coordinate is the yaw,
         */
        Vector3D complementaryFilterOutput = Vector3D();

        /**
         * Absolute angle of the sensor, obtained by integrating during `update()` 
         */
        Vector3D angle = Vector3D();

        /**
         * Absolute position of the sensor, obtained by integrating twice during `update()` 
         */
        Vector3D position = Vector3D();

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
         * MPU used to control the axelgyro microprocessor
         */
        Adafruit_MPU6050 mpu;

        /**
         * Specific MPU sensor
         */
        Adafruit_Sensor *mpu_temp, *mpu_accel, *mpu_gyro;

        /**
         * Calibrate the sensors.
         * This function blocks execution while the calibration is not over
         * and the timer not elapsed.
         * It should be called once during `setup()`.
         * This functions sets values to the offset vectors.
         * @param allowedAccelError Error vector allowed for acceleration calibration
         * @param allowedGyroError Error vector allowed for the gyroscope calibration 
         * @param maxDelay Max amount of milliseconds to wait before stopping calibration.
         * This functions returns `1` if the calibration process was stopped by this timer,
         * meaning the calibration couldn't find offset within allowed errors.
         * @return True if the calibration could be done, false otherwise
         */
        bool calibrate(
            Vector3D allowedAccelError = Vector3D(1.0, 1.0, 1.0),
            Vector3D allowedGyroError = Vector3D(1.0, 1.0, 1.0),
            unsigned long maxDelay = 10000
        );

        /**
         * Get the mean values of the sensor.
         * This function is supposedly called when the sensor isn't moving.
         * This is called during calibration.
         * Note that the sensors reading do not take offset into computations!
         * @param bufferSize Amount of measures to take for the mean, default is 100.
         * Making it bigger makes it more precise but slower as well.
         */
        Vector3D getMeanAccel(int bufferSize = 100); Vector3D getMeanGyro(int bufferSize = 100);

        // Offset from the sensors in all directions
        Vector3D axelOffset = Vector3D(), gyroOffset = Vector3D();

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
         * Read raw values from the sensors ...
         * but substracted the offsets !
         */
        Vector3D getOffsetedAxel(); Vector3D getOffsetedGyro();

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