/// @addtogroup Arduino
/// @{
#ifndef camera_h
#define camera_h

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

/**
 * Utility to control the servo motors holding the camera
 */
class CameraPosition
{
    public:
        CameraPosition();
        ~CameraPosition();

        /** Configure to default position. */
        void setup();

        /**
         * Set the vertical position, on the z-axis.
         * The position is constrained between zMin and zMax values.
         */
        void setVertical(int position);

        /**
         * Set the horizontal position, on the x-axis.
         * The position is constrained between xMin and xMax values.
         */
        void setHorizontal(int position);

        /** Sets the servos to their initial position. */
        void setDefault();

    private:
        /** PWN controller for the servos. */
        Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

        /** Channel for the servo motor doing vertical motion. */
        int verticalChannel = 0;

        /** Channel for the servo motor doing horizontal motion. */
        int horizontalChannel = 1;

        /** Lowest pwm position in z. */
        int zMin = 110;

        /** Lowest pwm position in x. */
        int xMin = 120;

        /** Max pwm position in z. */
        int zMax = 400;

        /** Max pwm position in x. */
        int xMax = 460;

        /** Get the default z position, for the vertical servo */
        inline int defaultZPosition() { return (zMin+zMax)/2; }

        /** Get the default x position, for the horizontal servo */
        inline int defaultXPosition() { return (xMin+xMax)/2; }
};

#endif
/// @}