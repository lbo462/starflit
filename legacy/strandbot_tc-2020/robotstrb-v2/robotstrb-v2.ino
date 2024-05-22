#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#include <time.h>
//#include <Adafruit_NeoPixel.h>

// RJO ::
#define SIZE 35
#define PAUSE 0
#define VITESSE_SERIE 9600
#define SYSTERROR 0

#define pas_pwm 10
#define VITESSE_SERIE 9600
#define zMinPWM  130      //180    //S monte     zMax en bas
#define zMaxPWM  450     //480     // Z descend  zMin en haut
// Canal 1 = Horizontal
#define xMinPWM  120     //130     //D vers la droite  xMax à gauche
#define xMaxPWM  450     //500    //Q  vers la gauche  xMin à droite

/* Utilisation du capteur Ultrason HC-SR04 */
#define TEST_ULTRASONIC_SENSOR_HC06
#define TEST_TEMERATURE_PRESSURE_HUMIDITY_SENSOR_MODULE_BME280
#define TEST_ACCELEROMETER_GYROSCOPE_GY521

// AON A dummy mode in which robot moves randomly, avoiding most of obstacles. It requires user to watch after the robot.
#define DUMMY_MODE false

#define TEST_BOARD_VOLTAGE_MEASUREMENTS

void dummy_motion(void);
void patrol(void);
void radio_command_listener(void);
void serial_command_listener(void);
void command_handler(char cmd);
void start_auto_mode(void);
void stop_auto_mode(void);
void ninety_deg_left(void);
void ninety_deg_right(void);
void forward_one_tile(void);
void affiche_response(void);
void led_management(void);
void hello(void);
void feedback(bool);

bool obst;
int D[3]={int(0.5*SIZE*(1/cos(45.0*PI/180.0))*cos(15.0*PI/180.0)),
int(0.5*SIZE*(1/cos(60*PI/180.0))*cos(15*PI/180.0)), int(1.5*SIZE*cos(15*PI/180.0))};

float originAngle;
float defaultAngle;
char state='0';
char  cde[10];
float systError=0;

// AON Robot moves only when this flag is down
bool auto_mode = false;

uint16_t k = 0;

word pwm_cptx, pwm_cptz;
long t1 = 0; //utilisé par le bluetooth

bool voltage_motors_set_command = false;

void setup() {
  // AON Robot will start automatically
  auto_mode = false;

  Serial.begin(VITESSE_SERIE);    // Initialiser la communication série
  srand(time(NULL));

  setup_motion(); // Must be setup first
  
  set_up_ultrasonic();
  setup_radio();
  set_up_rgb_leds_ws2812();
  setup_pwm_pca9685();
  setup_measure_angle();

  setup_bme280();

//   setup_motors_voltage();
//   change_motors_voltage_to_saved_value();

  pwm_cptx = round((xMinPWM+xMaxPWM)/2); //300;
  pwm_cptz = round((zMinPWM+zMaxPWM)/2); //300;

  t1 = 0;
  k = 0;
}

// the loop function runs over and over again forever
void loop() {
  if(DUMMY_MODE){
    dummy_motion();
  }else{
    get_order();
    stop_robot();
  }
}

/* AON Allows Robot to fetch the last order it was given */
void get_order(void){
  // Here we simply run a listener for each medium used to reach the robot
  radio_command_listener();
  serial_command_listener();
}

/* AON Dummy algo */
void dummy_motion(void){
   int for_dist = get_forward_distance_in_cm();
   if(for_dist < 70){
     if(for_dist < 40){
       va_en_arriere();
       delay(2250);
     }
     if(rand()%2){
       tourne_a_droite_normal();
     }else{
       tourne_a_gauche_normal();
     }
     delay(2000);

   }else{
     va_tout_droit();
   }

    if(get_backward_distance_in_cm() < 15){
      stop_motors();
    }
}

/* AON Listens for commands received via radio */
void radio_command_listener(void)
{
  char commande_radio;
  if (received_something_on_radio())
  {
    commande_radio = get_received_radio_value();
    Serial.println("RADIO COMMAND READY...");
    Serial.println(commande_radio);

    command_handler(commande_radio);
  }
}

/* AON Listens for commands received via serial port */
void serial_command_listener(void){
    if ( Serial.available() > 0 )
  {
    char  cde[10];
    Serial.readBytes(cde, 1);
    command_handler(cde[0]);
  }
}

