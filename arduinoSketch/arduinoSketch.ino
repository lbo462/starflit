#include <Wire.h>
#include "src/RescueBot.h"

RescueBot bot = RescueBot();


void setup() {
  Serial.begin(9600);  // Serial communication
  Wire.begin();  // I2C communication

  bot.setup();
}

void loop() {
  bot.update();

  delay(10);
}
