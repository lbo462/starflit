#include <Wire.h>
#include "src/RescueBot.h"

RescueBot bot = RescueBot();


void setup() {
  /*
   * We're definitely not going to start Serial communication since
   * since that would has unexpected behaviour with the Communication serial module.
   * 
   * This is the way it happens because we're actually using the serial to communicate with a RPi.
   * ... and this is done through the Communication serial module.
   * 
   * Life's hard but the path is short.
   * 
   * PS: You can still use the `radio` module for your debugging issues :)
   */

  /*
   * Still, one might want to develop here, when the RPi isn't there.
   * That's why the line to right below, and you can uncomment it,
   * 
   * ************ AS FAR AS YOU COMMENT IT BACK ! *************** 
   *
   *  Thank you!
   */
  //Serial.begin(115200);

  Wire.begin();  // I2C communication

  bot.setup();
}

void loop() {
  bot.update();

  delay(10);
}
