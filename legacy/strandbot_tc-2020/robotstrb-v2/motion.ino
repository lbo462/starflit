/* AON All motion-related functions */

#include <PID_v1.h>

#define DEFSPEEDG 80
#define DEFSPEEDD 70
#define ROTATIONG 120 //90
#define ROTATIOND 80 //56
#define KP 4
#define KI 4
#define KD 0.5
#define MOVETIME 3000
#define STIME 4
#define ERRORVALUE 3.0

double setPoint;
double outputVal;
double angle;
double currentAngle;
float timeOffset;
int speedG=DEFSPEEDG;
int speedD=DEFSPEEDD;
double timeWait;

PID myPID(&angle, &outputVal, &setPoint, KP, KI, KD, DIRECT);

// AON
void setup_motion(){
    set_up_motors(); // must be the first one to set_up !!! or some glitch on motor at start_up... !!!
    myPID.SetMode(AUTOMATIC);
    currentAngle = calcul_angle();
}

//RJO---------------------------------

void go_forward(){
  setPoint = currentAngle;
  //setPoint = calcul_angle();
  //speedG=DEFSPEEDG;
  //speedD=DEFSPEEDD;
  timeOffset=millis();
  moteur_droit_en_avant(speedD);
  moteur_gauche_en_avant(speedG);

  while(millis()-timeOffset<MOVETIME){
    angle = calcul_angle();
    myPID.Compute();
    speedD=min(outputVal,150);
    moteur_droit_en_avant(speedD);
    moteur_gauche_en_avant(speedG);
  }

  stop_robot();

  delay(PAUSE);
}

void go_backward(){
  setPoint = calcul_angle();
  //speedG=DEFSPEEDG;
  //speedD=DEFSPEEDD;
  timeOffset=millis();
  moteur_droit_en_arriere(speedD);
  moteur_gauche_en_arriere(speedG);
  while(millis()-timeOffset<MOVETIME){
    angle = calcul_angle();
    myPID.Compute();
    speedD=min(outputVal,150);
    moteur_droit_en_arriere(speedD);
    moteur_gauche_en_arriere(speedG);
  }

  stop_robot();

  delay(PAUSE);
}

void left(){
  currentAngle=currentAngle+89;
  turn(currentAngle);
//   Serial.print("gauche ok");
  delay(PAUSE);
}

void right(){
  currentAngle=currentAngle-89;
  turn(currentAngle);
//   Serial.print("droite ok");
  delay(PAUSE);
}

void turn(float myAngle){
  angle=calcul_angle();
  if(angle-myAngle>0){
    while(angle>myAngle){
      moteur_gauche_en_avant(ROTATIONG);
      moteur_droit_en_arriere(ROTATIOND);
      delay(10);
      stop_motors();
      delay(STIME);
      angle=calcul_angle();
    }
  } else {
    while(angle<myAngle){
      moteur_gauche_en_arriere(ROTATIONG);
      moteur_droit_en_avant(ROTATIOND);
      delay(10);
      stop_motors();
      delay(STIME);
      angle=calcul_angle();
    }
  }
}

void correction(){
  delay(PAUSE);
  timeWait=millis();
  while(millis()-timeWait<500){
    calcul_angle();
  }
  //systError+=SYSTERROR;
  //currentAngle-=systError;
  angle=calcul_angle();
  Serial.print("angle = "); Serial.println(angle);
  Serial.print("currentAngle = "); Serial.println(currentAngle);

  if(abs(angle-currentAngle)>ERRORVALUE){
    turn(currentAngle);
  }
  delay(PAUSE);
}

void pseudo_turn(float myAngle){
   angle = calcul_angle();
   if(angle-myAngle>0){
    while(angle>myAngle){
      moteur_gauche_en_avant(ROTATIONG);
      delay(10);
      stop_motors();
      delay(STIME);
      angle=calcul_angle();
    }
   } else {
      while(angle<myAngle){
        moteur_droit_en_avant(ROTATIOND);
        delay(10);
        stop_motors();
        delay(STIME);
        angle=calcul_angle();
      }
   }
}

void pseudo_inverse_turn(float myAngle){
   angle = calcul_angle();
   if(angle-myAngle>0){
      while(angle>myAngle){
        moteur_droit_en_arriere(ROTATIOND);
        delay(10);
        stop_motors();
        delay(STIME);
        angle=calcul_angle();
      }
   } else {
      while(angle<myAngle){
        moteur_gauche_en_arriere(ROTATIONG);
        delay(10);
        stop_motors();
        delay(STIME);
        angle=calcul_angle();
      }
   }
}

void stop_robot(){
  stop_motors();
  calcul_angle();
  //Serial.print("angle = "); Serial.println(calcul_angle());
  //delay(1000);
//   initC=false;
}
