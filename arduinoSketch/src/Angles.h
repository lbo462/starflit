#ifndef angles_h
#define angles_h
#include "Normalizer.h"


struct angle {
  float x, y, z = 0;
};

angle position;

class Angles {

    public:
        Angles();
        ~Angles();

        Normalizer normalizer = Normalizer();

        angle calculateAccelerometerAngles();

        angle calculateGyroscopeAngles(unsigned long sampleMicros);

        void detectPitch(angle gyroscope, angle accelerometer);

        double getPitch();

        void detectRoll(angle gyroscope, angle accelerometer);

        double getRoll();

        void detectYaw(angle gyroscope, angle accelerometer);

        double getYaw();
};

#endif