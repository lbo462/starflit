




long lecture_echo; 
long cm;

//#define ECHO PINC7
#define ECHO 2
#define TRIG1 8
#define TRIG2 7
//#define TRIG PC6


void set_up_ultrasonic() {

 pinMode(TRIG1, OUTPUT); 
 digitalWrite(TRIG1, LOW); 

 pinMode(TRIG2, OUTPUT); 
 digitalWrite(TRIG2, LOW); 


 
 pinMode(ECHO, OUTPUT); 
 digitalWrite(ECHO, LOW); 

}



unsigned int get_distance_in_cm(char pin_trig)
{
 digitalWrite(ECHO, LOW); 
 pinMode(ECHO, INPUT); 
 digitalWrite(pin_trig, HIGH); 
 delayMicroseconds(10); 
 digitalWrite(pin_trig, LOW); 
 lecture_echo = pulseIn(ECHO, HIGH);
 pinMode(ECHO, OUTPUT); // ECHO is used as output for RCB_LEDS_WS2812, so we put it back as output
 return(lecture_echo / 58);

}

unsigned int get_forward_distance_in_cm(void)
{
 return(get_distance_in_cm(TRIG1));
 /*
 digitalWrite(ECHO, LOW); 
 pinMode(ECHO, INPUT); 
 digitalWrite(TRIG2, HIGH); 
 delayMicroseconds(10); 
 digitalWrite(TRIG2, LOW); 
 lecture_echo = pulseIn(ECHO, HIGH);
 pinMode(ECHO, OUTPUT); // ECHO is used as output for RCB_LEDS_WS2812, so we put it back as output
 return(lecture_echo / 58);
*/
}

unsigned int get_backward_distance_in_cm(void)
{
  return(get_distance_in_cm(TRIG2));
/*
digitalWrite(ECHO, LOW); 
 pinMode(ECHO, INPUT); 
 digitalWrite(TRIG1, HIGH); 
 delayMicroseconds(10); 
 digitalWrite(TRIG1, LOW); 
 lecture_echo = pulseIn(ECHO, HIGH);
 pinMode(ECHO, OUTPUT); // ECHO is used as output for RCB_LEDS_WS2812, so we put it back as output
 return(lecture_echo / 58);
*/
}
