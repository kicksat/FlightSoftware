//**********************************************************
// burn.c
//**********************************************************

#include "Arduino.h"
#include <RTCCounter.h>
#include "burn.h"

void Burn::burn(const uint8_t relay, BURNWIRE burnwire, uint8_t duration, uint8_t dutyCycle, uint8_t pulseFrequency){ // duration(s), dutyCycle(%), Frequency (Hz)

  SerialUSB.println(relay);
  SerialUSB.println(burnwire);
  SerialUSB.println(duration);
  SerialUSB.println(dutyCycle);
  SerialUSB.println(pulseFrequency);
  SerialUSB.println();

  // pinMode(burnwire, OUTPUT);
  // digitalWrite(relay, HIGH);
  float  cycleTime = 1/pulseFrequency;
  timeout.start(duration);
  while(1) {
    // digitalWrite(burnwire, HIGH);
    delayMicroseconds(cycleTime*dutyCycle/100);
    // digitalWrite(burnwire, LOW);
    delayMicroseconds(cycleTime*(1-dutyCycle/100));
    if (timeout.triggered()) { // Checks time for timeout
      break;
    }
  }
  // digitalWrite(relay, LOW);

}

void Burn::burnAntennaOne() {
  burn(BURN_RELAY_A, BURN_ANTENNA_1, ANTENNA_BURN_TIME, ANTENNADUTYCYCLE, ANTENNABURNFREQUENCY);
}

void Burn::burnAntennaTwo() {
  burn(BURN_RELAY_A, BURN_ANTENNA_2, ANTENNA_BURN_TIME, ANTENNADUTYCYCLE, ANTENNABURNFREQUENCY);
}

void Burn::burnSpriteOne() {
  burn(BURN_RELAY_B, BURN_SPRITE_1, ANTENNA_BURN_TIME, SPRITEDUTYCYCLE, SPRITEBURNFREQUENCY);
}

void Burn::burnSpriteTwo() {
  burn(BURN_RELAY_B, BURN_SPRITE_2, ANTENNA_BURN_TIME, SPRITEDUTYCYCLE, SPRITEBURNFREQUENCY);
}

void Burn::burnSpriteThree(){
  burn(BURN_RELAY_B, BURN_SPRITE_3, ANTENNA_BURN_TIME, SPRITEDUTYCYCLE, SPRITEBURNFREQUENCY);
}

void Burn::burnAntenna() {
  burnAntennaOne();
  burnAntennaTwo();
}

Burn burn;
