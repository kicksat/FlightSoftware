//*************************************************************
// burn.h - Library for controlling burnwires
//*************************************************************

#ifndef BURN_h
#define BURN_h

#include "Arduino.h"

#define ANTENNA_BURN_1  ENAB_BURN1
#define ANTENNA_BURN_2  ENAB_BURN2
#define DEPLOY_BURN_1   ENAB_BURN3
#define DEPLOY_BURN_2   ENAB_BURN4
#define DEPLOY_BURN_3   ENAB_BURN5

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
    void burnDB3();

};

#endif
