#include <printf.h>
#include "Wire.h"
#include "I2C.h"

//int val=0;
char flag_droit = 0;
char flag_gauche = 0;
String debug;

#define INTERVAL_MS_PRINT 300

extern bool mpu_6050_present;

char duree_state = 1, state = 100;

#define VITESSE_SERIE 115200

bool voltage_motors_set_command = false;

void setup() {

  //set_up_motors_pwm();
  //Serial.end();
  Wire.begin();
  Serial.begin(VITESSE_SERIE);    // Initialiser la communication série
  delay(100);

  Serial.println("");
  Serial.println("");
  Serial.println("STRANDBEEST BOT STARTED...");

  set_up_motors(); // must be the first one to set_up !!! or some glitch on motor at start_up... !!!
  // setups MPU
  I2CwriteByte(MPU6050_IMU_ADDRESS, 25, 0x01); // Set the SRD to 1
  I2CwriteByte(MPU6050_IMU_ADDRESS, 26, 0x01); // Set the DLPF to 184HZ by default
  I2CwriteByte(MPU6050_IMU_ADDRESS, 27, GYRO_FULL_SCALE_1000_DPS); // Configure gyroscope range
  I2CwriteByte(MPU6050_IMU_ADDRESS, 28, ACC_FULL_SCALE_2G); // Configure accelerometer range
  I2CwriteByte(MPU6050_IMU_ADDRESS, 56, 0x01); // Enable interrupt pin for raw data

  setup_motors_voltage();
  change_motors_voltage_to_saved_value();
}

void loop() {

  unsigned int distance_in_cm;


  int valeur;
  float tensionx, tensiony;
  char  cde[10];
  char retour_chariot;
  float battery_voltage, motors_voltage;


unsigned long currentMillis = millis();

  readSample();

  if (currentMillis - lastPrintMillis > INTERVAL_MS_PRINT) {
    /*
    Serial.print("Pitch:\t");
    Serial.print(getPitch());
    Serial.print("\xC2\xB0"); //Print degree symbol
    Serial.println();

    Serial.print("Roll:\t");
    Serial.print(getRoll());
    Serial.print("\xC2\xB0"); //Print degree symbol
    Serial.println();

    Serial.println(gyroscope.z);
    Serial.println(gyroscope.y);
    Serial.println(gyroscope.x);

    Serial.println();
    */
      
    //Serial.print(getPitch());
    //Serial.print(",");
    //Serial.print(getRoll());
    //Serial.print(",");
    Serial.print(0);
    Serial.print(",");
    Serial.print(getYaw());
    Serial.println("");
    

    lastPrintMillis = currentMillis;
  }

  if ( Serial.available() > 0 )
  {
    Serial.readBytes(cde, 1);

    if (voltage_motors_set_command)
    {
      if  ( ( (cde[0] >= '5') && (cde[0] <= '9') ) || ( (cde[0] >= 'A') && (cde[0] <= 'C') )  )
      {
        if (cde[0] <= '9')
        {
          change_motors_voltage(cde[0] - 48);
        }
        else
        {
          change_motors_voltage(cde[0] - 55);
        }

      }

      voltage_motors_set_command = false;
      cde[0] = 0;
    }



    switch (cde[0])
    {
      case '8' : va_tout_droit(); flag_gauche = 0; flag_droit = 0; break;
      case '2' : va_en_arriere(); flag_gauche = 0; flag_droit = 0; break;
      case '5' : stop_motors(); flag_gauche = 0; flag_droit = 0; break;
      case '6' : if (flag_droit == 0) {
          tourne_a_droite_normal();
          flag_droit = 1;
        } else {
          tourne_a_droite_sur_place();
        } break;
      case '4' : if (flag_gauche == 0) {
          tourne_a_gauche_normal();
          flag_gauche = 1;
        } else {
          tourne_a_gauche_sur_place();
        } break;

      case 'Y' : va_tout_droit(); flag_gauche = 0; flag_droit = 0; break;
      case 'B' : va_en_arriere(); flag_gauche = 0; flag_droit = 0; break;
      case ' ' : stop_motors(); flag_gauche = 0; flag_droit = 0; break;
      case 'H' : if (flag_droit == 0) {
          tourne_a_droite_normal();
          flag_droit = 1;
        } else {
          tourne_a_droite_sur_place();
        } break;
      case 'G' : if (flag_gauche == 0) {
          tourne_a_gauche_normal();
          flag_gauche = 1;
        } else {
          tourne_a_gauche_sur_place();
        } break;

      case 'V' : voltage_motors_set_command = true; break;
      case 'R' : save_motors_voltage(); break;
      case 'N' : change_motors_voltage_to_saved_value(); break;
    }
  }
}
