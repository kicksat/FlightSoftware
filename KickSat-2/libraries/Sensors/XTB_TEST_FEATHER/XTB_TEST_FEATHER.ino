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
  uint8_t len=4;
  byte dataArray[len*4];
  
  kSensor.operate("xtb1"); //operate the specified sensor board (automatically stores data)
  
  delay(2000);
  
  // example of accessing stored board data and printing it out as a float
  kSensor.sensorData("xtb1", dataArray, len); //read data for xtb1, data is provided as byte array (4 bytes per float!)
  for (uint8_t i = 0; i <= len*4; i+=4) {
    Serial.print(i);
    Serial.print(" ");
    Serial.print(dataArray[i],HEX);
    Serial.print("\t");
    Serial.println(kSensor.getFloat(dataArray,i),8); //casting bytes back in to float
  }
}
