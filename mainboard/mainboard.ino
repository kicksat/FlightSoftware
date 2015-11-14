// Kicksat 2 Main Board Code
//

#include <Wire.h>
#include <Time.h>

#include "I2Cdev.h"
#include "ITG3200.h"
#include "HMC5883L.h"

// Initialize gyro and magnetometer classes
ITG3200 gyro;
HMC5883L mag;

int16_t gx, gy, gz;
int16_t mx, my, mz;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(2000);
  
  mag.initialize();
  gyro.initialize();

// verify connection
  Serial.println("Testing device connections...");
  Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
  Serial.println("Testing device connections...");
  Serial.println(gyro.testConnection() ? "ITG3200 connection successful" : "ITG3200 connection failed");
  
  
}

void loop() {
  
  Serial.println(gyro.testConnection() ? "ITG3200 connection successful" : "ITG3200 connection failed");
  mag.getHeading(&mx, &my, &mz);
  gyro.getRotation(&gx, &gy, &gz);
  
//  Serial.print("gyro:\t");
//  Serial.print(gx); Serial.print("\t");
//  Serial.print(gy); Serial.print("\t");
//  Serial.println(gz);
//    
//  Serial.print("mag:\t");
//  Serial.print(mx); Serial.print("\t");
//  Serial.print(my); Serial.print("\t");
//  Serial.print(mz); Serial.println("\t");
    
  delay(100);
}
  
 
