//**********************************************************
// burn.c
// updated 2018-10-03	M.Holliday
//**********************************************************
//TO DO: verify this behaves properly with hardware. Then incorporate into FPT/flight code

#include "Arduino.h"
#include <RTCCounter.h>
#include "burn.h"

void Burn::burn(const uint8_t relay, BURNWIRE burnwire, uint8_t duration, uint8_t dutyCycle){ // duration(ms), dutyCycle(%), Frequency (Hz)
  digitalWrite(relay, HIGH);
  delay(1000)
  int counter = millis();  
  while(millis()-counter > duration ) {          
	digitalWrite(burnwire, HIGH);
	delayMicroseconds(dutyCycle);
	digitalWrite(burnwire, LOW);
	delayMicroseconds(1000); 
  }
  digitalWrite(relay, LOW);

}

void Burn::burnAntennaOne() {
  burn(BURN_RELAY_A, BURN_ANTENNA_1, BURN_TIME, DUTY_CYCLE);
}

void Burn::burnAntennaTwo() {
  burn(BURN_RELAY_A, BURN_ANTENNA_2, BURN_TIME, DUTY_CYCLE);
}

void Burn::burnSpriteOne() {
  burn(BURN_RELAY_B, BURN_SPRITE_1, BURN_TIME, DUTY_CYCLE);
}

void Burn::burnSpriteTwo() {
  burn(BURN_RELAY_B, BURN_SPRITE_2, BURN_TIME, DUTY_CYCLE);
}

void Burn::burnSpriteThree(){
  burn(BURN_RELAY_B, BURN_SPRITE_3, BURN_TIME, DUTY_CYCLE);
}

void Burn::burnAntenna() {
  burnAntennaOne();
  burnAntennaTwo();
}

Burn burn;
