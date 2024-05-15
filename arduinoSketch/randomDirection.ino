


void randomDirection(){

  int randomNumber = random(2);
  
  if (randomNumber == 0) {
    Serial.print("Direction : turnleft \n");
    turnLeft();
    delay(1000);
    stopMotors();
    Serial.print("randomDirection : moteurs arrêtés \n");
  } else {
    Serial.print("Direction : turnright \n");
    turnRight();
    delay(1000);
    stopMotors();
    Serial.print("randomDirection : moteurs arrêtés \n");
  }

  Serial.print("randomDirection : sortie de la fonction \n");

}

