/*
BatteryHandler - Gathers voltage and current data from the battery and its charging circuit

Last update on: 8-13-18
by: Andrea
*/

#include "BattHandler.h"

//constructor
BattHandle::BattHandle() {

}

//calculates VBATT
float BattHandle::readBattVoltage() {
  int voltagePinValue = analogRead(VBATT);
  float battVoltage = voltagePinValue * (3.3/1023.0) * (426.0/110.0); //10bit-ADC with a 426/110 voltage divider
  return battVoltage;
}

//calculates IBATT, from measure_Ibatt code written by Max
float BattHandle::readBattCurrent() { 
  int currentPinValue = analogRead(I_BATT);
  float battCurrent = currentPinValue * (3.3/1023.0); //10bit-ADC, in volts
  battCurrent = (battCurrent / (20.0 * 0.02))*1000.0; //calculating actual Ibatt, putting it in mA
  return battCurrent;
}

//calculates the charging current in L1_PROG
//TODO: figure out the math, cant be tested in KMB21
float BattHandle::readBattChargeCurrent() {
  int chargeCurrentPinValue = analogRead(L1_PROG);
  float battChargeCurrent = chargeCurrentPinValue * (3.3/1023.0); //10bit-ADC, in volts
  battChargeCurrent = battChargeCurrent * 1000.0; //convert to mA, TODO: Fix math
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
