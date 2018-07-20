#include "battery.h"

#define LED 13
#define GPS_PIN 17


unsigned int volt, currIn, currOut;

battery battery;

void setup() {
  Serial.begin(9600);
  battery.init();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(GPS_PIN ,OUTPUT);
    
  digitalWrite(GPS_PIN, LOW);
  

}

void loop() {
  digitalWrite(LED, HIGH);
  
  battery.update();
  volt = battery.getVoltage();
  currIn = battery.getCurrentIn();
  currOut = battery.getCurrentOut();
  
  Serial.print(volt);
  Serial.println(" v");
  Serial.print(currIn);
  Serial.println(" mA in");
  Serial.print(currOut);
  Serial.println(" mA out");

  delay(1000);
  digitalWrite(LED,LOW); // LED Off
  delay(1000);

   digitalWrite(GPS_PIN, HIGH);



}
