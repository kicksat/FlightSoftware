#include "KickSat_Sensor.h"
#include <SPI.h>

KickSat_Sensor kSensor(ADC_CS, ADC_RST);



/*---------------SETUP---------------*/
void setup() {
  Serial.begin(115200);
  delay(200);
  SPI.begin();
}

/*---------------LOOP---------------*/
void loop() {
  
  kSensor.operate("xtb1"); //operate the specified sensor board (automatically stores data)
  
  delay(2000);
  
  // example of accessing stored board data and printing it out as a float
  float data[4]; 
  kSensor.sensorData("xtb1", data, 4); //read data for xtb1, store as float in data array
  Serial.println("printing data:");
  for (uint8_t i = 0; i < 4; i++) {
    Serial.println(data[i],8);    
  } 
}
