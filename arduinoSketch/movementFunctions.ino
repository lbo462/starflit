/* Tis file contains all the basic movements function to help moving the robot around
 */

// Stop all the motors
void stopMotors() {
  digitalWrite(MOTOR_RIGHT_PLUS_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_MINUS_PIN, LOW);
  digitalWrite(MOTOR_LEFT_PLUS_PIN, LOW);
  digitalWrite(MOTOR_LEFT_MINUS_PIN, LOW);

  isGoingForward = false;
  isGoingBackward = false;
  isTurningRight = false;
  isTurningLeft = false;
}

/* Sets a speed for all of the motors
 * Requires Wire.begin()
 * Works by writing 
 * As weird as it sounds, the higher the value is, 
 * the lower the motors will run :/
 */
void setMotorsSpeed(int value) {
  Wire.beginTransmission(40);
  Wire.write(17);
  Wire.write(value);
  Wire.endTransmission();
}

/* Makes the robots turning right until something else stops it
 * The "turning right" is from the point of view of "going forward"
 * "going forward" is going away from the dev cable port.
 */
void turnRight() {
  digitalWrite(MOTOR_RIGHT_PLUS_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_MINUS_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_PLUS_PIN, LOW);
  digitalWrite(MOTOR_LEFT_MINUS_PIN, HIGH);

  isGoingForward = false;
  isGoingBackward = false;
  isTurningRight = true;
  isTurningLeft = false;
}

/* Makes the robots turning left until something else stops it
 * The "turning left" is from the point of view of "going forward"
 * "going forward" is going away from the dev cable port.
 */
void turnLeft() {
  digitalWrite(MOTOR_RIGHT_PLUS_PIN, HIGH);
  digitalWrite(MOTOR_RIGHT_MINUS_PIN, LOW);
  digitalWrite(MOTOR_LEFT_PLUS_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_MINUS_PIN, LOW);

  isGoingForward = false;
  isGoingBackward = false;
  isTurningRight = false;
  isTurningLeft = true;
}

/* Makes the robots going foward until something else stops it
 * "going forward" is going away from the dev cable port.
 */
void goForward() {
  //collisionAvoidance
  if(collisionDetection(isGoingForward, isGoingBackward, isTurningRight, isTurningLeft)){
    collisionAvoidance(isGoingForward, isGoingBackward, isTurningRight, isTurningLeft);
  }

  digitalWrite(MOTOR_RIGHT_PLUS_PIN, HIGH);
  digitalWrite(MOTOR_RIGHT_MINUS_PIN, LOW);
  digitalWrite(MOTOR_LEFT_PLUS_PIN, LOW);
  digitalWrite(MOTOR_LEFT_MINUS_PIN, HIGH);

  isGoingForward = true;
  isGoingBackward = false;
  isTurningRight = false;
  isTurningLeft = false;
}

/* Makes the robots going backward until something else stops it
 * "going backward" is going in the direction of the dev cable port.
 */
void goBackward() {

  //collisionAvoidance
  if(collisionDetection(isGoingForward, true, isTurningRight, isTurningLeft)){
    collisionAvoidance(isGoingForward, true, isTurningRight, isTurningLeft);
  }

  digitalWrite(MOTOR_RIGHT_PLUS_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_MINUS_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_PLUS_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_MINUS_PIN, LOW);

  isGoingForward = false;
  isGoingBackward = true;
  isTurningRight = false;
  isTurningLeft = false;
}

/* Make the unit to have a little dance
 * Pretty useless but fun
 */
void dance() {
  turnRight();
  delay(200);
  turnLeft();
  delay(200);
  stopMotors();
}
