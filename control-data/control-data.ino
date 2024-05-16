// -------------------------------------------------
// Copyright (c) 2023 HiBit <https://www.hibit.dev>
// -------------------------------------------------

#include "Wire.h"
#include "I2C.h"

#define MPU6050_IMU_ADDRESS 0x68

#define GYRO_FULL_SCALE_250_DPS  0x00
#define GYRO_FULL_SCALE_500_DPS  0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define ACC_FULL_SCALE_2G  0x00
#define ACC_FULL_SCALE_4G  0x08
#define ACC_FULL_SCALE_8G  0x10
#define ACC_FULL_SCALE_16G 0x18

#define TEMPERATURE_OFFSET 21 // As defined in documentation

#define INTERVAL_MS_PRINT 300

#define G 9.80665

struct gyroscope_raw {
  int16_t x, y, z;
} gyroscope;

struct accelerometer_raw {
  int16_t x, y, z;
} accelerometer;

struct temperature_raw {
  int16_t value;
} temperature;

struct {
  struct {
    float x, y, z;
  } accelerometer, gyroscope;

  float temperature;
} normalized;

struct angle {
  float x, y, z = 0;
};

angle position;

unsigned long lastPrintMillis = 0;
unsigned long lastSampleMicros = 0;
/*
void setup()
{
  Wire.begin();
  Serial.begin(115200);
  delay(100);

  // Data Output Rate = 1000 / (1 + SRD)
  // Output rate must be at least 2x DLPF rate
  I2CwriteByte(MPU6050_IMU_ADDRESS, 25, 0x01); // Set the SRD to 1
  I2CwriteByte(MPU6050_IMU_ADDRESS, 26, 0x01); // Set the DLPF to 184HZ by default
  I2CwriteByte(MPU6050_IMU_ADDRESS, 27, GYRO_FULL_SCALE_1000_DPS); // Configure gyroscope range
  I2CwriteByte(MPU6050_IMU_ADDRESS, 28, ACC_FULL_SCALE_2G); // Configure accelerometer range
  I2CwriteByte(MPU6050_IMU_ADDRESS, 56, 0x01); // Enable interrupt pin for raw data
  
  
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  
}


void loop()
{
  unsigned long currentMillis = millis();

  readSample();

  if (currentMillis - lastPrintMillis > INTERVAL_MS_PRINT) {
    
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
      
    Serial.print(getPitch());
    Serial.print(",");
    Serial.print(getRoll());
    Serial.println("");
    

    lastPrintMillis = currentMillis;
  }
}
*/
bool readSample()
{
  if (isImuReady() == false) {
    return false;
  }

  unsigned long sampleMicros = (lastSampleMicros > 0) ? micros() - lastSampleMicros : 0;

  lastSampleMicros = micros()/10000;

  readRawImu();

  normalize(gyroscope);
  normalize(accelerometer);
  normalize(temperature);

  angle accelerometer = calculateAccelerometerAngles();
  angle gyroscope = calculateGyroscopeAngles(sampleMicros);

  detectPitch(gyroscope, accelerometer);
  detectRoll(gyroscope, accelerometer);
  detectYaw(gyroscope, accelerometer);

  return true;
}
