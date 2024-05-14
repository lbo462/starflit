#include <Wire.h>
#include "states.h"

#define MOTOR_RIGHT_PLUS_PIN 10
#define MOTOR_RIGHT_MINUS_PIN 9
#define MOTOR_LEFT_PLUS_PIN 6
#define MOTOR_LEFT_MINUS_PIN 5


void setup() {
  Serial.begin(9600);  // Serial communication
  Wire.begin();  // I2C communication

  // Setup PINs mode for the motors pin
  pinMode(MOTOR_RIGHT_PLUS_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_MINUS_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_PLUS_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_MINUS_PIN, OUTPUT);

  // Simple sanitasize check, cut off the motors
  stopMotors();

  // Set an initial pretty low speed
  setMotorsSpeed(192);
}

void loop() {
  dance();
  delay(2000);
}
