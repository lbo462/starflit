#include "StarflitSerialUART.h"


StarflitSerialUART::StarflitSerialUART() {}

StarflitSerialUART::~StarflitSerialUART() {}

void StarflitSerialUART::setup()
{
    serial.begin(speed);
}

bool StarflitSerialUART::send(const char *msg)
{
    return serial.print(msg) > 0;
}