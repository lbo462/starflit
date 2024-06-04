#ifndef bluetooth_h
#define bluetooth_h

#include <Arduino.h>
#include <SoftwareSerial.h>

/**
 * Speed of the bluetooth communication.
 * Note that the Serial communication takes place on the default
 * pins (0 and 1).
 * Hence, one shall not write at that speed !
 * It's reserved for the Bluetooth module.
 */
#define BT_BAUD 38400


/** Allows Bluetooth communication through the HC-06 module */
class Bluetooth
{
    public:
        Bluetooth();
        ~Bluetooth();

        void setup();

        /**
         * Send a buffer content by writing it on the bluetooth pipe
         * @param buf Message to transmit
         * @return True if the payload was delivered successfully false if not
         */
        bool send(const char *buf);

    private:
        /** Serial object to write on the bluetooth channel. */
        SoftwareSerial serial = SoftwareSerial(0, 1);

};


#endif