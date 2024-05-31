



#define INVERSE_MOTOR_DIRECTION 

#ifdef INVERSE_MOTOR_DIRECTION  

#define MOTOR_RIGHT_PLUS 10
#define MOTOR_RIGHT_MINUS 9
#define MOTOR_LEFT_PLUS 6
#define MOTOR_LEFT_MINUS 5

#else

#define MOTOR_RIGHT_PLUS 9  // = PB1 = 0C1A
#define MOTOR_RIGHT_MINUS 10  // = PB2 = OC1B
#define MOTOR_LEFT_PLUS 5  // = PD5  = OC0B
#define MOTOR_LEFT_MINUS 6  // = PD6 = OC0A

#endif



void set_up_motors() {

  pinMode(MOTOR_RIGHT_PLUS, OUTPUT);
  pinMode(MOTOR_RIGHT_MINUS, OUTPUT);

  digitalWrite(MOTOR_RIGHT_PLUS, LOW);  
  digitalWrite(MOTOR_RIGHT_MINUS, LOW);   


  pinMode(MOTOR_LEFT_MINUS, OUTPUT);
  pinMode(MOTOR_LEFT_PLUS, OUTPUT);

  digitalWrite(MOTOR_LEFT_MINUS, LOW);  
  digitalWrite(MOTOR_LEFT_PLUS, LOW);  



}



void moteur_droit_en_avant()
{
  digitalWrite(MOTOR_RIGHT_PLUS, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(MOTOR_RIGHT_MINUS, LOW);   // turn the LED on (HIGH is the voltage level)
}

void moteur_droit_en_arriere()
{
  digitalWrite(MOTOR_RIGHT_PLUS, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(MOTOR_RIGHT_MINUS, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void moteur_droit_stop()
{
  digitalWrite(MOTOR_RIGHT_PLUS, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(MOTOR_RIGHT_MINUS, LOW);   // turn the LED on (HIGH is the voltage level)
}



void moteur_gauche_en_avant()
{
  digitalWrite(MOTOR_LEFT_MINUS, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(MOTOR_LEFT_PLUS, LOW);   // turn the LED on (HIGH is the voltage level)
}

void moteur_gauche_en_arriere()
{
  digitalWrite(MOTOR_LEFT_MINUS, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(MOTOR_LEFT_PLUS, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void moteur_gauche_stop()
{
  digitalWrite(MOTOR_LEFT_MINUS, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(MOTOR_LEFT_PLUS, LOW);   // turn the LED on (HIGH is the voltage level)
}



void stop_motors()
{
 moteur_droit_stop();
 moteur_gauche_stop();
}



void va_tout_droit()
{
 moteur_gauche_en_avant();
 moteur_droit_en_avant();
}

void va_en_arriere()
{
 moteur_gauche_en_arriere();
 moteur_droit_en_arriere();
}


void tourne_a_droite_normal()
{
 moteur_gauche_en_avant();
 moteur_droit_stop();
}

void tourne_a_droite_sur_place()
{
 moteur_gauche_en_avant();
 moteur_droit_en_arriere();
}

void tourne_a_gauche_normal()
{
 moteur_droit_en_avant();
 moteur_gauche_stop();
}

void tourne_a_gauche_sur_place()
{
 moteur_droit_en_avant();
 moteur_gauche_en_arriere();
}



