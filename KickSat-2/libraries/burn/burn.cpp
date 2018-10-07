//**********************************************************
// burn.c
// updated 2018-10-07 M.Holliday
// testing and working with NDP6020P mosfets

//                                +Y
//                              (TOP)
//          ------------------USB PORT---------------------
//          |                                             |
//          |                                             |
//          |                                             |
//          |                                             |
// -X     ENAB_BURN1                                   ENAB_BURN2   +X
//    (burnAntennaOne)                            (burnAntennaTwo)
//          |                                             |
//          |                                             |
//          |                                             |
//          |                                             |
//      ENAB_BURN3-----(BARREL JACK)--ENAB_BURN5-------ENAB_BURN4
//  (burnSpriteOne)---------------(burnSpriteThree)---(burnSpriteTwo)
//                                -Y
//
//**********************************************************
//TO DO: 

#include "Arduino.h"
#include "burn.h"

void burn::burnAntennaOne() {
  pinMode(BURN_RELAY_A, OUTPUT);
  digitalWrite(BURN_RELAY_A, HIGH);
  pinMode(ENAB_BURN1, OUTPUT);
  digitalWrite(ENAB_BURN1, LOW);

  int counter = millis();  
  while((millis()-counter) < 2000 ) {
    digitalWrite(ENAB_BURN1, HIGH);
    delay(2);
    digitalWrite(ENAB_BURN1, LOW);
    delay(48); 
  }
  digitalWrite(ENAB_BURN1, HIGH);
  delay(2);
  digitalWrite(BURN_RELAY_A, LOW);
  digitalWrite(ENAB_BURN1, LOW);
}

void burn::burnAntennaTwo() {
  pinMode(BURN_RELAY_A, OUTPUT);
  digitalWrite(BURN_RELAY_A, HIGH);
  pinMode(ENAB_BURN2, OUTPUT);
  digitalWrite(ENAB_BURN2, LOW);

  int counter = millis();  
  while((millis()-counter) < 2000 ) {
    digitalWrite(ENAB_BURN2, HIGH);
    delay(2);
    digitalWrite(ENAB_BURN2, LOW);
    delay(48); 
  }
  digitalWrite(ENAB_BURN2, HIGH);
  delay(2);
  digitalWrite(BURN_RELAY_A, LOW);
  digitalWrite(ENAB_BURN2, LOW);
}

void burn::burnSpriteOne() {
  pinMode(BURN_RELAY_B, OUTPUT);
  digitalWrite(BURN_RELAY_B, HIGH);
  pinMode(ENAB_BURN3, OUTPUT);
  digitalWrite(ENAB_BURN3, LOW);

  int counter = millis();  
  while((millis()-counter) < 2000 ) {
    digitalWrite(ENAB_BURN3, HIGH);
    delay(2);
    digitalWrite(ENAB_BURN3, LOW);
    delay(48); 
  }
  digitalWrite(ENAB_BURN3, HIGH);
  delay(2);
  digitalWrite(BURN_RELAY_B, LOW);
  digitalWrite(ENAB_BURN3, LOW);
}

void burn::burnSpriteTwo() {
  pinMode(BURN_RELAY_B, OUTPUT);
  digitalWrite(BURN_RELAY_B, HIGH);
  pinMode(ENAB_BURN4, OUTPUT);
  digitalWrite(ENAB_BURN4, LOW);
  int counter = millis();  
  while((millis()-counter) < 2000 ) {
    digitalWrite(ENAB_BURN4, HIGH);
    delay(2);
    digitalWrite(ENAB_BURN4, LOW);
    delay(48); 
  }
  digitalWrite(ENAB_BURN4, HIGH);
  delay(2);
  digitalWrite(BURN_RELAY_B, LOW);
  digitalWrite(ENAB_BURN4, LOW);
}

void burn::burnSpriteThree(){
  pinMode(BURN_RELAY_B, OUTPUT);
  digitalWrite(BURN_RELAY_B, HIGH);
  pinMode(ENAB_BURN5, OUTPUT);
  digitalWrite(ENAB_BURN5, LOW);

  int counter = millis();  
  while((millis()-counter) < 2000 ) {
    digitalWrite(ENAB_BURN5, HIGH);
    delay(2);
    digitalWrite(ENAB_BURN5, LOW);
    delay(48); 
  }
  digitalWrite(ENAB_BURN5, HIGH);
  delay(2);
  digitalWrite(BURN_RELAY_B, LOW);
  digitalWrite(ENAB_BURN5, LOW);
}
