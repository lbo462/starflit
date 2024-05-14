


#define BT_DELAY 1000000


long tab_speed[12]={1200,2400,4800,9600,19200,38400,57600,115200,230400,460800,921600,1382400};


void change_bluetooth_serial_speed(long bluetooth_speed)
{
 String ch1,ch2;
 char n;
 switch (bluetooth_speed)
 {
  case 1200 : ch2="1";break;
  case 2400 : ch2="2";break;
  case 4800 : ch2="3";break;
  case 9600 : ch2="4";break;
  case 19200 : ch2="5";break;
  case 38400 : ch2="6";break;
  case 57600 : ch2="7";break;
  case 115200 : ch2="8";break;
  case 230400 : ch2="9";break;
  case 460800 : ch2="A";break;
  case 921600 : ch2="B";break;
  case 1382400 : ch2="C";break;
  default : ch2="4";break;
 }

ch1="AT+BAUD"+ch2;

delay(BT_DELAY / 10);
for (n=0;n<8;n++)
  {
   Serial.begin(tab_speed[n]);    // Initialiser la communication série 
   Serial.print(ch1);
    if (wait_response()!=0) 
     {
      Serial.begin(bluetooth_speed);    // Initialiser la communication série 
      Serial.println("CHANGEMENT DE VITESSE REUSSI !!!"); 
      break;
     }

  }

  
}




char wait_response(void)
{
 String chaine;
 char recu;

 recu=0;
 t1=micros();
 do
   {
    if ( Serial.available()>0 )
      {
       chaine=Serial.readString();
       //Serial.println(debug);
       if ( (chaine[0]='O') && (chaine[1]='K') ) {recu=1;} 
      }
   } while ( ( (micros()-t1)<1000000) && (recu==0) );  

 return(recu);
}







