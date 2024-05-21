#ifndef radio_h
#define radio_h

#include <Arduino.h>
#include <RF24.h>

/**
 * Enables read-write radio communication through the RF24 antennas
 */
class Radio
{
    public:
        Radio();
        ~Radio();

        /**
         * Set-up the radio.
         * To be called on global setup.
         */
        void setup();

        /**
         * Send a message by writing it on the pipe
         * @param msg String message to transmit
         * @param size sizeof(msg)
         * @return True if the payload was delivered successfully false if not
         */
        bool send(const char msg[], const int size);

    private:
        /**
         * Radio object from the RF24 module to handle communication with the antennas
         */
        RF24 radio = RF24(A1, A0);

        /**
         * Pipe address on which the RF communication takes place
         */
        const byte address[6] = {0x1C,0xCE,0xCC,0xCE,0xCC};
};

#endif
