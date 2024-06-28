#include <Wire.h>
#include "PCA9685.h"

PCA9685 pwmController;                  // Library using default Wire and default linear phase balancing scheme
// Canal 0 = Vertical Z
#define zMinPWM  130      //180    //S
#define zMaxPWM  450     //480     // Z
// Canal 1 = Horizontal X
#define xMinPWM  120     //130     //D
#define xMaxPWM  450     //500    //Q


 
void set_servo_to_initial_position(void)
{
// pwmController.setChannelPWM(0, 345);
// pwmController.setChannelPWM(1, 340);
// 0 => vertical
// 1 => horizontal
 pwmController.setChannelPWM(0, round((zMinPWM +zMaxPWM)/2) );
 pwmController.setChannelPWM(1, round((xMinPWM +xMaxPWM)/2));
}


void setup_pwm_pca9685() {
 
    Wire.begin();                       // Wire must be started first
    Wire.setClock(400000);              // Supported baud rates are 100kHz, 400kHz, and 1000kHz

    pwmController.resetDevices();       // Software resets all PCA9685 devices on Wire line

    pwmController.init(B000000);        // Address pins A5-A0 set to B000000
    pwmController.setPWMFrequency(50); // Default is 200Hz, supports 24Hz to 1526Hz

    pwmController.setChannelPWM(0, 128 << 4); // Set PWM to 128/255, but in 4096 land

    set_servo_to_initial_position();
 
    //Serial.println(pwmController.getChannelPWM(0)); // Should output 2048, which is 128 << 4
}


void set_channel_pwm_pca9685(char num_channel, word value)
{
 pwmController.setChannelPWM(num_channel, value); 
}

void test_pwm_pca9685() {
    word pwms[12];
    
//    pwms[0] = random(180, 480);  // vertical
//    pwms[1] = random(110, 520);  // horizontal
      pwms[0] = random(zMinPWM, zMaxPWM);  // vertical
      pwms[1] = random(xMinPWM, xMaxPWM);  // horizontal

/*
    pwms[1] = random(0, 4096);
    
    pwms[2] = random(0, 4096);
    pwms[3] = random(0, 4096);
    pwms[4] = random(0, 4096);
    pwms[5] = random(0, 4096);
    pwms[6] = random(0, 4096);
    pwms[7] = random(0, 4096);
    pwms[8] = random(0, 4096);
    pwms[9] = random(0, 4096);
    pwms[10] = random(0, 4096);
    pwms[11] = random(0, 4096);
    pwmController.setChannelsPWM(0, 12, pwms);
   */
   
    pwmController.setChannelPWM(0, pwms[0]);
    pwmController.setChannelPWM(1, pwms[1]);
    
    
    delay(1000);

    // Note: Only 7 channels can be written in one i2c transaction due to a
    // BUFFER_LENGTH limit of 32, so 12 channels will take two i2c transactions.
}
