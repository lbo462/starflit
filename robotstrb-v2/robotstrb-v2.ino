
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


bool mode_demo_servo;
bool voltage_motors_set_command = false;

void affiche_response(void)
{
  String debug;

  t1 = micros();
  do
  {
    if ( Serial.available() > 0 )
    {
      debug = Serial.readString();
      Serial.println(debug);

    }
  } while ( ( (micros() - t1) < 1000000)  );

}



void setup() {
  Serial.begin(VITESSE_SERIE);    // Initialiser la communication série

  Serial.println("");
  Serial.println("");
  Serial.println("STRANDBEEST BOT STARTED...");

  set_up_motors(); // must be the first one to set_up !!! or some glitch on motor at start_up... !!!
  set_up_ultrasonic();
  setup_radio();
  set_up_rgb_leds_ws2812();
  setup_pwm_pca9685();
  setup_bme280();
  setup_mpu6050();
  setup_motors_voltage();
  change_motors_voltage_to_saved_value();

  pwm_cptx = round((xMinPWM+xMaxPWM)/2); //300;
  pwm_cptz = round((zMinPWM+zMaxPWM)/2); //300;

  t1 = 0;
  t2 = 0;
  k = 0;

  mode_demo_servo = false;

}

void radio_commande_bot() {

  // Déclaration des variables locales
  char commande_radio;

  // Vérifie si quelque chose est reçu sur la radio
  if (received_something_on_radio()) {
    // Récupère la valeur reçue sur la radio
    commande_radio = get_received_radio_value();

    // Affiche la commande radio reçue
    Serial.println("RADIO COMMANDE READY...");
    Serial.println(commande_radio);

    // Vérifie si une commande de réglage de tension des moteurs est en cours
    if (voltage_motors_set_command) {
      // Vérifie la validité de la commande de réglage de tension des moteurs et ajuste les tensions des moteurs en conséquence
      if (((commande_radio >= '5') && (commande_radio <= '9')) || ((commande_radio >= 'A') && (commande_radio <= 'C'))) {
        // Convertit la commande de réglage de tension des moteurs en nombre et ajuste les tensions des moteurs en conséquence
        if (commande_radio <= '9') {
          change_motors_voltage(commande_radio - 48);
        } else {
          change_motors_voltage(commande_radio - 55);
        }
      }

      // Réinitialise la commande de réglage de tension des moteurs
      voltage_motors_set_command = false;
      commande_radio = 0;
    }

    // Effectue une action en fonction de la commande radio reçue
    switch (commande_radio) {
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
      case 'Z' : mode_demo_servo = false; pwm_cptz = pwm_cptz + pas_pwm; pwm_cptz = min(zMaxPWM, pwm_cptz); set_channel_pwm_pca9685(0, pwm_cptz); Serial.println(pwm_cptz, DEC); break;
      case 'S' : mode_demo_servo = false; pwm_cptz = pwm_cptz - pas_pwm; pwm_cptz = max(zMinPWM, pwm_cptz); set_channel_pwm_pca9685(0, pwm_cptz); Serial.println(pwm_cptz, DEC); break;
      case 'Q' : mode_demo_servo = false; pwm_cptx = pwm_cptx + pas_pwm; pwm_cptx = min(xMaxPWM, pwm_cptx); set_channel_pwm_pca9685(1, pwm_cptx); Serial.println(pwm_cptx, DEC); break;
      case 'D' : mode_demo_servo = false; pwm_cptx = pwm_cptx - pas_pwm; pwm_cptx = max(xMinPWM, pwm_cptx); set_channel_pwm_pca9685(1, pwm_cptx); Serial.println(pwm_cptx, DEC); break;
      case 'F' : mode_demo_servo = true; set_servo_to_initial_position(); break;
      case 'V' : voltage_motors_set_command = true; break;
      case 'R' : save_motors_voltage(); break;
      case 'N' : change_motors_voltage_to_saved_value(); break;
      case 'L' : state = 100; mode_demo_servo = false; set_servo_to_initial_position(); stop_motors(); break;
      case 'M' : state = 0; duree_state = 1; t4 = 0; break;
    }
  }
}




