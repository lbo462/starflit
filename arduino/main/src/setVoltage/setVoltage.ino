#include <Wire.h>

#define MAX5434_VREG_COMMAND 17  //write ti volatile register ; wiper position update
#define MAX5434_NVREG_COMMAND 33  //write to non volatile register ; wiper position unchanged
#define MAX5434_NVREGXVREG_COMMAND 97  //byte transfert from non volatile register to volatile register ; wiper position update
#define MAX5434_VREGXNVREG_COMMAND 81  //byte transfert from volatile register to non volatile register

//                           5  6   7  8  9 10 11  12 V
const char voltage_array[8]={24,16,11, 7, 5, 3, 1, 0 };

const int motorRP=6, motorRM=5, motorLP=10, motorLM=9;

void setup(){
    Wire.begin();
    Serial.begin(115200);
    Wire.beginTransmission(40); // Adress of Max5434 with L suffix (Max5434LEZT+T)                         
    Wire.write(MAX5434_VREG_COMMAND);             // sends value byte  
    Wire.write(voltage_array[6]<<3);             // sends value byte  
    Wire.write(MAX5434_VREGXNVREG_COMMAND);             // sends value byte  
    Wire.write(0); 
    Wire.endTransmission();     // stop transmitting 
    delay(2000);
}

void loop(){
    analogWrite(motorLM, 70) ;   
    analogWrite(motorLP, 0);   
    analogWrite(motorRP, 70);   
    analogWrite(motorRM, 0);   

    Serial.println(3.0 * analogRead(A7) * (5.0/1023.0));
}
