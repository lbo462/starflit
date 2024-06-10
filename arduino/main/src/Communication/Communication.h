/// @addtogroup Arduino
/// @{
#ifndef communication_h
#define communication_h

#include <Arduino.h>
#include <Array.h>
#include <SoftwareSerial.h>
#include "Radio.h"

#define STX 0x02
#define ETX 0x03

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
         * Mimics the behaviour of python's context manager.
         * Receives a message, give to the wrapped function and free the buffers.
         * 
         * Here's an how to use it:
         * 
         * ```
         * serial.withRecv(
         *   RECEIVED_FRAME_LENGTH, [&](char *frame) {
         *       // do stuff with the frame object
         *   }
         * );
         * ```
         * 
         * Note that the signature `[&](char *frame)` should be respected!
         * The frame object is a list of char (aka unsigned byte) that were gotten
         * from the module, between a STX and ETX byte.
         * These bytes (STX and ETX) are not included in the frame.
         * This frame can be read and parsed by the `FrameParser`.
         * 
         * Plus, the wrapped function won't be called if no frame was received.
         * But it will still block the code executive during the timer of the `recv()` function.
         * 
         * WARNING !
         * This function is only able to receive a frame squished between an STX and an ETX.
         * If the STX byte is not received, nothing else will be!
         * 
         * @param maxLength Maximum length to receive.
         * @param f The function to execute, as described above.
         * Please, provide with the expected length of the message.
         */
        template<class F>
        void withRecv(int maxLength, F && f);

    private:
        /** Communication through RF24 radio. */
        Radio radio = Radio();

        /** Bluetooth communication. */
        SoftwareSerial bluetooth = SoftwareSerial(0, 1);  // RX, TX

        /** Standard serial communication. Same as `Serial`, but a bit more professional. */
        SoftwareSerial serial = SoftwareSerial(0, 1);  // RX, TX

        /**
         * Receive data from the given module.
         * 
         * THIS IS PRIVATE!
         * Instead, use `withRecv()`.
         * 
         * In order to stop, this functions waits to receive a EOT.
         * Plus, to avoid block coding for too long when nothing is received,
         * this function features a timer of 5 seconds that will end the listening
         * and return nothing.
         * @param buf Filled with a frame of bytes received from the module.
         * @param len Max length to read.
         * But will stop if EOT if encountered before.
         * @return The length of the frame received. -1 if nothing received
         */
        int recv(char *buf, int len);

};

#include "CommunicationDefinitions.h"

#endif

/// @}