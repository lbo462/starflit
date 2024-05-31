#include "Bluetooth.h"


Bluetooth::Bluetooth() {}

Bluetooth::~Bluetooth() {}

void Bluetooth::setup()
{
    serial.begin(38400);
}

bool Bluetooth::send(const char *msg)
{
    return serial.write(msg) > 0;
}