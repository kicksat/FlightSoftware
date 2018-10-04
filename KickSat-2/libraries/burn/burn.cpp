//**********************************************************
// burn.c
// updated 2018-10-03	M.Holliday
//**********************************************************
//TO DO: verify this behaves properly with hardware. Then incorporate into FPT/flight code

#include "Arduino.h"
#include <RTCCounter.h>
#include "burn.h"

void Burn::burn(const uint8_t relay, BURNWIRE burnwire, uint8_t duration, uint8_t dutyCycle){ // duration(ms), dutyCycle(%), Frequency (Hz)
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  delay(1000)
  int counter = millis();  
  while((millis()-counter) > duration ) {          
	digitalWrite(burnwire, HIGH);
	delayMicroseconds(dutyCycle);
	digitalWrite(burnwire, LOW);
	delayMicroseconds(1000); 
  }
  digitalWrite(relay, LOW);
  digitalWrite(burnwire, LOW);
}

void Burn::burnAntennaOne() {
  pinMode(ENAB_BURN1, OUTPUT);
  digitalWrite(ENAB_BURN1, LOW);
  burn(BURN_RELAY_A, BURN_ANTENNA_1, BURN_TIME, DUTY_CYCLE);
}

void Burn::burnAntennaTwo() {
  pinMode(ENAB_BURN2, OUTPUT);
  digitalWrite(ENAB_BURN2, LOW);
  burn(BURN_RELAY_A, BURN_ANTENNA_2, BURN_TIME, DUTY_CYCLE);
}

void Burn::burnSpriteOne() {
  pinMode(ENAB_BURN3, OUTPUT);
  digitalWrite(ENAB_BURN3, LOW);
  burn(BURN_RELAY_B, BURN_SPRITE_1, BURN_TIME, DUTY_CYCLE);
}

void Burn::burnSpriteTwo() {
  pinMode(ENAB_BURN4, OUTPUT);
  digitalWrite(ENAB_BURN4, LOW);
  burn(BURN_RELAY_B, BURN_SPRITE_2, BURN_TIME, DUTY_CYCLE);
}

void Burn::burnSpriteThree(){
  pinMode(ENAB_BURN5, OUTPUT);
  digitalWrite(ENAB_BURN5, LOW);
  burn(BURN_RELAY_B, BURN_SPRITE_3, BURN_TIME, DUTY_CYCLE);
}

void Burn::burnAntenna() {
  pinMode(ENAB_BURN1, OUTPUT);
  digitalWrite(ENAB_BURN1, LOW);
  pinMode(ENAB_BURN2, OUTPUT);
  digitalWrite(ENAB_BURN2, LOW);
  burnAntennaOne();
  burnAntennaTwo();
}

Burn burn;
