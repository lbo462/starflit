/// @addtogroup Arduino
/// @{
#ifndef radio_h
#define radio_h

#include <Arduino.h>
#include <RF24.h>


/** Enables read-write radio communication through the RF24 antennas */
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
         * Send a buffer content by writing it on the radio pipe
         * @param buf Buffer to transmit
         * @param len sizeof(buf)
         * @return True if the payload was delivered successfully false if not
         */
        bool send(const void *buf, byte len);

        /**
         * Receive data from the radio.
         * In order to stop, this functions waits to receive a ETX.
         * Plus, to avoid block coding for too long when nothing is received,
         * this function features a timer of 5 seconds that will end the listening
         * and return nothing.
         * @param buf Filled with a frame of bytes received from the module.
         * @param len Max length to read.
         * But will stop if ETX if encountered before.
         * @return The length of the frame received. -1 if nothing received
         */
        int recv(char *buf, int len);

    private:
        /** Radio object from the RF24 module to handle communication with the antennas */
        RF24 radio = RF24(A1, A0);

        /** Pipe address on which the RF communication takes place */
        const byte address[6] = {0x1C,0xCE,0xCC,0xCE,0xCC};
};

#endif

/// @}