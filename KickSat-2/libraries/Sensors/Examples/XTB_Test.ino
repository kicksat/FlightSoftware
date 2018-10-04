#include "KickSat_Sensor.h"
#include <SPI.h>
#define Serial SerialUSB

KickSat_Sensor sensor1("xtb1");
KickSat_Sensor sensor2("xtb2");
KickSat_Sensor sensor3("xtb3");

/*---------------SETUP---------------*/
void setup() {
  Serial.begin(115200);
  delay(200);
  SPI.begin();
}

/*---------------LOOP---------------*/
void loop() {  
  
  sensor1.operate(); //operate sensor1 and automatically record data
  sensor2.operate(); //operate sensor2 and automatically record data
  sensor3.operate(); //operate sensor2 and automatically record data  
  
  // example of accessing stored sensor data and printing it out as a float
  byte dataArray1[4*4]; //data array must be 4X bigger due to floats
  sensor1.sensorData(dataArray1, 4); //read data for sensor1, data is provided as byte array (4 bytes per float!)
  for (uint8_t i = 0; i <= 4*4; i+=4) {
    Serial.println(sensor1.getFloat(dataArray1,i),8); //casting bytes back in to float
  }
  byte dataArray2[7*4]; //data array must be 4X bigger due to floats
  sensor2.sensorData(dataArray2, 7); //read data for sensor1, data is provided as byte array (4 bytes per float!)
  for (uint8_t i = 0; i <= 7*4; i+=4) {
    Serial.println(sensor1.getFloat(dataArray2,i),8); //casting bytes back in to float
  }
  byte dataArray3[9*4]; //data array must be 4X bigger due to floats
  sensor3.sensorData(dataArray3, 9); //read data for sensor1, data is provided as byte array (4 bytes per float!)
  for (uint8_t i = 0; i <= 9*4; i+=4) {
    Serial.println(sensor1.getFloat(dataArray3,i),8); //casting bytes back in to float
  }
  delay(2000);
}