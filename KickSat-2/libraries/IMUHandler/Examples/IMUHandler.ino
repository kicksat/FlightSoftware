/*
 * IMUHandler test - tests the IMU, reads data from the IMU every 5 seconds and prints to Serial Monitor
 */

#include "IMUHandler.h"
#include <Wire.h>
#include <SPI.h>

IMUHandle IMU;
Counter readIMUTimer;
float imuData[9];

void readIMU() {
  IMU.read(imuData); // Read IMU data
  // delay(1000);
  // SerialUSB.println();
  for(int i = 0; i < 9; i++) { // Print to Serial IMU data
    SerialUSB.print(i);SerialUSB.print(": ");SerialUSB.println(imuData[i]);
  }
  SerialUSB.println();
}

void setup() {
  SerialUSB.begin(115200); // Start SerialUSB
  while(!SerialUSB); // Wait for serial USB port to open
  SerialUSB.println("Serial intialized");
  SerialUSB.println("Delay for 5 seconds");
  delay(5000); // Delay is neccesary because of the use of sleep mode
  if(!IMU.begin()){ // Initialize imu)
    SerialUSB.println("IMU Could Not Be Intialized");
  } else {
    SerialUSB.println("IMU Intialized");
  }
  readIMUTimer.init(5); // timer delay, seconds
}

void loop() {
  if (readIMUTimer.check()) { readIMU(); }
}
