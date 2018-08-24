//*************************************************************
// battery.h - Library for reading battery voltage and current
//*************************************************************
#ifndef BATTERY_h
#define BATTERY_h

#include "Arduino.h"

class battery {
public:
    void init();
    void update();
    float getVoltage();
    float getCurrentIn();
    float getCurrentOut();

    float voltage;
    float currentIn;
    float currentOut;

};

#endif