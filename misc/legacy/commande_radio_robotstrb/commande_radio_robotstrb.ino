


void setup() {
  setup_radio();
  Serial.begin(9600);    // Initialiser la communication série 
  Serial.println("TELECOMMANDE STRANDBEAST STARTED..."); 

}

void loop(void) {
 
 
   if ( Serial.available()>0 )
  {
    char c = toupper(Serial.read());
    Serial.println(c);
    envoi_radio(c);

  }

  }

