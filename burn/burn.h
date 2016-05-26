//*************************************************************
// burn.h - Library for controlling burnwires
//*************************************************************
#ifndef BURN_h
#define BURN_h

#include "Arduino.h"

#define ANTENNA_BURN_1  3
#define ANTENNA_BURN_2  4
#define DEPLOY_BURN_1   5
#define DEPLOY_BURN_2   6
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
    bool spritesArmed();
    bool spritesDeployed();

    bool antenna1_deployed = 0;
    bool antenna2_deployed = 0;
    bool sprites_armed = 0;
    bool sprites_deployed = 0;

};

#endif