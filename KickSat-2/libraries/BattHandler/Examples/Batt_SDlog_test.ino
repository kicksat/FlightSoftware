/*
  Monitors battery voltage, current draw, and charging current.
  Saves data to a unique file on SD card.

  Last Edited: 2018-08-22
  Max Holliday


 */

#include <BattHandler.h>
#include <SPI.h>
#include <SdFat.h>

SdFat SD;
File logfile;
BattHandle power;

/*----------INITAL START TIME---------*/
const byte hours = 10;
const byte minutes = 30;
const byte seconds = 00;
const byte day = 13;
const byte month = 8;
const byte year = 18;
float measuredvbat;   
char filename[15];               // Array for filename

void setup() {
  SerialUSB.begin(115200);
  delay(2000);
  BattHandle();
  SPI.begin();
  strcpy(filename, "DATA0000.CSV");                       // Template for file name, characters 6 & 7 get set automatically later 
  CreateFile();                                           //MUST do - init SD card and creates unique filename
  writeHeader();  
}

void loop() {
  int timeT = millis();
  SerialUSB.println("");
  SerialUSB.print("Time:\t\t\t");
  SerialUSB.print(timeT);
  SerialUSB.println(" ms");
  logfile.print(timeT);                         // saving data to the SD card
  logfile.print(",");
  float battery = power.readBattVoltage();
  SerialUSB.print("Battery Voltage:\t");
  SerialUSB.print(battery,3);
  SerialUSB.println(" V");
  logfile.print(battery);                       // saving data to the SD card
  logfile.print(",");
  float battCurr = power.readBattCurrent();
  SerialUSB.print("Current Draw:\t\t");
  SerialUSB.print(battCurr);
  SerialUSB.println(" mA");
  logfile.print(battCurr);                       // saving data to the SD card
  logfile.print(",");
  float battChrg = power.readBattChargeCurrent();
  SerialUSB.print("Charging Current:\t");
  SerialUSB.print(battChrg);
  SerialUSB.println(" mA");
  logfile.println(battChrg);                       // saving data to the SD card
  logfile.flush();
  delay(1000);  
}

void writeHeader() {
  logfile.println("Time (millis), Battery Voltage (V), Current Draw (mA), Charging Current (mA)");
}

void CreateFile() {  
  if (!SD.begin(SPI_CS_SD)) {                 // see if the card is present and can be initialized:
    while (1){};                           // stay in this while loop to prevent data loss
  }
  for (uint8_t i = 0; i < 100; i++) {     //ensure the filename is unique  
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;    
    if (! SD.exists(filename)) {          // create if does not exist, do not open existing, write, sync after write
      break;
    }
  }  
  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    while (1){};                           // stay in this while loop to prevent data loss
  }
}