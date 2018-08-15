//*************************************************************
// burn.h - Library for controlling burnwires
//*************************************************************

#ifndef BURN_h
#define BURN_h

#include "Arduino.h"
#include <RTCCounter.h>

#define ANTENNA_BURN_TIME   2 // seconds
#define SPRITE_BURN_TIME   12 // seconds

#define ANTENNADUTYCYCLE 10 // %
#define SPRITEDUTYCYCLE 30 // %

#define ANTENNABURNFREQUENCY 20 //Hz
#define SPRITEBURNFREQUENCY 20 // Hz

typedef enum { // Relay options
  BURN_ANTENNA_1, = ENAB_BURN1,
  BURN_ANTENNA_2 = ENAB_BURN2,
  BURN_SPRITE_1 = ENAB_BURN3,
  BURN_SPRITE_2 = ENAB_BURN4,
  BURN_SPRITE_3 = ENAB_BURN5
} BURNWIRE;

typedef enum { // Relay options
  BURN_RELAY_A,
  BURN_RELAY_B
} RELAY;

class Burn {
public:
  void burnAntennaOne();
  void burnAntennaTwo();
  void burnAntenna();
  void burnSpriteOne();
  void burnSpriteTwo();
  void burnSpriteThree();
private:
  void burn(RELAY relay, BURNWIRE burnwire, uint8_t duration, uint8_t dutyCycle, uint8_t pulseFrequency); // duration(s), dutyCycle(%), Frequency (Hz)
};

extern Burn burn;

#endif
