


#include  <SPI.h>
#include  <RF24.h>
//#include  "packet.h"

//#define RADIO_EN_EMISSION

#define TEST_RADIO_1

//#define TEST_RADIO_2


#define DATA_TRANSMIT_LENGTH_IN_BYTES 3

typedef struct payload_t
{ //uint8_t src_node ;
  //uint8_t dest_node ;
  //uint16_t pkt_id ;
  //unsigned long time ;
  char data[DATA_TRANSMIT_LENGTH_IN_BYTES]; 
} payload_t;



//RF24 radio(PC1,PC0); 
#ifdef TEST_RADIO_1
RF24 radio(A1,A0); 
RF24 radio2(A3,A2);
#else
//#ifdef TEST_RADIO_2
RF24 radio(A3,A2); 
RF24 radio2(A1,A0);
//#endif

#endif
 


uint8_t address[6] = {0x1C,0xCE,0xCC,0xCE,0xCC};   // Adresse du pipe
uint8_t address2[6] = {0x1C,0xCE,0xCC,0xCE,0x22};   // Adresse du pipe


int rcv_val = 0;

payload_t rcv_payload;
byte response=1;

//long val;

void setup_radio() {


digitalWrite(A0, HIGH);  
digitalWrite(A1, HIGH);  
digitalWrite(A2, HIGH);  
digitalWrite(A3, HIGH);  

pinMode(A0, OUTPUT);
pinMode(A1, OUTPUT);
pinMode(A2, OUTPUT);
pinMode(A3, OUTPUT);



 radio.begin();
 radio.setChannel(108);
 //radio.setDataRate(RF24_1MBPS);
 //radio.setDataRate(RF24_2MBPS);
 radio.setDataRate(RF24_250KBPS);
 radio.enableDynamicPayloads();

 #ifdef RADIO_EN_EMISSION
 radio.openWritingPipe(address);    // Ouvrir le Pipe en Ã©criture
 radio.stopListening();
 #else
 radio.openReadingPipe(0,address); // Ouvrir le Pipe en lecture
 radio.startListening();
 #endif


 radio2.begin();
 radio2.setChannel(115);
 //radio.setDataRate(RF24_1MBPS);
 //radio.setDataRate(RF24_2MBPS);
 radio2.setDataRate(RF24_250KBPS);
 radio2.enableDynamicPayloads();

 #ifdef RADIO_EN_EMISSION
 radio2.openWritingPipe(address2);    // Ouvrir le Pipe en Ã©criture
 radio2.stopListening();
 #else
 radio2.openReadingPipe(0,address2); // Ouvrir le Pipe en lecture
 radio2.startListening();
 #endif



}


bool received_something_on_radio(){
return(radio.available());
}



char get_received_radio_value()
{
 radio.read( &rcv_payload, sizeof(payload_t) );
 return(rcv_payload.data[0]);
}




void send_radio(char value)
{
  rcv_payload.data[0]=value;
  radio.write(&rcv_payload,1); 
  radio2.write(&rcv_payload,1); 
}

void send_string_radio(String  chaine)
{
 int i;
 for (i=0;i<chaine.length();i++)
   {
    send_radio(chaine[i]);
    
   }

   send_radio(13);
   
}

