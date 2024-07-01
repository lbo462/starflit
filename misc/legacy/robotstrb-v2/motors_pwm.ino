



#define INVERSE_MOTOR_DIRECTION 

//#ifdef INVERSE_MOTOR_DIRECTION  

#define MOTOR_RIGHT_PLUS_PIN 10 // = PB2 = OC1B
#define MOTOR_RIGHT_MINUS_PIN 9 // = PB1 = 0C1A
#define MOTOR_LEFT_PLUS_PIN 6   // = PD6 = OC0A
#define MOTOR_LEFT_MINUS_PIN 5  // = PD5  = OC0B


#define MOTOR_RIGHT_PLUS_PWM_VALUE  OCR1BL
#define MOTOR_RIGHT_MINUS_PWM_VALUE OCR1AL
#define MOTOR_LEFT_PLUS_PWM_VALUE   OCR0A
#define MOTOR_LEFT_MINUS_PWM_VALUE  OCR0B



/*
#else

#define MOTOR_RIGHT_PLUS 9  // = PB1 = 0C1A
#define MOTOR_RIGHT_MINUS 10  // = PB2 = OC1B
#define MOTOR_LEFT_PLUS 5  // = PD5  = OC0B
#define MOTOR_LEFT_MINUS 6  // = PD6 = OC0A

#endif
*/


void set_up_motors_pwm() {
/*
  pinMode(MOTOR_RIGHT_PLUS_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_MINUS_PIN, OUTPUT);

//  digitalWrite(MOTOR_RIGHT_PLUS, LOW);  
//  digitalWrite(MOTOR_RIGHT_MINUS, LOW);   


  pinMode(MOTOR_LEFT_MINUS_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_PLUS_PIN, OUTPUT);

//  digitalWrite(MOTOR_LEFT_MINUS, LOW);  
//  digitalWrite(MOTOR_LEFT_PLUS, LOW);  

*/

   pinMode(MOTOR_RIGHT_PLUS_PIN, OUTPUT);   // = PB2 = OC1B
   pinMode(MOTOR_RIGHT_MINUS_PIN, OUTPUT);  // = PB1 = 0C1A
   TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
   TCCR1B = _BV(WGM12) | _BV(CS10); // No prescaling Fpwm=16MHZ/256= 62500 KhZ
   MOTOR_RIGHT_PLUS_PWM_VALUE=0;
   MOTOR_RIGHT_MINUS_PWM_VALUE=0;
   OCR1BH=0;
   OCR1AH=0;

   //OCR1A = 0; //MOTOR_RIGHT_MINUS
   //OCR1B = 0; //MOTOR_RIGHT_PLUS


   pinMode(MOTOR_LEFT_PLUS_PIN, OUTPUT);  // = PD6 = OC0A
   pinMode(MOTOR_LEFT_MINUS_PIN, OUTPUT);  // = PD5  = OC0B
   TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
   TCCR0B = _BV(CS00);  // No prescaling Fpwm=16MHZ/256= 62500 KhZ
   MOTOR_LEFT_PLUS_PWM_VALUE=0;
   MOTOR_LEFT_MINUS_PWM_VALUE=0;
   
//   OCR0A = 0; // MOTOR_LEFT_PLUS
 //  OCR0B = 0; //MOTOR_LEFT_MINUS 




/*


  pinMode(3, OUTPUT);
   pinMode(11, OUTPUT);
   TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
   TCCR2B = _BV(CS22);
   OCR2A = 180;
   OCR2B = 50; 
*/

}



void motor_left_pwm_value(int value)
{
  //value=min(value,255);
  //value=max(value,-255);
  
#ifdef INVERSE_MOTOR_DIRECTION  
  if (value>0) 
    {
     MOTOR_LEFT_PLUS_PWM_VALUE=(unsigned char) (value);
     MOTOR_LEFT_MINUS_PWM_VALUE=0;
    }
  else
    {
     MOTOR_LEFT_PLUS_PWM_VALUE=0;
     MOTOR_LEFT_MINUS_PWM_VALUE=(unsigned char) (-value);
    }
#else
  if (value<0)
    {
     MOTOR_LEFT_PLUS_PWM_VALUE=(unsigned char) (-value);
     MOTOR_LEFT_MINUS_PWM_VALUE=0;
    }
  else
    {
     MOTOR_LEFT_PLUS_PWM_VALUE=0;
     MOTOR_LEFT_MINUS_PWM_VALUE=(unsigned char) (value);
    }
   
#endif


    
}


void motor_right_pwm_value(int value)
{
  value=min(value,255);
  value=max(value,-255);

 //      MOTOR_RIGHT_PLUS_PWM_VALUE= (value);
  //   MOTOR_RIGHT_MINUS_PWM_VALUE=0;
//OCR1BL=value;
//OCR1AL=0;


#ifdef INVERSE_MOTOR_DIRECTION  
  if (value>0) 
    {
     MOTOR_RIGHT_PLUS_PWM_VALUE= (value);
     MOTOR_RIGHT_MINUS_PWM_VALUE=0;
    }
  else
    {
     MOTOR_RIGHT_PLUS_PWM_VALUE=0;
     MOTOR_RIGHT_MINUS_PWM_VALUE=(unsigned char) (-value);
    }
#else
  if (value<0) 
    {
     MOTOR_RIGHT_PLUS_PWM_VALUE=(unsigned char) (-value);
     MOTOR_RIGHT_MINUS_PWM_VALUE=0;
    }
  else
    {
     MOTOR_RIGHT_PLUS_PWM_VALUE=0;
     MOTOR_RIGHT_MINUS_PWM_VALUE=(unsigned char) (value);
    }

#endif

}





void motor_pwm_values_right_and_left(int right_value,int left_value)
{
 motor_right_pwm_value(right_value);
 motor_left_pwm_value(left_value);  
}




