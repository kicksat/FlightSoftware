/*
  battery.h - Library for reading battery voltage and current
*/
#ifndef battery_h
#define battery_h

#include "Arduino.h"

 class battery
{
  public:
    battery(int pin);
    void burnAB1();
    void burnAB2();
    void burnDB1();
    void burnDB2();
    void burnCheck(char check);
    // Important Pins
    int ANTENNA_BURN_1
    int ANTENNA_BURN_2
    int DEPLOY_BURN_1
    int DEPLOY_BURN_2
    int ENAB_GPS=17
    int LED=13
    unsigned char antenna1_deployed;
    unsigned char antenna2_deployed;
    unsigned char sprites_armed;
    unsigned char sprites_deployed;
    char command_buffer[4];
};

#endif