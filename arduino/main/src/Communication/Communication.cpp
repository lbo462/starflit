#include "Communication.h"


Communication::Communication() {}

Communication::~Communication() {}

void Communication::setModules(const CommModules modules)
{
    activatedModules = modules;
}

void Communication::setup()
{
    for(int i = 0; i < activatedModules.size(); i++)
    {
        CommunicationModule mod = activatedModules[i];

        switch (mod)
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
}

void Communication::update()
{
    for(int i = 0; i < activatedModules.size(); i++)
    {
        CommunicationModule mod = activatedModules[i];

        switch (mod)
        {
        /* Nothing to update here ... */
        
        default:
            break;
        }
    }
}

bool Communication::send(const void *buf, byte len, CommModules modules, bool asASCII)
{
    /* Whether the message was delivered or not. */
    bool sent = true;

    for(int i = 0; i < modules.size(); i++)
    {
        CommunicationModule mod = modules[i];

        switch (mod)
        {
        case CommunicationModule::radio:
            sent = sent && radio.send(buf, len);
            break;

        case CommunicationModule::serial:
            if(asASCII)
                sent = sent && serial.print((char *)buf);
            else
                sent = sent && serial.write((char *)buf);
            break;
        
        case CommunicationModule::bluetooth:
            if(asASCII)
                sent = sent && bluetooth.print((char *)buf);
            else
                sent = sent && bluetooth.write((char *)buf);
            break;
        }

        // Add a small delay to wait for the sending to be done
        delay(10);
    }
    return sent;
}

const byte *Communication::recv(CommunicationModule module, byte len)
{
    unsigned long timerLength = 5000;  // milliseconds
    unsigned long timerStart = millis();
    byte *buf;

    while(millis() - timerLength < 5000)
    {
        String m1;
        switch (module)
        {
        case CommunicationModule::serial:
            serial.listen();

            if(serial.available())
            {
                m1 = "Available !";
                radio.send(m1.c_str(), m1.length());

                serial.readBytesUntil(EOT, buf, len);

                m1 = "Received ";
                m1 += String((char *)buf);
                radio.send(m1.c_str(), m1.length());
                
                return buf;
            }
            break;

        case CommunicationModule::bluetooth:
            if(serial.available())
            {
                serial.readBytesUntil(EOT, buf, len);
                return buf;
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
}