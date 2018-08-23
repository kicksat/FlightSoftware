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
    SerialUSB.print("Battery Voltage:\t");
    SerialUSB.print(battery,3);
    SerialUSB.println(" V");
    delay(500);
    float battCurr = power.readBattCurrent();
    SerialUSB.print("Current Draw:\t\t");
    SerialUSB.print(battCurr);
    SerialUSB.println(" mA");
    delay(500);
    float battChrg = power.readBattChargeCurrent();
    SerialUSB.print("Charging Current:\t");
    SerialUSB.print(battChrg);
    SerialUSB.println(" mA");
    delay(500);
//    power.read(buff);
//    SerialUSB.print(buff[0]);
//    SerialUSB.print("    ");
//    SerialUSB.print(buff[1]);
//    SerialUSB.print("    ");
//    SerialUSB.println(buff[2]);
//    delay(500);
//  }               
  }
}
