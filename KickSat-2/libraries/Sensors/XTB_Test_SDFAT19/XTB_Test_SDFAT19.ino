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
  kSensor.operate("xtb1"); //operate the sensor board and store data in buf  
  delay(2000);
  kSensor.readoutData("xtb1");  
}



