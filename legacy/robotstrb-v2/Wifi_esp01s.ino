//#include 
//#include 
#define TIMEOUT 5000 // mS
//#define LED 5
//SoftwareSerial mySerial(7, 6); // RX, TX
//const int button = 11;
//int button_state = 0;

/*
 #include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
*/
void setup_esp01s()
{
 /*
  * 
  *
 pinMode(LED,OUTPUT);
 pinMode(button,INPUT); 
 Serial.begin(9600);
 mySerial.begin(9600);
 */
 //delay(100000);
 //Serial.end;
 SendCommand("AT+RST", "Ready");
 delay(5000);
 SendCommand("AT+CWMODE=1","OK");
 SendCommand("AT+CIFSR", "OK");
 SendCommand("AT+CIPMUX=1","OK");
 SendCommand("AT+CIPSERVER=1,80","OK");
}
/* 
void loop(){
 button_state = digitalRead(button);
 
 if(button_state == HIGH){
    mySerial.println("AT+CIPSEND=0,23");
    mySerial.println("Button was pressed!");
    delay(1000);
    SendCommand("AT+CIPCLOSE=0","OK");
  }
  
 String IncomingString="";
 boolean StringReady = false;
 
 while (mySerial.available()){
   IncomingString=mySerial.readString();
   StringReady= true;
  }
 
  if (StringReady){
    Serial.println("Received String: " + IncomingString);
  
  if (IncomingString.indexOf("LED=ON") != -1) {
    digitalWrite(LED,HIGH);
   }
 
  if (IncomingString.indexOf("LED=OFF") != -1) {
    digitalWrite(LED,LOW);
   }
  }
 }

*/
 
boolean SendCommand(String cmd, String ack){
  Serial.println(cmd); // Send "AT+" command to module
 // send_string_radio(cmd);
  
if (!echoFind(ack)) // timed out waiting for ack string
    return true; // ack blank or ack found
}
 
boolean echoFind(String keyword){
 byte current_char = 0;
 byte keyword_length = keyword.length();
 long deadline = millis() + TIMEOUT;
 while(millis() < deadline){
  if (Serial.available()){
    char ch = Serial.read();
    send_radio(ch);
    //Serial.write(ch);
    if (ch == keyword[current_char])
      if (++current_char == keyword_length){
       //Serial.println("OK");
       return true;
    }
   }
  }
 return false; // Timed out
}
