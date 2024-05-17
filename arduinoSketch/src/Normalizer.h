#ifndef normalizer_h
#define normalizer_h

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

class Normalizer {

    public:
        Normalizer();
        ~Normalizer();

        const float g = 9.80665;

        void normalizeGyro(gyroscope_raw gyroscope);

        void normalizeAcc(accelerometer_raw accelerometer); 
};

#endif