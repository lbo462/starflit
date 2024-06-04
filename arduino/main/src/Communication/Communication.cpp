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

const void *Communication::recv(byte len, CommunicationModule module)
{
    /* TODO */
}