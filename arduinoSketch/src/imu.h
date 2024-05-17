#ifndef Imu_h
#define Imu_h

class Imu {
    public:
        Imu();
        ~Imu();

        const int imu_addr = 68;

        Angles angles = Angles();

        bool isReady();

        void readRaw();
};

#endif