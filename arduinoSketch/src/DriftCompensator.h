#ifndef drift_compensator_h
#define drift_compensator_h

#include <cstdint>

struct gyroscope_raw {
  int16_t x, y, z;
} gyroscope;

struct accelerometer_raw {
  int16_t x, y, z;
} accelerometer;

struct {
  struct {
    float x, y, z;
  } accelerometer, gyroscope;
} normalized;

struct angle {
  float x, y, z = 0;
};

class DriftCompensator {
    public:
        DriftCompensator();
        ~DriftCompensator();

        const int gyro_fs_250_dps = 0;
        const int gyro_fs_500_dps = 8;
        const int gyro_fs_1000_dps = 10;
        const int gyro_fs_2000_dps = 24;

        const int acc_fs_2g = 0;
        const int acc_fs_4g = 8;
        const int acc_fs_8g = 10;
        const int acc_fs_16g = 24;

        const float g = 9.80665;

        /**
         * Does the basic setup of gyro and acc.
         * Needs to be called on global setup.
        */
        void setup();

        bool readSample();
};

#endif