/// @addtogroup Arduino
/// @{
#ifndef communication_h
#define communication_h

#include <Arduino.h>
#include <Array.h>
#include <SoftwareSerial.h>
#include "Radio.h"

#define EOT 0x04

/**
 * Defines all the communication modules available through instances of the `Communication` class.
 */
enum CommunicationModule {
    /** Communication through RF24 radio. */
    radio = 0,

    /**
     * Classic serial communication.
     * Note that one can't work with multiple serial module at a time.
     * This is due to the hardware wiring: all the serial are wired on the same pins ...
     */
    serial = 1,
    
    /**
     * Communication through bluetooth (bluetooth).
     * This doesn't work well with `serial`.
     */
    bluetooth = 2,
};

/** 
 * Communication module.
 * Contains instances of class that allows external communications.
 * An instance of this class communicate via Bluetooth, radio, or other.
 * 
 * About the communications through the serial RX / TX pins ...
 * The ways it's wired, there's multiple devices connected to the default pins.
 * This is embarrassing ...
 * One should be careful when using multiple sub-classes since they're all be
 * reading / writing on the same channel and create interferences when created with the default pins.
 * 
 * The default pins are __RX = 0__ and __TX = 1__ and they're used by the following modules:
 * 
 * - serial
 * - bluetooth
 * 
 *  ╱|、
 * (˚ˎ 。7  
 * |、˜〵 
 * じしˍ,)ノ
 */
template<CommunicationModule module>
class Communication
{
    public:
        Communication();
        ~Communication();

        /** Sets-up the internal module of this instance */
        void setup();

        /**
         * Send a raw buffer a given module.
         * @param buf Buffer to transmit
         * @param len sizeof(buf)
         * @param asASCII Send bytes as text. Default is false
         * @return True if the payload was delivered successfully false if not.
         */
        bool send(const void *buf, byte len, bool asASCII = false);

        /** Same as `send` but parse the string and send as ASCII to do the lazy job you won't do. */
        inline bool sendString(String msg)
            { return send(msg.c_str(), msg.length(), true); };

        /**
         * Receive data from the given module.
         * In order to stop, this functions waits to receive a EOT.
         * Plus, to avoid block coding for too long when nothing is received,
         * this function features a timer of 5 seconds that will end the listening
         * and return nothing.
         * @param buf Filled with a frame of bytes received from the module.
         * @param len Max length to read.
         * But will stop if EOT if encountered before.
         * Default is 1000 (ie, a big value).
         * @return The length of the frame received. -1 if nothing received
         */
        int recv(char *buf, byte len = 1000);

        template<class F>
        void withRecv(F && f);

    private:
        /** Communication through RF24 radio. */
        Radio radio = Radio();

        /** Bluetooth communication. */
        SoftwareSerial bluetooth = SoftwareSerial(0, 1);  // RX, TX

        /** Standard serial communication. Same as `Serial`, but a bit more professional. */
        SoftwareSerial serial = SoftwareSerial(0, 1);  // RX, TX

};

#include "CommunicationDefinitions.h"

#endif

/// @}