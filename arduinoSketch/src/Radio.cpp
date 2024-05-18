#include "Radio.h"


Radio::Radio()
{
    /**
     * Function constructor is moved to the method setup()
     * This is not for no reason, please, leave it that way
     * `setup()` should be called during global setup
     */
}

Radio::~Radio()
{

}

void Radio::setup()
{
    // Begin communication
    radio.begin();

    // Write address
    radio.openWritingPipe(address);

    // Set power level threshold
    radio.setPALevel(RF24_PA_MIN);
}

bool Radio::send(const void *buf, byte len)
{
    // Make the radio to work as an emitter
    radio.stopListening();

    // Send the message
    return radio.write(buf, len);
}

bool Radio::sendString(String msg)
{
    const char *buf = msg.c_str();
    return send(buf, msg.length());
}