/* AON Handles commands */
void command_handler(char cmd){
  switch (cmd)
    {
      case 'A' : if(auto_mode){stop_auto_mode();}else{start_auto_mode();} break;
      case 'Z' : if(!auto_mode){pwm_cptz = pwm_cptz + pas_pwm; pwm_cptz = min(zMaxPWM, pwm_cptz); set_channel_pwm_pca9685(0, pwm_cptz); Serial.println(pwm_cptz, DEC);} break;
      case 'S' : if(!auto_mode){pwm_cptz = pwm_cptz - pas_pwm; pwm_cptz = max(zMinPWM, pwm_cptz); set_channel_pwm_pca9685(0, pwm_cptz); Serial.println(pwm_cptz, DEC);} break;
      case 'Q' : if(!auto_mode){pwm_cptx = pwm_cptx + pas_pwm; pwm_cptx = min(xMaxPWM, pwm_cptx); set_channel_pwm_pca9685(1, pwm_cptx); Serial.println(pwm_cptx, DEC);} break;
      case 'D' : if(!auto_mode){pwm_cptx = pwm_cptx - pas_pwm; pwm_cptx = max(xMinPWM, pwm_cptx); set_channel_pwm_pca9685(1, pwm_cptx); Serial.println(pwm_cptx, DEC);} break;
      case 'I' : if(!auto_mode){va_tout_droit();} break;
      case 'K' : if(!auto_mode){va_en_arriere();} break;
      case 'J' : if(!auto_mode){tourne_a_gauche_sur_place();} break;
      case 'L' : if(!auto_mode){tourne_a_droite_sur_place();} break;

      // AON Raspberry dedicated commands
      case '*' : if(auto_mode){hello();} break;
      case '.' : if(auto_mode){left(); feedback(verif_obstacles()); correction();} break;
      case '/' : if(auto_mode){right(); feedback(verif_obstacles()); correction();} break;
      case '?' : if(auto_mode){go_forward(); feedback(verif_obstacles()); correction();} break;
      case '$' : if(auto_mode){correction();} break;

      case '>' : if(auto_mode){move_camera_hard_right();} break;
      case '<' : if(auto_mode){move_camera_hard_left();} break;
      case '^' : if(auto_mode){move_camera_up();} break;
      case 'v' : if(auto_mode){move_camera_down();} break;
    }
}

/* AON Starts Robot. It will move on its own and film around automatically. */
void start_auto_mode(void){
  auto_mode = true;
  set_servo_to_initial_position();
}

/* AON Stops Robot. User will have control over motion and camera. */
void stop_auto_mode(void){
  auto_mode = false;
  stop_motors();
  set_servo_to_initial_position();
}

// RJO
bool verif_obstacles(){
  defaultAngle=calcul_angle();
  obst=false;
  pseudo_turn(defaultAngle+37.5);
  for(int i=1; i>=0; i--){
    //Serial.print("dist = ");Serial.println(get_forward_distance_in_cm());
    if(get_forward_distance_in_cm()<D[1-i]){
      obst=true;
    }
    if(i!=2){
      pseudo_inverse_turn(defaultAngle+15*i+7.5);
    }
  }
  pseudo_inverse_turn(defaultAngle);
  delay(PAUSE);
  if(get_forward_distance_in_cm()<D[2]){
      obst=true;
  }
  pseudo_turn(defaultAngle-37.5);
  for(int i=1; i>=0; i--){
    //Serial.print("dist = ");Serial.println(get_forward_distance_in_cm());
    if(get_forward_distance_in_cm()<D[1-i]){
      obst=true;
    }
    if(i!=2){
      pseudo_inverse_turn(defaultAngle-15*i-7.5);
    }
  }
  pseudo_inverse_turn(defaultAngle);
  /*if(obst){
    fullRed();
    delay(1000);
    fullBlack();
    return true;
  } else {
    return false;
  }*/
  delay(PAUSE);
}

/*repositioning(int pLeft, int pForward, int pRight){
  TODO
}*/

/* AON Sends data back to the raspberry about the reachability of the forward tile*/
void feedback(bool status){
  if(status){
      Serial.println("{\"reachability\" : true }");
   } else {
      Serial.println("{\"reachability\" : false }");
   }

}

void hello(void){
  led_management();
  Serial.println("ACK");
}

/* AON Manages LEDs */
void led_management(void){
  if ((micros() - t1) > 1000)
  {
    rainbow_pos(k);
    k++;
    if (k > 255) {
      k = 0;
    }
    t1 = micros();
  }
}

void move_camera_hard_left(){
  while(pwm_cptx < xMaxPWM){
    pwm_cptx = min(xMaxPWM, pwm_cptx + pas_pwm);
  }
}

void move_camera_hard_right(){
  while(pwm_cptx > xMinPWM){
    pwm_cptx = max(xMinPWM, pwm_cptx - pas_pwm);
  }
}

void move_camera_hard_up(){
  while(pwm_cptz > zMinPWM){
    pwm_cptz = max(zMinPWM, pwm_cptz - pas_pwm);
  }
}

void move_camera_up(){
  int i;
  for(i = 0; i < 5; ++i){
    pwm_cptz = max(zMinPWM, pwm_cptz - pas_pwm);
  }
}

void move_camera_hard_down(){
  while(pwm_cptz < zMaxPWM){
    pwm_cptz = min(zMaxPWM, pwm_cptz + pas_pwm);
  }
}

void move_camera_down(){
  int i;
  for(i = 0; i < 5; ++i){
    pwm_cptz = min(zMaxPWM, pwm_cptz + pas_pwm);
  }
}