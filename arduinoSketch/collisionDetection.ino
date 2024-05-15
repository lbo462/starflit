






// Fonction permettant de savoir si le bot a un objet devant ou derrière lui en utilisant les ultrasons
bool collisionDetection(bool WF, bool WB, bool WR, bool WL){

  unsigned int forward_distance = 0;
  unsigned int backward_distance = 0;

  forward_distance=get_forward_distance_in_cm();
  backward_distance=get_backward_distance_in_cm();
  if(forward_distance<30 && WF){
    Serial.print("Collision avant détectée \n");
    return true;
  }
  if(backward_distance<30 && WB){
    Serial.print("Collision arrière détectée \n");
    return true;
  }
  return false;
}

// Fonction permettant de prendre une décision après qu'une collision soit détectée
void collisionAvoidance(bool WF,bool WB, bool WR, bool WL){
  Serial.print("Collision avoidance \n");
  stopMotors();
  delay(2000);
  if(collisionDetection(isGoingForward, isGoingBackward, isTurningRight, isTurningLeft)){
    Serial.print("Appel de randomDirection \n");
    randomDirection();
  }

}

