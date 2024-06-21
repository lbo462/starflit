#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Create an RF24 object
RF24 radio(7, 8); // CE, CSN pins (adjust these as per your wiring)

// Address for the RF24 communication
const byte address[6] = {0x1C,0xCE,0xCC,0xCE,0xCC};

void setup() {
  // Start the serial communication
  Serial.begin(115200);
  
  // Initialize the RF24 module
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  // Will be execute only once:
  char buf[] = {
    0x02,  // STX byte
    0x01,  // Says the object was found
    0x03   // ETX byte
  };

  Serial.print("Sent ");
  Serial.print(buf, HEX)
  radio.write(&buf, sizeof(buf));
}

void loop() {
  // do nothing
}
