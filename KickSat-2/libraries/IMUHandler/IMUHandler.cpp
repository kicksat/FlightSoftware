/*
IMUHandler - Gathers and formats LSM9DS1Sensor data from the LSM9DS1 using the SparkFunLSM9DS1 lib

Last update on: 8-11-18
by: Ralen
*/

#include "IMUHandler.h"
#include "SparkFunLSM9DS1.h"
#include "RTCCounter.h"

void IMUHandle::read(float buf[]) {
  float gyroData[3];
  float accelData[3];
  float magData[3];
  getGyro(gyroData);
  getAccel(accelData);
  getMag(magData);
  buf[0] = gyroData[0];
  buf[1] = gyroData[1];
  buf[2] = gyroData[2];
  buf[3] = accelData[0];
  buf[4] = accelData[1];
  buf[5] = accelData[2];
  buf[6] = magData[0];
  buf[7] = magData[1];
  buf[8] = magData[2];
}

void IMUHandle::getGyro(float buf[]) {
  if(!LSM9DS1Sensor.gyroAvailable()) { // Init gyroscope
    SerialUSB.println("GYROSCOPE UNAVAILABALE");
    buf[0] = 0; // Store empty values into buffer
    buf[1] = 0;
    buf[2] = 0;
    return;
  }

  LSM9DS1Sensor.settings.mag.operatingMode = 1; // Single data sample mode
  timeout.start(1); // Start timeout
  while(!LSM9DS1Sensor.gyroAvailable()){
    if (timeout.triggered()) { // Checks time for timeout
      SerialUSB.println("GYROSCOPE TIMEOUT");
      buf[0] = 0; // Store empty values into buffer
      buf[1] = 0;
      buf[2] = 0;
      return;
    }
  }
  LSM9DS1Sensor.readGyro(); // Read and store new gyroscope data
  LSM9DS1Sensor.settings.mag.operatingMode = 2; // Power down

  // Store gyro data
  buf[0] = LSM9DS1Sensor.calcGyro(LSM9DS1Sensor.gx);
  buf[1] = LSM9DS1Sensor.calcGyro(LSM9DS1Sensor.gy);
  buf[2] = LSM9DS1Sensor.calcGyro(LSM9DS1Sensor.gz);
}

void IMUHandle::getAccel(float buf[]) {
  if(!LSM9DS1Sensor.accelAvailable()) { // Init accelerometer
    SerialUSB.println("ACCELEROMETER UNAVAILABALE");
    buf[0] = 0; // Store empty values into buffer
    buf[1] = 0;
    buf[2] = 0;
    return;
  }

  LSM9DS1Sensor.readAccel(); // Read and store new accelerometer data

  // Store accel data
  buf[0] = LSM9DS1Sensor.calcAccel(LSM9DS1Sensor.ax);
  buf[1] = LSM9DS1Sensor.calcAccel(LSM9DS1Sensor.ay);
  buf[2] = LSM9DS1Sensor.calcAccel(LSM9DS1Sensor.az);
}

void IMUHandle::getMag(float buf[]) {
  if(!LSM9DS1Sensor.magAvailable()) { // Init magnetometer
    SerialUSB.println("MAGNETOMETER UNAVAILABALE");
    buf[0] = 0; // Store empty values into buffer
    buf[1] = 0;
    buf[2] = 0;
    return;
  }

  LSM9DS1Sensor.readMag(); // Read and store new magnetometer data

  // Store mag data
  buf[0] = LSM9DS1Sensor.calcMag(LSM9DS1Sensor.mx);
  buf[1] = LSM9DS1Sensor.calcMag(LSM9DS1Sensor.my);
  buf[2] = LSM9DS1Sensor.calcMag(LSM9DS1Sensor.mz);
}
