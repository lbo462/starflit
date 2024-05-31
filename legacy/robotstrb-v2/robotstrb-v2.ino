
/*
  #include <helper_3dmath.h>
  #include <MPU6050.h>
  #include <MPU6050_6Axis_MotionApps20.h>
  #include <MPU6050_9Axis_MotionApps41.h>

  #include <MPU6050.h>
*/
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
//#include <Adafruit_NeoPixel.h>

// be careful, also defined in Pwm_PCA9685
//Canal 0 = Vertical
#define zMinPWM  130      //180    //S monte     zMax en bas
#define zMaxPWM  450     //480     // Z descend  zMin en haut
// Canal 1 = Horizontal
#define xMinPWM  120     //130     //D vers la droite  xMax à gauche
#define xMaxPWM  450     //500    //Q  vers la gauche  xMin à droite
/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/


/* Utilisation du capteur Ultrason HC-SR04 */




#define TEST_ULTRASONIC_SENSOR_HC06
#define TEST_TEMERATURE_PRESSURE_HUMIDITY_SENSOR_MODULE_BME280
#define TEST_ACCELEROMETER_GYROSCOPE_GY521





#define TEST_BOARD_VOLTAGE_MEASUREMENTS



//int val=0;
char flag_droit = 0;
char flag_gauche = 0;
String debug;


extern bool mpu_6050_present;

uint16_t k = 0, w = 0;

long t1 = 0, t2 = 0, t3 = 0, t4 = 0;

word pwm_cptx, pwm_cptz;



char duree_state = 1, state = 100;


#define pas_pwm 10;

#define VITESSE_SERIE 9600
//#define VITESSE_SERIE 115200


bool mode_demo_servo;
bool voltage_motors_set_command = false;

void affiche_response(void)
{
  String debug;
  // char recu;

  //recu=0;
  t1 = micros();
  do
  {
    if ( Serial.available() > 0 )
    {
      debug = Serial.readString();
      Serial.println(debug);
      //if ( (chaine[0]='O') && (chaine[1]='K') ) {recu=1;}
    }
  } while ( ( (micros() - t1) < 1000000)  );

  //return(recu);
}



void setup() {

  //set_up_motors_pwm();
  //Serial.end();

  Serial.begin(VITESSE_SERIE);    // Initialiser la communication série
  //delay(100);

  Serial.println("");
  Serial.println("");
  Serial.println("STRANDBEEST BOT STARTED...");

  set_up_motors(); // must be the first one to set_up !!! or some glitch on motor at start_up... !!!
  setup_pwm_pca9685();


  setup_motors_voltage();
  change_motors_voltage_to_saved_value();
  //setup_esp01s();

  pwm_cptx = round((xMinPWM+xMaxPWM)/2); //300;
  pwm_cptz = round((zMinPWM+zMaxPWM)/2); //300;

  //motor_pwm_values_right_and_left(128,128);


  //Serial.begin(9600);    // Initialiser la communication série
  //Serial.begin(VITESSE_SERIE);    // Initialiser la communication série

  //Serial.print("AT");
  //affiche_reponse();
  //Serial.print("AT+VERSION");
  //affiche_reponse();
  //Serial.print("AT+UART");
  //affiche_reponse();

  //Serial.print("AT+BAUD4");
  //delay(2000);


  //Serial.print("AT+BAUD8");
  //Serial.print("AT");
  //Serial.print("AT+VERSION");

  //Serial.println("AT+UART");
  //Serial.begin(115200);    // Initialiser la communication série




  //Serial.begin(115200);    // Initialiser la communication série

  //rainbowCycle(5);
  t1 = 0;
  t2 = 0;
  k = 0;

  //delay(1000);

  //change_bluetooth_serial_speed(VITESSE_SERIE);


  mode_demo_servo = false;


}



