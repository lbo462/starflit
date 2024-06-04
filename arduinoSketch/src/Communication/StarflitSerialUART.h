#ifndef starflit_serial_h
#define starflit_serial_h

#include <Arduino.h>
#include <SoftwareSerial.h>


/**
 * Communication through the serial RX / TX pins.
 * The ways it's wired, there's multiple devices connected to the default pins.
 * This is embarrassing ...
 * One should be careful when using multiple sub-classes since they're all be
 * reading / writing on the same channel and create interferences when created with the default pins.
 * 
 * The default pins are RX = 0 and TX = 1
 */
class StarflitSerialUART
{
    /** Speed for the communication */
    int speed;

    /**
     * These are the defaults pins for the serial communication
     * for any Arduino microprocessor.
     * The default pins are RX = 0 and TX = 1
     */
    const int RX = 0, TX = 1;


    public:
        StarflitSerialUART();
        ~StarflitSerialUART();

        /** To be called during global set-up */
        void setup();

        /**
         * Send a buffer content by writing it on the channel
         * @param buf Message to transmit
         * @return True if the payload was delivered successfully false if not
         */
        bool send(const char *buf);

    private:
        /** Serial object to write on the serial channel. */
        SoftwareSerial serial = SoftwareSerial(RX, TX);


};

#endif