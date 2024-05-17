#ifndef Imu_h
#define Imu_h

#include "I2C.h"
#include <stdint.h>
#include <Arduino.h>

struct gyroscope_raw {
  int x, y, z;
} gyroscope;

struct accelerometer_raw {
  int x, y, z;
} accelerometer;

class Imu {
    public:
        Imu();
        ~Imu();

        const int imu_addr = 68;

        bool isReady();

        void readRaw();
};

#endif