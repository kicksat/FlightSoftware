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
    datafile3 = SD.open("xtb12.dat", FILE_WRITE);
    Serial.println("SDcard initalized");
  }
}
uint8_t cnt=0;
/*---------------LOOP---------------*/
void loop() {
  float buf1[4];
  kSensor.operate(buf1, "xtb1"); //operate the sensor board and store data in buf  
  for (uint8_t i=0; i<4; i++){
    Serial.println(buf1[i],8);
    datafile3.write((const float *)&buf1[i], sizeof(buf1[i])); //save buf to SD card as bytes (4B/float)
  }
  datafile3.flush();
  delay(1000);
  
  if (cnt == 4){ //example of reading out the packed data from SD card
    datafile3.close();
    datafile3 = SD.open("xtb12.dat", FILE_READ);
    struct datastore myData;
    datafile3.read((float *)&myData, sizeof(myData));
    Serial.println(myData.dat1,4);
    Serial.println(myData.dat1,8);
    while(1);
  }
  cnt ++;
}