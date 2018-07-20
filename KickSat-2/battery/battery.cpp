//**********************************************************
// battery.c
//**********************************************************

#include "battery.h"

void battery::init() {
    analogReadRes(10);
    analogReadAveraging(32);
    analogReference(DEFAULT);
}

void battery::update() {
    unsigned int val;

    // TODO: make sure voltage levels are correct

    val = analogRead(A1); // Read Voltage
    voltage = val*(426.0/110.0)*(3.3/1024); // Convert to Volts

    val = analogRead(A0); // Read Current Out
    currentOut = val*200.0*(3.3/1024); //Convert to mA

    val = analogRead(A2); // Read Current In
    currentIn = val*200.0*(3.3/1024); //Convert to mA
}

float battery::getVoltage() {
    return voltage;
}

float battery::getCurrentIn() {
    return currentIn;
}

float battery::getCurrentOut() {
    return currentOut;
}

