#include "SAMD_AnalogCorrection.h"
#include <ATSAMD21_ADC.h>
#include <BattHandler.h>

int sensorPin = VBATT;    // select the input pin for the potentiometer
BattHandle power;
float buff[3];

void setup() {
  // declare the ledPin as an OUTPUT:
  SerialUSB.begin(115200);
}

void loop() {
//  analogReadCorrection(2, 2053); 
  SerialUSB.println("Init");
  for(int i = 0; i < 20; i++) {
    float battery = power.readBattVoltage();
    SerialUSB.print("Voltage: ");
    SerialUSB.println(battery);
    delay(500);
    SerialUSB.print("Current: ");
    SerialUSB.println(power.readBattCurrent());
    delay(500);
    SerialUSB.print("Charge I: ");
    SerialUSB.println(power.readBattChargeCurrent());
    delay(500);
    power.read(buff);
    SerialUSB.print(buff[0]);
    SerialUSB.print("    ");
    SerialUSB.print(buff[1]);
    SerialUSB.print("    ");
    SerialUSB.println(buff[2]);
    delay(500);
  }               
}
