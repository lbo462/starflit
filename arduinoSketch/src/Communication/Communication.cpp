#include "Communication.h"


Communication::Communication(CommModules modules)
{
    activatedModules = modules;
}

Communication::~Communication() {}

void Communication::setup()
{
    for(int i = 0; i < activatedModules.size(); i++)
    {
        CommunicationModule mod = activatedModules[i];

        switch (mod)
        {
        case CommunicationModule::all:
            radio.setup();
            break;

        case CommunicationModule::radio:
            radio.setup();
            break;

        case CommunicationModule::bluetooth:
            // TODO
            break;
        
        default:
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
        case CommunicationModule::all:
            /** Nothing to update here ... */
            break;
        
        default:
            break;
        }
    }
}

bool Communication::send(const void *buf, byte len, CommModules modules)
{
    /** Whether the message was delivered or not. */
    bool sent = true;

    for(int i = 0; i < modules.size(); i++)
    {
        CommunicationModule mod = modules[i];

        switch (mod)
        {
        case CommunicationModule::all:
            sent = sent && radio.send(buf, len) && true;
            break;

        case CommunicationModule::radio:
            sent = sent && radio.send(buf, len);
            break;
        
        default:
            sent = false;  /// Unknown comm module
            break;
        }
    }
    return sent;
}

const void *Communication::recv(byte len, CommunicationModule module)
{
    /** TODO */
}