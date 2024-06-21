/// @addtogroup Arduino
/// @{
#ifndef rescue_bot_h
#define rescue_bot_h

#include <Arduino.h>
#include <Wire.h>
//#include <Adafruit_NeoPixel.h>

#include "SmartMotors/SmartMotors.h"
#include "UltrasonicSensors.h"
#include "Communication/Communication.h"
#include "FrameParser.h"
<<<<<<< HEAD
#include "CameraPosition.h"
=======
#include "Leds.h"
>>>>>>> e0bbb29 (led class)

/** Time between two scans */
#define SCAN_INTERVAL 5000


/**
 * Main class that holds the logic and glue every modules to control the robot efficiently.
 * One should call `setup()` once and `update()` at each frame
 */
class RescueBot 
{
    public:
        RescueBot();
        ~RescueBot();

        /** Smart motors instance to make the robot move in a smart way */
        SmartMotors smartMotors = SmartMotors();

        /** Ultrasonic sensors used to detect incoming collisions */
        UltrasonicSensors ultrasonicSensors = UltrasonicSensors();

        /** LED strip on top of the robot used to provide feedback on the state of the robot. */
        Leds ledStrip = Leds();

        /** Serial communication */
        Communication<CommunicationModule::serialModule> serial = Communication<CommunicationModule::serialModule>();

        /** Radio communication */
        Communication<CommunicationModule::radioModule> radio = Communication<CommunicationModule::radioModule>();

        /**
         * Set-up the pins mode.
         * To be called on global setup.
         */ 
        void setup();

        /**
         * Updates the robot to make it move.
         * To be called at each frame.
         */
        void update();

        /** Scan the environnement to see if there are obstacles out of its detection cone */
        void scan();

        /**
         * Tells whether we're currently scanning or not.
         * Returns the value of the private attribute `scanning`.
         * We don't want this attribute to be public since we don't want it modified outside this class.
         * Instead, we define it private and allow reading write through this method.
         * This makes the `scanning` attribute read-only.
         */
        inline bool isScanning() { return scanning; }

        /** Avoid any collision by making some wiggy-jiggy moves */
        void collisionAvoidance();

        /**
         * Make random moves to explore the space.
         * Aware of collisions and won't break walls !
         */
        void explore();

        /**
         * Controls the leds on top of the robot.
         */
        //void leds();

    private:
        /** Tells if the RPI was initialized. */
        bool RPIInitialized = false;

        /** Tells if the object was found. */
        bool objectFound = false;

        /**
         * Tells whether we're currently scanning or not.
         * See `isScanning()` for more details.
         */
        bool scanning = false;

        /** Previous scan timing in milliseconds */
        unsigned long previousScan = 0;

        /**
         * Gets rewritten during the scan and tells if
         * the robot did scan on right or left. 
         */
        bool scannedRight = false, scannedLeft = false;

        /** Parses the received frames from the Communication module */
        FrameParser parser = FrameParser();

        /** Configures the camera positions */
        CameraPosition camPosition = CameraPosition();
        
        /** Params to control the LEDs */
        const int ledPin=2, numLeds=33, brightness=50;
        
        /** Is an object detected or not \'O'/ */
        bool objectDetected = false;
        
};

#endif
/// @}