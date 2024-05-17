/**
 * AxelGyro stands for Accelerator + Gyroscope
 */

#ifndef axelgyro_h
#define axelgyro_h

#include <Arduino.h>
#include <MPU6050.h>


class AxelGyroSensor
{
    public:
        AxelGyroSensor();
        ~AxelGyroSensor();

        /**
         * MPU used to control the axelgyro microprocessor
         */
        MPU6050 mpu;

        /**
         * Set-up the pins mode.
         * To be called on global setup.
         * Note that pins can be modified before setup if needed
         */ 
        void setup();

    private:
};

#endif