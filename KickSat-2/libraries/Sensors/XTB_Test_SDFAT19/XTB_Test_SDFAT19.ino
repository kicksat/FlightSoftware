#include "KickSat_Sensor.h"
#include <RTCZero.h>
#include <SPI.h>
#include <SdFat.h>
SdFat SD;
File logfile;


KickSat_Sensor kSensor(ADC_CS, ADC_RST, SD_CS, "config.txt");



/*-----------SLEEP VARRIABLES----------*/
RTCZero rtc;                     // Create RTC object
               // Array for file name data logged to named in setup
int NextAlarmSec;                // Variable to hold next alarm time seconds value
int NextAlarmMin;                // Variable to hold next alarm time minute value


/*----------INITAL START TIME---------*/
const byte hours = 10;
const byte minutes = 30;
const byte seconds = 00;
const byte day = 13;
const byte month = 8;
const byte year = 18;

/*---------------SETUP---------------*/
void setup() {
  Serial.begin(115200);
  delay(200);
  pinMode(GREEN, OUTPUT);
  SPI.begin();

  
  kSensor.CreateFile();                                           //MUST do - init SD card and creates unique filename
  kSensor.writeHeader();
  
}

/*---------------LOOP---------------*/
void loop() {
  float buf[9];
  kSensor.operate(buf, "xtb1");  
  blink(GREEN,1); 
  delay(10000);
                                                             
}
/*-----------------------------------*/
/*-----------------------------------*/





void blink(uint8_t LED, uint8_t flashes) {  // blink for a certain number of flashes
  uint8_t i;
  for (i=0; i<flashes; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(200);
  }
}
