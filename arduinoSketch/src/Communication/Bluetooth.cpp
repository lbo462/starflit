#include "Bluetooth.h"


Bluetooth::Bluetooth() {}

Bluetooth::~Bluetooth() {}

void Bluetooth::setup()
{
    serial.begin(BT_BAUD);
}

bool Bluetooth::send(const char *msg)
{
    return serial.print(msg) > 0;
}