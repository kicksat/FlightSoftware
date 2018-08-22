#include "SAMD_AnalogCorrection.h"
#include <ATSAMD21_ADC.h>
#include <BattHandler.h>

BattHandle power;
float buff[3];

void setup() {
  SerialUSB.begin(115200);
  delay(2000);
  BattHandle();
  delay(200);
  
}

void loop() {
//  analogReadCorrection(2, 2053); 
  SerialUSB.println("Init");
  for(int i = 0; i < 20; i++) {
    float battery = power.readBattVoltage();
    SerialUSB.print("Voltage: ");
    SerialUSB.println(battery,3);
    delay(500);
    float battCurr = power.readBattCurrent();
    SerialUSB.print("Current Draw: ");
    SerialUSB.println(battCurr);
    delay(500);
    float battChrg = power.readBattChargeCurrent();
    SerialUSB.print("Charging Current: ");
    SerialUSB.println(battChrg);
    delay(500);
//    power.read(buff);
//    SerialUSB.print(buff[0]);
//    SerialUSB.print("    ");
//    SerialUSB.print(buff[1]);
//    SerialUSB.print("    ");
//    SerialUSB.println(buff[2]);
//    delay(500);
  }               
}
}
