#include "KickSat_Sensor.h"
#include <SPI.h>
#include <SdFat.h>
SdFat SD;
File datafile3;

struct datastore {
    float dat1;
    float dat2;
    float dat3;
    float dat4;
};

KickSat_Sensor kSensor(ADC_CS, ADC_RST);

/*---------------SETUP---------------*/
void setup() {
  Serial.begin(115200);
  delay(200);
  SPI.begin(); 
  if (SD.begin(SD_CS)) {
    datafile3 = SD.open("xtb2.dat", FILE_WRITE);
    Serial.println("SDcard initalized");
  }
}
uint8_t cnt=0;
/*---------------LOOP---------------*/
void loop() {
  float buf1[4];
  kSensor.operate(buf1, "xtb1"); //operate the sensor board and store data in buf  
  datafile3.write((const uint8_t *)&buf1, sizeof(buf1)); //save buf to SD card as bytes (4B per float)
  datafile3.flush();
  delay(2000);
  readoutData();
}

void readoutData(){
  if (datafile3.isOpen()) {
    datafile3.close();
  }
  datafile3 = SD.open("xtb2.dat", FILE_READ);
  struct datastore testData;
  datafile3.read((uint8_t *)&testData, sizeof(testData));
  Serial.println(testData.dat1,8);
  Serial.println(testData.dat2,8);
  Serial.println(testData.dat3,8);
  Serial.println(testData.dat4,8);
  datafile3.close();
  while(1);
}

