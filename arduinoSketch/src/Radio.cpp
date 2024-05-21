#include "Radio.h"


Radio::Radio()
{
    /**
     * Function constructor is moved to the method setup()
     * This is not for no reason, please, leave it that way
     * `setup()` should be called during global setup
     */
}

Radio::~Radio()
{

}

void Radio::setup()
{
    // Begin communications
    radio.begin();
    //antenna2.begin();

    // Write addresses
    radio.openWritingPipe(address);
    //antenna2.openWritingPipe(address2);

    // Set power level threshold
    radio.setPALevel(RF24_PA_MIN);

    // Set communication rate
    // antenna1.setDataRate(RF24_250KBPS);
    //antenna2.setDataRate(RF24_250KBPS);

    // Set channels
    // antenna1.setChannel(channel1);
    //antenna2.setChannel(channel2);

    // Set dynamic payload
    //antenna1.enableDynamicPayloads();
    //antenna2.enableDynamicPayloads();
}

bool Radio::send(const char msg[], const int size)
{
    // Turn radios to emitters
    radio.stopListening();
    // antenna2.stopListening();

    // Send the message
    return radio.write(msg, size);
    // antenna2.write(&msg, sizeof(msg));
}