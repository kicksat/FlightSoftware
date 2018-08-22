#include "KickSat_Sensor.h"
#include <SD_DataFile.h>
#include <SPI.h>
#include <SdFat.h>
SdFat SD;
                                //Metro Mini //Feather M0
#define ADC_CS   SPI_CS_XTB1    //10         //A2
#define ADC_RST  XTB_RESET      //0          //A1
#define SD_CS    SPI_CS_SD      //9          //4

#define Serial SerialUSB

KickSat_Sensor kSensor(ADC_CS, ADC_RST, SD_CS, "config.txt", SD);
SD_DataFile dataFile(SD_CS, 3, "data.txt", SD);

void setup() {
  SPI.begin();
  dataFile.refresh();
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Beginning");
  kSensor.shutdownADC();
  delay(2000);

//==== NEW CONFIG LOADER ====//
//uncomment the code below to load a new config file into the SD card
//this code simulates recieving an uplink with new config data
//it will read in the new config file,  calculate a checksum byte, and write it to the correct location
//   |
//   V

//  SD.begin(SD_CS);
//  digitalWrite(SD_CS, LOW);
//  Serial.println("starting rewrite");
//  File configFile = SD.open("newconfig.txt", O_READ);
//  if (configFile) {
//    Serial.println("new config found");
//    byte newConfig[configFile.size()];
//    configFile.read(newConfig, configFile.size());
//    configFile.close();
//
//    if (kSensor.rewriteConfigs(newConfig, configFile.size())) {
//      Serial.println("rewrite complete");
//    } else {
//      Serial.println("rewrite failed");
//    }
//  } else {
//    Serial.println("new config not located");
//  }
//  digitalWrite(SD_CS, HIGH);
//  while(1) {}


}

void loop() {
  byte buf[10]; //holds data from readData, after handling all commands, use it as you please
  kSensor.operate(buf);
  dataFile.writeDataEntry(buf);

  for (int i = 0; i < 3; i++) {
    Serial.print(buf[i]), Serial.print(",");
  }
  Serial.println();

//  byte out[3];
//  dataFile.readDataEntry(0, out);

//  for (int i = 0; i < 3; i++) {
//    Serial.println(out[i]);
//  }
  delay(5000);
}
