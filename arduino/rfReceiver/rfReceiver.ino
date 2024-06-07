#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = {0x1C,0xCE,0xCC,0xCE,0xCC};

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available())
  {
    char text[64] = "";
    radio.read(&text, sizeof(text));

    Serial.print("HEX > ");

    /** Print the HEX version of the msg received */
    for(int i = 0; i < sizeof(text); i++)
    {
      Serial.print(text[i], HEX);
      if(i < sizeof(text) - 1)
        Serial.print(".");
    }

    Serial.println();

    /** Print it as ASCII */
    if(String(text).length() > 0)
    {
      Serial.print(String(text).length());
      Serial.print(" char(s) > '");
      Serial.print(String(text));
      Serial.print("'");
      Serial.println();
    }

  }
}