

#include <Wire.h>



#define MAX5434_VREG_COMMAND 17  //write ti volatile register ; wiper position update
#define MAX5434_NVREG_COMMAND 33  //write to non volatile register ; wiper position unchanged
#define MAX5434_NVREGXVREG_COMMAND 97  //byte transfert from non volatile register to volatile register ; wiper position update
#define MAX5434_VREGXNVREG_COMMAND 81  //byte transfert from volatile register to non volatile register



//                           5  6   7  8  9 10 11  12 V
const char voltage_array[8]={24,16,11, 7, 5, 3, 1, 0 };

 
float get_battery_voltage(void)
{
 unsigned int valeur;
 valeur = analogRead(A6);
 //return(valeur * (5.0 / 1023.0));
 Serial.print("voltage batterie = ");Serial.println(valeur * (5.0 / 1023.0));
}


float get_motors_voltage(void)
{
 unsigned int valeur;
 valeur = analogRead(A7);
 return(3.0*valeur * (5.0 / 1023.0));
}


void setup_motors_voltage()
{
   Wire.begin(); // join i2c bus
}

void change_motors_voltage(char voltage_value)
{
 if ( (voltage_value>=5) && (voltage_value<=12) )
   {
    Wire.beginTransmission(40); // Adress of Max5434 with L suffix (Max5434LEZT+T)                         
    Wire.write(MAX5434_VREG_COMMAND);             // sends value byte  
    Wire.write(voltage_array[voltage_value-5]<<3);             // sends value byte  
    Wire.endTransmission();     // stop transmitting 
   
   }
}



void save_motors_voltage()
{
 Wire.beginTransmission(40); // Adress of Max5434 with L suffix (Max5434LEZT+T)                         
 Wire.write(MAX5434_VREGXNVREG_COMMAND);             // sends value byte  
 Wire.write(0);             // 
 Wire.endTransmission();     // stop transmitting  
}


void change_motors_voltage_to_saved_value()
{
 Wire.beginTransmission(40); // Adress of Max5434 with L suffix (Max5434LEZT+T)                         
 Wire.write(MAX5434_NVREGXVREG_COMMAND);             // sends value byte  
 Wire.write(0);             // 
 Wire.endTransmission();     // stop transmitting  
}
