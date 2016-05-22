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
    unsigned int getVoltage();
    unsigned int getCurrentIn();
    unsigned int getCurrentOut();

    unsigned int voltage;
    unsigned int currentIn;
    unsigned int currentOut;

};

#endif