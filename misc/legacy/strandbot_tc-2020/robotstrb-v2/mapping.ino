/*Lmat*/
#define tile_length  35 
String orientation = ""; // True or false 


void affichage_inclusion_test(){
  Serial.print("test d'inclusion");

}

void forward_tile_status(){
  if(get_forward_distance_in_cm()< tile_length ){
     Serial.println("{\"reachability\" : true }");
     //va_tout_droit();         va tout droit associé au distance de nos cases
  }else{
    direction_choice();
    Serial.println("{\"reachability\" : false , \"left\" : " + orientation + " }");
    
  }
}

void direction_choice(){
  if(rand()%2){
        tourne_a_droite_normal(); // devrait devenir tourne a droite adapté à nos angle de 90°
        orientation = "false";
        
      }else{
        tourne_a_gauche_normal();
        orientation = "true";
      }
      delay(2250);                
}
