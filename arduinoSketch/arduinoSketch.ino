#include <Wire.h>
#include "src/RescueBot.h"

RescueBot bot = RescueBot();


void setup() {
  Serial.begin(115200);  // Serial communication
  while(!Serial)  // Wait for Serial to start its communication
    delay(10);

  Wire.begin();  // I2C communication

  bot.setup();
}

void loop() {
  bot.update();

  delay(10);
}
