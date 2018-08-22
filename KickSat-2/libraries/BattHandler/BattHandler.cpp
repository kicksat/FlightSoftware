/*
BatteryHandler - Gathers voltage and current data from the battery and its charging circuit

Last update on: 8-22-18
by: Max
*/

#include "BattHandler.h"

//constructor
BattHandle::BattHandle() {
  //configure pins and inputs
  analogReadResolution(12); //setting 12 bit resolution
  pinMode(VBATT, INPUT);
  pinMode(L1_PROG, INPUT);
  pinMode(I_BATT, INPUT);

}

//calculates VBATT
float BattHandle::readBattVoltage() {
  int voltagePinValue = analogRead(VBATT);
  float battVoltage = voltagePinValue * (3.3/4096.0) * (426.0/110.0); //12bit-ADC with a 426/110 voltage divider
  return battVoltage;
}

//calculates I_BATT (in mA)
// 8mV reading = 8mA 
float BattHandle::readBattCurrent() { 
  int currentPinValue = analogRead(I_BATT);
  float battCurrent = currentPinValue * (3.3/4096.0); //12bit-ADC, in volts
  battCurrent = battCurrent * 1000.0; //putting it in mA
  return battCurrent;
}

//calculates the charging current in L1_PROG
/*
  Ichrg= (988 * Vprog)/Rprog
  Vprog=1.227V during constant current mode, 0.122 during trickle
  Rprog=6.04K
*/
float BattHandle::readBattChargeCurrent() {
  int chargeCurrentPinValue = analogRead(L1_PROG);
  float battChargeCurrent = chargeCurrentPinValue * (3.3/4096.0); //12bit-ADC, in volts
  battChargeCurrent = ((battChargeCurrent * 988.0) / 6040.0) * 1000.0; //solve for Ichrg, convert to mA, 
  return battChargeCurrent;
}

//calculate values form VBATT, IBATT, and L1_PROG, saves them to a buffer
void BattHandle::read(float buf[]) {
  float battVoltage; //refers to the VBATT pin
  float battCurrent; //refers to the IBATT pin
  float battChargeCurrent; //refers to L1_prog pin connected to the battery charging circuit
  battVoltage = readBattVoltage();
  battCurrent = readBattCurrent();
  battChargeCurrent = readBattChargeCurrent();
  buf[0] = battCurrent;
  buf[1] = battVoltage;
  buf[2] = battChargeCurrent;
}
