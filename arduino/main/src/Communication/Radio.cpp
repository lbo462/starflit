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
    radio.startListening();
    if(radio.available())
    {
        bool readingFrame = false;

        size_t index = 0;
        while (index < len) {
            radio.read(&buf, len);

            // Search for the STX byte to start the reading
            if(!readingFrame && c == STX)
            {
                readingFrame = true;
                continue;  // Continue to avoid adding the STX byte to the frame
            }


            // Avoid code being stuck here!
            if (c < 0 || (c == ETX && readingFrame)) break;

            // Continue parsing iff we're in the frame
            // Otherwise, continue searching for the STX byte
            if(readingFrame)
                buf[index++] = (char)c;
        }
        return index;
}
