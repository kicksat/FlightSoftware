#include <KickSat_Sensor.h>
#include <SPI.h>
#include <SdFat.h>
#define Serial SerialUSB
SdFat SD;

KickSat_Sensor kSensor(XTB_RESET);

/*---------------SETUP---------------*/
void setup() {
  Serial.begin(115200);
  delay(200);
  SPI.begin();
}

/*---------------LOOP---------------*/
void loop() {  
  //arrays to store the sensor data (only for readback) 
  byte One[sensor1_BUF_LEN*4];
  byte Two[sensor2_BUF_LEN*4];
  byte Three[sensor3_BUF_LEN*4];

  kSensor.operate("xtb1"); //operate the specified sensor board (automatically stores data)
  kSensor.operate("xtb2");
  kSensor.operate("xtb3");
  
  // example of accessing stored sensor data and printing it out as a BYTE
  kSensor.sensorPacket(One, Two, Three); //doesn't matter which sensor you call

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
