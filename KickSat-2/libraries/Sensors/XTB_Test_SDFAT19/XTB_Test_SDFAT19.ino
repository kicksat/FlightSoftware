#include "KickSat_Sensor.h"
#include <SPI.h>

KickSat_Sensor kSensor(ADC_CS, ADC_RST, SD_CS);

/*---------------SETUP---------------*/
void setup() {
  Serial.begin(115200);
  delay(200);
  pinMode(GREEN, OUTPUT);
  SPI.begin();
  
}

/*---------------LOOP---------------*/
void loop() {
  float buf[8];
  kSensor.operate(buf, "xtb3");  
  blink(GREEN,1); 
  for (uint8_t i=0; i<8; i++){
    Serial.println(buf[i],8);
  }
  kSensor.regReadout();
  delay(10000);                                                             
}

void blink(uint8_t LED, uint8_t flashes) {  // blink for a certain number of flashes
  uint8_t i;
  for (i=0; i<flashes; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(200);
  }
}
