

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

int speed_default = 70;

void set_up_motors() {

  pinMode(MOTOR_RIGHT_PLUS, OUTPUT);
  pinMode(MOTOR_RIGHT_MINUS, OUTPUT);

  analogWrite(MOTOR_RIGHT_PLUS, 0);  
  analogWrite(MOTOR_RIGHT_MINUS, 0);   


  pinMode(MOTOR_LEFT_MINUS, OUTPUT);
  pinMode(MOTOR_LEFT_PLUS, OUTPUT);

  analogWrite(MOTOR_LEFT_MINUS, 0);  
  analogWrite(MOTOR_LEFT_PLUS, 0);  



}



void moteur_droit_en_avant(int speed)
{
  analogWrite(MOTOR_RIGHT_PLUS, speed);   // turn the LED on (HIGH is the voltage level)
  analogWrite(MOTOR_RIGHT_MINUS, 0);   // turn the LED on (HIGH is the voltage level)
}

void moteur_droit_en_arriere(int speed)
{
  analogWrite(MOTOR_RIGHT_PLUS, 0);   // turn the LED on (HIGH is the voltage level)
  analogWrite(MOTOR_RIGHT_MINUS, speed);   // turn the LED on (HIGH is the voltage level)
}

void moteur_droit_stop()
{
  analogWrite(MOTOR_RIGHT_PLUS, 0);   // turn the LED on (HIGH is the voltage level)
  analogWrite(MOTOR_RIGHT_MINUS, 0);   // turn the LED on (HIGH is the voltage level)
}



void moteur_gauche_en_avant(int speed)
{
  analogWrite(MOTOR_LEFT_MINUS, speed);   // turn the LED on (HIGH is the voltage level)
  analogWrite(MOTOR_LEFT_PLUS, 0);   // turn the LED on (HIGH is the voltage level)
}

void moteur_gauche_en_arriere(int speed)
{
  analogWrite(MOTOR_LEFT_MINUS, 0);   // turn the LED on (HIGH is the voltage level)
  analogWrite(MOTOR_LEFT_PLUS, speed);   // turn the LED on (HIGH is the voltage level)
}

void moteur_gauche_stop()
{
  analogWrite(MOTOR_LEFT_MINUS, 0);   // turn the LED on (HIGH is the voltage level)
  analogWrite(MOTOR_LEFT_PLUS, 0);   // turn the LED on (HIGH is the voltage level)
}



void stop_motors()
{
 moteur_droit_stop();
 moteur_gauche_stop();
}



void va_tout_droit()
{
 moteur_gauche_en_avant(speed_default);
 moteur_droit_en_avant(speed_default);
}

void va_en_arriere()
{
 moteur_gauche_en_arriere(speed_default);
 moteur_droit_en_arriere(speed_default);
}


void tourne_a_droite_normal()
{
 moteur_gauche_en_avant(speed_default);
 moteur_droit_stop();
}

void tourne_a_droite_sur_place()
{
 moteur_gauche_en_avant(speed_default);
 moteur_droit_en_arriere(speed_default);
}

void tourne_a_gauche_normal()
{
 moteur_droit_en_avant(speed_default);
 moteur_gauche_stop();
}

void tourne_a_gauche_sur_place()
{
 moteur_droit_en_avant(speed_default);
 moteur_gauche_en_arriere(speed_default);
}
