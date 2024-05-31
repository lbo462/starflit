#ifndef communication_h
#define communication_h

#include <Arduino.h>
#include <Array.h>
#include "Radio.h"


/**
 * Defines all the communication modules available through instances of the `Communication` class.
 */
enum CommunicationModule {
    all = 0, /** Match all the other modules. */
    radio = 1, /** Communication through RF24 radio. */
    bluetooth = 2 /** Communication through bluetooth. */
};

/** Array of `CommunicationModule` enum originated values. */
typedef Array<CommunicationModule, sizeof(CommunicationModule)> CommModules;

/** 
 * Communication module.
 * Contains instances of class that allows external communications.
 * An instance of this class communicate via Bluetooth, radio, or other.
 */
class Communication
{
    public:
        Communication(CommModules modules);
        ~Communication();

        /** Sets-up all the internal modules of this instance, as defined on built. */
        void setup();

        /** Updates all the internal modules of this instance, as defined on built. */
        void update();

        /**
         * Send a raw buffer a given module.
         * @param buf Buffer to transmit
         * @param len sizeof(buf)
         * @param module Modules to send the buffer with.
         * Items must be members of the `CommunicationModule` array given to the constructor.
         * @return True if the payload was delivered successfully false if not.
         * If at least one module couldn't send the message, `false` is returned.
         */
        bool send(const void *buf, byte len, CommModules modules);

        /** Same as `send` but parse the string to do the lazy job you won't do. */
        inline bool sendString(String msg, CommModules modules)
            { return send(msg.c_str(), msg.length(), modules); };

        /** TODO */
        const void *recv(byte len, CommunicationModule module);

    private:
        /** List of every activated communication modules for this `Communication` instance. */
        CommModules activatedModules;

        /** Communication through RF24 radio. */
        Radio radio = Radio();

};

#endif