// La fonction loop s'exécute en boucle indéfiniment.
void loop() {
  // Déclaration des variables
  unsigned int distance_in_cm;
  int valeur;
  float tensionx, tensiony;
  char  cde[10];
  float battery_voltage, motors_voltage;
  char retour_chariot; 

  // Vérifie si des données sont disponibles sur le port série
  if (Serial.available() > 0) {
    // Lit une commande du port série
    Serial.readBytes(cde, 1);

    // Vérifie et ajuste la tension des moteurs si une commande est en cours
    if (voltage_motors_set_command) {
      // Vérifie la validité de la commande et ajuste la tension des moteurs
      if (((cde[0] >= '5') && (cde[0] <= '9')) || ((cde[0] >= 'A') && (cde[0] <= 'C'))) {
        // Convertit le caractère de commande en nombre et ajuste la tension des moteurs en conséquence
        if (cde[0] <= '9') {
          change_motors_voltage(cde[0] - 48);
        } else {
          change_motors_voltage(cde[0] - 55);
        }
      }
      // Réinitialise la commande de réglage de tension des moteurs
      voltage_motors_set_command = false;
      cde[0] = 0;
    }
  }

  // Gestion des commandes de mouvement
  switch (cde[0]) {
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
  }

  // Gestion du mode de démonstration des servomoteurs
  if (mode_demo_servo) {
    // Vérifie si une seconde s'est écoulée depuis la dernière mise à jour
    if ((micros() - t2) > 1000000) {
      t2 = micros(); // Réinitialise le temps

      // Réinitialise le compteur de position des servomoteurs si nécessaire
      if (w > 5) {
          w = 0;
      }

      // Met à jour la position des servomoteurs en fonction du compteur w
      switch (w) {
        case 0 : set_channel_pwm_pca9685(1, round((xMinPWM+xMaxPWM)/2));           set_channel_pwm_pca9685(0,zMinPWM+round((zMaxPWM-zMinPWM)/4)); break;
        case 1 : set_channel_pwm_pca9685(1, round((xMinPWM+xMaxPWM)/2));           set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
        case 2 : set_channel_pwm_pca9685(1, round((xMinPWM+xMaxPWM)/2));           set_channel_pwm_pca9685(0, zMaxPWM-round((zMaxPWM-zMinPWM)/4)); break;
        case 3 : set_channel_pwm_pca9685(1, xMinPWM+round((xMaxPWM-xMinPWM)/4));   set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
        case 4 : set_channel_pwm_pca9685(1, round((xMinPWM+xMaxPWM)/2));           set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
        case 5 : set_channel_pwm_pca9685(1, xMaxPWM-round((xMaxPWM-xMinPWM)/4));   set_channel_pwm_pca9685(0, round((zMinPWM+zMaxPWM)/2)); break;
      }
      w++;
    }
  }

  // Met à jour la variable k toutes les 1000 microsecondes
  if ((micros() - t1) > 1000) {
    k++;
    if (k > 255) {
      k = 0;
    }
    t1 = micros();
  }

  // Vérifie si une commande radio a été reçue pour le bot
  radio_commande_bot();

  // Vérifie si une seconde s'est écoulée depuis la dernière mise à jour
  if ((micros() - t3) > 1000000) {
    t3 = micros(); // Réinitialise le temps

    retour_chariot = 0; // Initialise la variable de retour à la ligne

    // Affiche les mesures du capteur BME280 si le test est activé
#ifdef TEST_TEMERATURE_PRESSURE_HUMIDITY_SENSOR_MODULE_BME280
    affiche_mesures_bme280();
    retour_chariot = 1; // Active le retour à la ligne
#endif

    // Affiche les distances mesurées par le capteur ultrasonique HC06 si le test est activé
#ifdef TEST_ULTRASONIC_SENSOR_HC06
    distance_in_cm = get_forward_distance_in_cm();
    Serial.print("Forward distance : ");
    Serial.print(distance_in_cm);
    Serial.print(" cm            ");

    distance_in_cm = get_backward_distance_in_cm();
    Serial.print("Backward distance : ");
    Serial.print(distance_in_cm);
    Serial.println(" cm ");
    retour_chariot = 1; // Active le retour à la ligne
#endif

    // Affiche les mesures du gyroscope-acceleromètre GY521 si le test est activé
#ifdef TEST_ACCELEROMETER_GYROSCOPE_GY521
    if (mpu_6050_present) {
     // affiche_mesures_gy521();  //#COMMENTE
      retour_chariot = 1; // Active le retour à la ligne
    }
#endif

    // Affiche les tensions de la batterie et des moteurs si le test est activé
#ifdef TEST_BOARD_VOLTAGE_MEASUREMENTS
    battery_voltage = get_battery_voltage();
    motors_voltage = get_motors_voltage();
    /*Serial.print("Battery voltage =  ");   //#COMMENTE
    Serial.print(battery_voltage);
    Serial.print(" V      ");
    Serial.print("Motors voltage =  ");
    Serial.print(motors_voltage);
    Serial.println(" V"); */
    retour_chariot = 1; // Active le retour à la ligne
#endif

    // Ajoute un retour à la ligne si nécessaire
    if (retour_chariot != 0) {
      Serial.println("");
    }
  }

  // Effectue une action toutes les 200 millisecondes si l'état est inférieur à 100
  if (((micros() - t4) > 200000) && (state < 100)) {
    t4 = micros(); // Réinitialise le temps
    duree_state--; // Décrémente la durée restante pour l'état actuel

    // Si la durée pour l'état actuel est terminée
    if (duree_state == 0) {
      // Effectue une action en fonction de l'état actuel
      switch (state) {
        case 0 : tourne_a_droite_sur_place(); duree_state = 10; state = 1; mode_demo_servo = false; set_servo_to_initial_position(); break;
        case 1 : va_tout_droit(); duree_state = 5; state = 2; break;
        case 2 : va_en_arriere(); duree_state = 5; state = 3; break;
        case 3 : stop_motors(); duree_state = 5; state = 4; break;
        case 4 : tourne_a_gauche_sur_place(); duree_state = 10; state = 5; break;
        case 5 : va_tout_droit(); duree_state = 5; state = 6; break;
        case 6 : va_en_arriere(); duree_state = 5; state = 7; break;
        case 7 : stop_motors(); duree_state = 30; state = 0; mode_demo_servo = true; break;
      }
    }
  }
}
