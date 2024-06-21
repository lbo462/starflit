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
        // Raw buffer read from the radio
        char receivedBuf[len + 2] = "";
        radio.read(&receivedBuf, len + 2);

        // If there's no STX at the start of our buffer, we poped in the middle of nowhere, so exit
        if(receivedBuf[0] != STX)
            return -1;

        size_t index = 0;
        while (index < len) {
            // Exit when one find an ETX byte (which is not included in the frame)
            if (receivedBuf[index] == ETX) break;

            // Continue parsing and keep a byte in the buffer (frame)
            buf[index++] = (char)receivedBuf[index];
        }
        return index;
    }
    return -1;
}
