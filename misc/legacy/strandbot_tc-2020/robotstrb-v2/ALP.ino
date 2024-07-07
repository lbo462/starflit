/*Lmat*/

boolean reachability = true;
String rs = ""; 

void move_forward(){
  //TODO  phase de déplacement physique, code de Rémi
//  Serial.println("I moved forward ");
  delay(1000);
}


void turn_left(){
  //TODO  phase de déplacement physique, code de Rémi
//  Serial.println("I moved Left ");
   delay(1000);
}


void turn_right(){
  // TODO phase de déplacement physique, code de Rémi
//  Serial.println("I moved right ");
   delay(1000);
}

void send_case_status(bool status){  // Analyse si la case en face d'elle est disponible. 
   // phase d'analyse physique, code de Rémi //Cette fonction modifie la variable reachability afin de confirmer l'état de la case
   
   if(status){
      rs = "true";
   } else {
      rs = "false";
   }
   Serial.println("{\"reachability\" : " + rs + " }"); //Envoie de l'état de la case //Forme demandé {\"reachability\" : true }"
   
} 
