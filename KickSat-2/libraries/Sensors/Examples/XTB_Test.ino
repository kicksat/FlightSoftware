#include "KickSat_Sensor.h"
#include "SD_DataFile.h"
#include <SPI.h>
#include <SD.h>

#define ADC_CS A1
#define ADC_RST A2
#define SD_CS A5

KickSat_Sensor kSensor(ADC_CS, ADC_RST, SD_CS, "config.txt");
SD_DataFile dataFile(SD_CS, 3, "data.txt");

void setup() {
  SPI.begin();
  dataFile.refresh();
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Beginning");
  delay(2000);
}

void loop() {
  byte buf[100]; //holds data from readData, after handling all commands, use it as you please
  kSensor.operate(buf);
  dataFile.writeDataEntry(buf);

  for (int i = 0; i < 3; i++) {
    Serial.println(buf[i]);
  }
  
  Serial.println("-=-=-=-=-");
  
  byte out[3];
  dataFile.readDataEntry(0, out);

  for (int i = 0; i < 3; i++) {
    Serial.println(out[i]);
  }
  delay(2000);
}
