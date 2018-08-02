//*************************************************************
// burn.h - Library for controlling burnwires
//*************************************************************
#ifndef BURN_h
#define BURN_h

#include "Arduino.h"

#define ANTENNA_BURN_1  2
#define ANTENNA_BURN_2  4
#define DEPLOY_BURN_1   PORT_PB03
#define DEPLOY_BURN_2   3
#define BURN_RELAY_A    5
#define BURN_RELAY_B    PORT_PA10
#define ANTENNA_BURN_TIME   2000 //milliseconds
#define SPRITE_BURN_TIME   12000 //milliseconds

 class burn {
  public:
    void init();
    void burnAntennas();
    void burnAB1();
    void burnAB2();
    void burnDB1();
    void burnDB2();
    bool AB1deployed();
    bool AB2deployed();
    bool DB1burned();
    bool DB2burned();

    bool antenna1_deployed = 0;
    bool antenna2_deployed = 0;
    bool db1_burned = 0;
    bool db2_burned = 0;

};

#endif