// the loop function runs over and over again forever
void loop() {

  unsigned int distance_in_cm;


  int valeur;
  float tensionx, tensiony;
  char  cde[10];
  char retour_chariot;
  float battery_voltage, motors_voltage;


  /*

    do
    {
    delay(3000);
    Serial.println(" cm            ");

    } while(1);
  */

  /*
    for(k=0; k<256; k++)  // 5 cycles of all colors on wheel
    {
    rainbow_pos(k);
    }
    delay(500);
  */


  //test_pwm_pca9685();



  if ( Serial.available() > 0 )
  {
    Serial.readBytes(cde, 1);
    //cde.trim();
    //Serial.println(cde);

    //send_radio(cde[0]);

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



      //flag_gauche=0;flag_droit=0;
      case 'Z' : mode_demo_servo = false; pwm_cptz = pwm_cptz + pas_pwm; pwm_cptz = min(zMaxPWM, pwm_cptz); set_channel_pwm_pca9685(0, pwm_cptz); Serial.println(pwm_cptz, DEC); break;
      case 'S' : mode_demo_servo = false; pwm_cptz = pwm_cptz - pas_pwm; pwm_cptz = max(zMinPWM, pwm_cptz); set_channel_pwm_pca9685(0, pwm_cptz); Serial.println(pwm_cptz, DEC); break;
      case 'Q' : mode_demo_servo = false; pwm_cptx = pwm_cptx + pas_pwm; pwm_cptx = min(xMaxPWM, pwm_cptx); set_channel_pwm_pca9685(1, pwm_cptx); Serial.println(pwm_cptx, DEC); break;
      case 'D' : mode_demo_servo = false; pwm_cptx = pwm_cptx - pas_pwm; pwm_cptx = max(xMinPWM, pwm_cptx); set_channel_pwm_pca9685(1, pwm_cptx); Serial.println(pwm_cptx, DEC); break;

//      case 'Z' : mode_demo_servo = false; pwm_cptz = pwm_cptz + pas_pwm; pwm_cptz = min(480, pwm_cptz); set_channel_pwm_pca9685(0, pwm_cptz); Serial.println(pwm_cptz, DEC); break;
//      case 'S' : mode_demo_servo = false; pwm_cptz = pwm_cptz - pas_pwm; pwm_cptz = max(180, pwm_cptz); set_channel_pwm_pca9685(0, pwm_cptz); Serial.println(pwm_cptz, DEC); break;
//      case 'Q' : mode_demo_servo = false; pwm_cptx = pwm_cptx + pas_pwm; pwm_cptx = min(500, pwm_cptx); set_channel_pwm_pca9685(1, pwm_cptx); Serial.println(pwm_cptx, DEC); break;
//      case 'D' : mode_demo_servo = false; pwm_cptx = pwm_cptx - pas_pwm; pwm_cptx = max(130, pwm_cptx); set_channel_pwm_pca9685(1, pwm_cptx); Serial.println(pwm_cptx, DEC); break;

      case 'F' : mode_demo_servo = true; set_servo_to_initial_position(); break;


      case 'V' : voltage_motors_set_command = true; break;
      case 'R' : save_motors_voltage(); break;
      case 'N' : change_motors_voltage_to_saved_value(); break;

      case 'L' : state = 100; mode_demo_servo = false; set_servo_to_initial_position(); stop_motors(); break;
      case 'M' : state = 0; duree_state = 1; t4 = 0; break;
    }


  }


  if (mode_demo_servo)
  {
    if ((micros() - t2) > 1000000)
    {
      
      if (w > 5) {
          w = 0;
      }
      
      switch (w)
      {
        case 0 : set_channel_pwm_pca9685(1, round((xMinPWM+xMaxPWM)/2));           set_channel_pwm_pca9685(0,zMinPWM+round((zMaxPWM-zMinPWM)/4)); break;
        case 1 : set_channel_pwm_pca9685(1, round((xMinPWM+xMaxPWM)/2));           set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
        case 2 : set_channel_pwm_pca9685(1, round((xMinPWM+xMaxPWM)/2));           set_channel_pwm_pca9685(0, zMaxPWM-round((zMaxPWM-zMinPWM)/4)); break;
//        case 3 : set_channel_pwm_pca9685(1, xMinPWM);                            set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
//        case 4 : set_channel_pwm_pca9685(1, round((xMinPWM+xMaxPWM)/2));         set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
//        case 5 : set_channel_pwm_pca9685(1, xMaxPWM);                            set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
        case 3 : set_channel_pwm_pca9685(1, xMinPWM+round((xMaxPWM-xMinPWM)/4));   set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
        case 4 : set_channel_pwm_pca9685(1, round((xMinPWM+xMaxPWM)/2));           set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
        case 5 : set_channel_pwm_pca9685(1, xMaxPWM-round((xMaxPWM-xMinPWM)/4));   set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
     
      }
      w++;
      t2 = micros();
      //Serial.println(w,DEC);


    }

  }



  if ((micros() - t1) > 1000)
  {
   // rainbow_pos(k);
    k++;
    if (k > 255) {
      k = 0;
    }
    t1 = micros();
    /*
      if (k==0)
        {
         valeur = analogRead(A7);
         tensionx = valeur * (5000 / 1023.0);
         Serial.print(tensionx);
         Serial.print("   ");
         valeur = analogRead(A6);
         tensiony = valeur * (5000 / 1023.0);
         Serial.println(tensiony);
        }
    */
  }




  if ((micros() - t1) > 1000)
  {
   // rainbow_pos(k);
    k++;
    if (k > 255) {
      k = 0;
    }
    t1 = micros();
    /*
      if (k==0)
        {
         valeur = analogRead(A7);
         tensionx = valeur * (5000 / 1023.0);
         Serial.print(tensionx);
         Serial.print("   ");
         valeur = analogRead(A6);
         tensiony = valeur * (5000 / 1023.0);
         Serial.println(tensiony);
        }
    */
  }



  //rainbowCycle(5);

  //show_rgb_leds();


  //Serial.println(val);
  //delay(1000);
  //val++;





  if ((micros() - t3) > 1000000)
  {
    t3 = micros();

    retour_chariot = 0;

  if ( ((micros() - t4) > 200000) && (state < 100)  )
  {
    t4 = micros();
    duree_state--;

    if (duree_state == 0)
    {
      switch (state)
      {
        case 0 : tourne_a_droite_sur_place(); duree_state = 10; state = 1; mode_demo_servo = false; set_servo_to_initial_position(); break;
        case 1 : va_tout_droit(); duree_state = 5; state = 2; break;
        case 2 : va_en_arriere(); duree_state = 5; state = 3; break;
        case 3 : stop_motors(); duree_state = 5; state = 4; break;

        case 4 : tourne_a_gauche_sur_place(); duree_state = 10; state = 5; break;
        case 5 : va_tout_droit(); duree_state = 5; state = 6; break;
        case 6 : va_en_arriere(); duree_state = 5; state = 7; break;
        case 7 : stop_motors(); duree_state = 30; state = 0; mode_demo_servo = true; break;



      }


      /*
             case 0 :
                       duree_state--;
                       if (duree_state==0)
                         {
                          stop_motors();
                          distance_in_cm=get_forward_distance_in_cm();
                          if (distance_in_cm>50)
                            {
                             va_tout_droit();
                             state=1;
                             duree_state=1;

                            }
                          else
                            {
                             tourne_a_droite_sur_place();
                             duree_state=1;
                            }
                         }

                       break;


             case 1 :
                       duree_state--;
                       if (duree_state==0)
                         {
                          distance_in_cm=get_forward_distance_in_cm();
                          if (distance_in_cm>50)
                            {
                             va_tout_droit();
                             duree_state=1;
                            }
                          else
                            {
                             state=0;
                             tourne_a_droite_sur_place();
                             duree_state=1;
                            }
                         }

                       break;

      */

    }

  }


  //  }
  //while (1);

  //   MOTOR_RIGHT_PLUS_PWM_VALUE=128;
  //   MOTOR_RIGHT_MINUS_PWM_VALUE=0;





  //va_en_arriere();
  /*
    motor_right_pwm_value(120);
    delay(80000);
    motor_right_pwm_value(255);
    delay(80000);
    //va_tout_droit();
    motor_right_pwm_value(0);
    delay(80000);
    motor_right_pwm_value(-120);
    delay(80000);
    motor_right_pwm_value(-255);
    delay(80000);
    motor_right_pwm_value(0);
    delay(80000);
  */
  /*
    motor_left_pwm_value(120);
    delay(80000);
    motor_left_pwm_value(255);
    delay(80000);
    //va_tout_droit();
    motor_left_pwm_value(0);
    delay(80000);
    motor_left_pwm_value(-120);
    delay(80000);
    motor_left_pwm_value(-255);
    delay(80000);
    motor_left_pwm_value(0);
    delay(80000);
  */



  /*
    motor_right_pwm_value(-120);
    delay(80000);
    //motor_right_pwm_value(0);
    //delay(2000);
    //va_tout_droit();
    //motor_right_pwm_value(0);
    //delay(80000);
    motor_right_pwm_value(0);
    delay(50000);
  */

  /*
    analogWrite(9,255);
    delay(3000);

    analogWrite(9,0);
    delay(3000);
  */

  /*
    analogWrite(9,255);
    delay(3000);

    analogWrite(9,191);
    delay(3000);

    analogWrite(9,128);
    delay(3000);

    analogWrite(9,64);
    delay(3000);

    analogWrite(9,0);
    delay(3000);

    analogWrite(9,64);
    delay(3000);

    analogWrite(9,128);
    delay(3000);

    analogWrite(9,191);
    delay(3000);



  */


  //  va_tout_droit();
  //  delay(4000);

  /*

    tourne_a_droite_normal();
    delay(1000);

    va_tout_droit();
    delay(4000);

    tourne_a_gauche_normal();
    delay(1000);

    va_tout_droit();
    delay(4000);



    tourne_a_droite_sur_place();
    delay(1000);

    va_tout_droit();
    delay(4000);

    tourne_a_gauche_sur_place();
    delay(1000);

    va_tout_droit();
    delay(4000);

  */


  /*
    digitalWrite(9, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(2000);                       // wait for a second

    digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(2000);                       // wait for a second


    digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(10, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(2000);                       // wait for a second

    digitalWrite(9, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(10, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(2000);                       // wait for a second

  */
  //delay(1000);                       // wait for a second
}
