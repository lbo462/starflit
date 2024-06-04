#ifndef communication_h
#define communication_h

#include <Arduino.h>
#include <Array.h>
#include <SoftwareSerial.h>
#include "Radio.h"

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

/** Number of element of the `CommunicationModule` enum. */
const int COMM_MODULES_AMOUNT = 3;

/** Array of `CommunicationModule` enum originated values. */
typedef Array<CommunicationModule, COMM_MODULES_AMOUNT> CommModules;

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
 */
class Communication
{
    public:
        /**
         * Due to Arduino array limitation (or maybe a lack of skill),
         * we're unable to create this class with a `CommModules` as parameter.
         * Hence, one should add the modules by hand by calling `setModules()`.
         * 
         * If one wants to dive with me in the details, let's talk.
         * In fact, we do are capable of creating instance class with `CommModules` as parameters
         * since that's what we're doing with `setModules()`.
         * But what's we're unable to do is to create instances of this class as we wished in
         * the `*.h` files because that would imply to create Array instance in the passed
         * parameters for this constructor.
         * Instead, we prefer to call a function such as `setModules()` in the `*.cpp` files,
         * just before calling `setup()`.
         * And just because you took time to read this, here's a cute ASCII art :
         * 
         *  ╱|、
         * (˚ˎ 。7  
         * |、˜〵          You're nice <3
         * じしˍ,)ノ
         */
        Communication();
        ~Communication();

        /** Sets-up all the internal modules of this instance, as defined on built. */
        void setup();

        /**
         * Sets the modules for this instance.
         * Does not setup the instances.
         * Therefore, one should call this method before calling `setup()`
         */
        void setModules(const CommModules modules);

        /** Updates all the internal modules of this instance, as defined on built. */
        void update();

        /**
         * Send a raw buffer a given module.
         * @param buf Buffer to transmit
         * @param len sizeof(buf)
         * @param module Modules to send the buffer with.
         * Items must be members of the `CommunicationModule` array given to the constructor.
         * @param asASCII Send bytes as text. Default is false
         * @return True if the payload was delivered successfully false if not.
         * If at least one module couldn't send the message, `false` is returned.
         */
        bool send(const void *buf, byte len, CommModules modules, bool asASCII = false);

        /** Same as `send` but parse the string and send as ASCII to do the lazy job you won't do. */
        inline bool sendString(String msg, CommModules modules)
            { return send(msg.c_str(), msg.length(), modules, true); };

        /** TODO */
        const void *recv(byte len, CommunicationModule module);

    private:
        /** List of every activated communication modules for this `Communication` instance. */
        CommModules activatedModules;

        /** Communication through RF24 radio. */
        Radio radio = Radio();

        /** Bluetooth communication. */
        SoftwareSerial bluetooth = SoftwareSerial(0, 1);  // RX, TX

        /** Standard serial communication. Same as `Serial`, but a bit more professional. */
        SoftwareSerial serial = SoftwareSerial(0, 1);  // RX, TX

};

#endif