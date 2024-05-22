#include  <SPI.h>
#include  <RF24.h>

#define DATA_TRANSMIT_LENGTH_IN_BYTES 3

typedef struct payload_t
{ //uint8_t src_node ;
  //uint8_t dest_node ;
  //uint16_t pkt_id ;
  //unsigned long time ;
  char data[DATA_TRANSMIT_LENGTH_IN_BYTES]; 
} payload_t;


RF24 radio(9,10); 
//RF24 radio(A3,A2); 

uint8_t address[6] = {0x1C,0xCE,0xCC,0xCE,0xCC};   // Adresse du pipe


int rcv_val = 0;

payload_t payload;
byte response=1;

long val;

void setup_radio() {

 radio.begin();
 radio.setChannel(108);
 //radio.setDataRate(RF24_1MBPS);
 //radio.setDataRate(RF24_2MBPS);
 radio.setDataRate(RF24_250KBPS);
  
  radio.setRetries(15,15);
  
  radio.enableDynamicPayloads();
  
  radio.openWritingPipe(address);    // Ouvrir le Pipe en écriture
  radio.stopListening(); 
}


bool received_something_on_radio(){
 return(radio.available());
}



bool envoi_radio(char value){
  payload.data[0]=value; 
  return(radio.write(&payload,sizeof(payload_t)));   
}


char get_received_radio_value()
{
 radio.read( &payload, sizeof(payload_t) );
 return(payload.data[0]);
}

