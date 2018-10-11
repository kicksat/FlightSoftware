#include "KickSat_Sensor.h"
#include <SPI.h>


KickSat_Sensor kSensor(ADC_CS, ADC_RST);

//WORKING FOR BYTES
//SEE VERSION C FOR FLOAT FORMATING (ALSO WORKING)

    
/*---------------SETUP---------------*/
void setup() {
  Serial.begin(115200);
  while(!Serial);
  delay(200);
  SPI.begin();
}

/*---------------LOOP---------------*/
void loop() {
  
  byte One[sensor1_BUF_LEN*4];
  byte Two[sensor2_BUF_LEN*4];
  byte Three[sensor3_BUF_LEN*4];
  kSensor.operate("xtb1"); //operate the specified sensor board (automatically stores data)
  kSensor.operate("xtb2");
  kSensor.operate("xtb3");
  delay(2000);
  
  // example of accessing stored board data and printing it out as a float
  kSensor.sensorData(One, Two, Three); //read (byte) data payload based on the sensorPayload Struct
  Serial.println("Printing FLOAT Data from one");
  for (uint8_t i = 0; i < sensor1_BUF_LEN*4; i+=4) {
    Serial.println(kSensor.getFloat(One,i),8);   
  }
  Serial.println("Printing FLOAT Data from two");
  for (uint8_t i = 0; i < sensor2_BUF_LEN*4; i+=4) {
    Serial.println(kSensor.getFloat(Two,i),8);   
  }
  Serial.println("Printing FLOAT Data from three");
  for (uint8_t i = 0; i < sensor3_BUF_LEN*4; i+=4) {
    Serial.println(kSensor.getFloat(Three,i),8);    
  }
  
}
