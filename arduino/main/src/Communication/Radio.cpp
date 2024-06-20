#include "Radio.h"


Radio::Radio() {}

Radio::~Radio() {}

void Radio::setup()
{
    radio.begin();
    radio.openWritingPipe(address);
    radio.openReadingPipe(0, address);
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
        char buffer[len + 2] = "";
        radio.read(&buffer, len + 2);

        // If there's no STX, we poped in the middle of nowhere, so exit
        if(buffer[0] != STX)
            return -1;

        size_t index = 0;
        while (index < len) {

            // Avoid code being stuck here!
            if (buffer[index] < 0 || buffer[index] == ETX) break;

            // Continue parsing and keep that byte in the buffer
            buf[index++] = (char)buffer[index];
        }
        return index;
    }
    return -1;
}
