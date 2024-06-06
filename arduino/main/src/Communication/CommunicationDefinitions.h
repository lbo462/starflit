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
    radio.setup();
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
const byte *Communication<module>::recv(byte len)
{
    unsigned long timerLength = 5000;  // milliseconds
    unsigned long timerStart = millis();
    char buf[len];

    String m;

    while(millis() - timerStart < 5000)
    {
        switch (module)
        {
        case CommunicationModule::serial:
            serial.listen();

            if(serial.available())
            {
                size_t index = 0;
                while (index < len) {
                    int c = serial.read();
                    m = "Got : ";
                    m += (char)c;
                    radio.send(m.c_str(), m.length());

                    if (c < 0 || c == EOT) break;
                    buf[index] = (char)c;
                    index++;
                }

                m = "Over : ";
                m += buf;
                radio.send(m.c_str(), m.length());
                return buf;
            }
            break;

        case CommunicationModule::bluetooth:
            if(serial.available())
            {
                //serial.readBytesUntil(EOT, buf, len);
                //return buf;
            }
            break;
        }

        /*
         * Return the buf without cutting the last byte
         * because we reached the max len and no EOT was to be seen.
         */
        if(sizeof(buf) >= len)
        {
            return buf;
        }
    }
    return NULL;
}

#endif