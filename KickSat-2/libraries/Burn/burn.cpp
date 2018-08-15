//**********************************************************
// burn.c
//**********************************************************

#include "Arduino.h"
#include <RTCCounter.h>
#include "burn.h"

void burn::burn(RELAY relay, BURNWIRE burnwire, uint8_t duration, uint8_t dutyCycle, uint8_t pulseFrequency){ // duration(s), dutyCycle(%), Frequency (Hz)
  pinMode(burnwire, OUTPUT);
  digitalWrite(relay, HIGH);
  float  cycleTime = 1/pulseFrequency;
  timeout.start(duration);
  while(1) {
    digitalWrite(burnwire, HIGH);
    delay(cycleTime*dutyCycle/100);
    digitalWrite(burnwire, LOW);
    delay(cycleTime*(1-dutyCycle/100));
    if (timeout.triggered()) { // Checks time for timeout
      return false;
    }
  }
  digitalWrite(relay, LOW);
  pinMode(burnwire, OUTPUT);
}

void burn::burnAntennaOne() {
  burn(BURN_RELAY_A, BURN_ANTENNA_1, ANTENNA_BURN_TIME, ANTENNADUTYCYCLE, ANTENNABURNFREQUENCY);
}

void burn::burnAntennaTwo() {
  burn(BURN_RELAY_A, BURN_ANTENNA_2, ANTENNA_BURN_TIME, ANTENNADUTYCYCLE, ANTENNABURNFREQUENCY);
}

void burn::burnSpriteOne() {
  burn(BURN_RELAY_B, BURN_SPRITE_1, ANTENNA_BURN_TIME, SPRITEDUTYCYCLE, SPRITEBURNFREQUENCY);
}

void burn::burnSpriteTwo() {
  burn(BURN_RELAY_B, BURN_SPRITE_2, ANTENNA_BURN_TIME, SPRITEDUTYCYCLE, SPRITEBURNFREQUENCY);
}

void burn::burnSpriteThree(){
  burn(BURN_RELAY_B, BURN_SPRITE_3, ANTENNA_BURN_TIME, SPRITEDUTYCYCLE, SPRITEBURNFREQUENCY);
}

void burn::burnAntenna() {
  burnAntennaOne();
  burnAntennaTwo();
}

Burn burn;
