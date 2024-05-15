


void randomDirection(){

  int randomNumber = random(2);
  
  if (randomNumber == 0) {
    Serial.print("Direction : turnleft \n");
    turnLeft();
    delay(2000);
    stopMotors();
  } else {
    Serial.print("Direction : turnright \n");
    turnRight();
    delay(2000);
    stopMotors();
  }
}

