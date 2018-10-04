//*************************************************************
// burn.h - Library for controlling burnwires
//*************************************************************

#ifndef BURN_h
#define BURN_h

#include "Arduino.h"
#include <RTCCounter.h>

#define BURN_TIME   30000
#define DUTY_CYCLE 300 


typedef enum { // Relay options
  BURN_ANTENNA_1 = ENAB_BURN1,
  BURN_ANTENNA_2 = ENAB_BURN2,
  BURN_SPRITE_1 = ENAB_BURN3,
  BURN_SPRITE_2 = ENAB_BURN4,
  BURN_SPRITE_3 = ENAB_BURN5
} BURNWIRE;

class Burn {
public:
  void burnAntennaOne();
  void burnAntennaTwo();
  void burnAntenna();
  void burnSpriteOne();
  void burnSpriteTwo();
  void burnSpriteThree();
private:
  void burn(const uint8_t relay, BURNWIRE burnwire, uint8_t duration, uint8_t dutyCycle); // duration(s), dutyCycle(%), Frequency (Hz)
};

extern Burn burn;

#endif
