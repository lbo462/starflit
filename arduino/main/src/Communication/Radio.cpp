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

int Radio::recv(char *buf, int len)
{
    // Make the radio to work as a receiver
    radio.startListening();

    if(radio.available())
    {
        // Read a single char from the buffer ...
        // ... to check for a STX
        char *smallBuffer;
        radio.read(&smallBuffer, 1);
        char c = (char)smallBuffer;

        // If there's no STX, we poped in the middle of nowhere, so exit
        if(c != STX)
            return -1;

        size_t index = 0;
        while (index < len) {
            // Read a single char from the buffer
            char *smallBuffer;
            radio.read(&smallBuffer, 1);
            char c = (char)smallBuffer;

            // Avoid code being stuck here!
            if (c < 0 || c == ETX) break;

            // Continue parsing and keep that byte in the buffer
            buf[index++] = (char)c;
        }
        return index;
    }
    return -1;
}
