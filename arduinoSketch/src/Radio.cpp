#include "Radio.h"


Radio::Radio() {}

Radio::~Radio() {}

void Radio::setup()
{
    radio.begin();
    radio.openWritingPipe(address);
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