#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Create an RF24 object
RF24 radio(7, 8); // CE, CSN pins (adjust these as per your wiring)

// Address for the RF24 communication
const byte address[6] = "00001";
const char receivingMessage[] = "receiving";
bool isReceiving = false;

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  
  // Initialize the RF24 module
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // Check if there is data available on the serial port
  if (Serial.available() > 0) {
    // Read the data from the serial port
    String data = Serial.readStringUntil('\n');
    
    // Ensure the data is not empty
    if (data.length() > 0) {
      // Convert the string to a char array
      char charArray[data.length() + 1];
      data.toCharArray(charArray, data.length() + 1);
      
      // Send the data via RF24
      if (radio.write(&charArray, sizeof(charArray))) {
        Serial.println("Data sent successfully");
      } else {
        Serial.println("Failed to send data");
      }
      
      // Send "receiving" message
      if (!isReceiving) {
        isReceiving = true;
      }
    }
  } else {
    if (isReceiving) {
      isReceiving = false;
    }
  }

  // Send "receiving" status while receiving data
  if (isReceiving) {
    if (radio.write(&receivingMessage, sizeof(receivingMessage))) {
      Serial.println("Status: receiving");
    } else {
      Serial.println("Failed to send status");
    }
  }
}
