#ifndef comm_def_h
#define comm_def_h

#include "Communication.h"

template<CommunicationModule module>
Communication<module>::Communication() {}

template<CommunicationModule module>
Communication<module>::~Communication() {}

template<CommunicationModule module>
void Communication<module>::setup()
{
    switch (module)
        {
        case CommunicationModule::radio:
            radio.setup();
            break;

        case CommunicationModule::serial:
            serial.begin(115200);
            break;

        case CommunicationModule::bluetooth:
            bluetooth.begin(38400);
            break;
        }
}

template<CommunicationModule module>
bool Communication<module>::send(const void *buf, byte len, bool asASCII)
{
    switch (module)
    {
    case CommunicationModule::radio:
        return radio.send(buf, len);
    
    case CommunicationModule::serial:
        if(asASCII)
            return serial.print((char *)buf);
        else
            return serial.write((char *)buf);

    case CommunicationModule::bluetooth:
        if(asASCII)
            return bluetooth.print((char *)buf);
        else
            return bluetooth.write((char *)buf);
    }
}

template<CommunicationModule module>
int Communication<module>::recv(char *buf, int len)
{
    unsigned long timerLength = 5000;  // milliseconds
    unsigned long timerStart = millis();

    while(millis() - timerStart < 5000)
    {
        switch (module)
        {
        case CommunicationModule::serial:
            serial.listen();  // This is undocumented on the Arduino doc but is mandatory ...
            if(serial.available())
            {
                // Says if we're currently reading a frame.
                bool readingFrame = false;

                size_t index = 0;
                while (index < len) {
                    int c = serial.read();

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
            break;

        case CommunicationModule::bluetooth:
            return -1;

        default:
            return -1;
        }
    }
    return -1;
}

template<CommunicationModule module>
template<class F>
void Communication<module>::withRecv(int maxLength, F && f)
{
    char *buf = new char[maxLength];
    int len = recv(buf, maxLength);
    if(len > 0)
    {
        // That's where the function passed in parameters is executed!
        // `buf` contains the received frame.
        f(buf);
    }
    free(buf);
}

#endif