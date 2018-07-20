//**********************************************************
// burn.c
//**********************************************************


// TODO: ADD IN EEPROM STUFF

#include "burn.h"

void burn::init() {
    pinMode(ANTENNA_BURN_1, OUTPUT);
    digitalWrite(ANTENNA_BURN_1, LOW);
    
    pinMode(ANTENNA_BURN_2, OUTPUT);
    digitalWrite(ANTENNA_BURN_2, LOW);
    
    pinMode(DEPLOY_BURN_1, OUTPUT);
    digitalWrite(DEPLOY_BURN_1, LOW);
    
    pinMode(DEPLOY_BURN_2, OUTPUT);
    digitalWrite(DEPLOY_BURN_2, LOW);
}

void burn::burnAntennas() {
    burnAB1();
    burnAB2();
}

void burn::burnAB1() {
    for (unsigned int k = 0; k < ANTENNA_BURN_TIME/100; ++k) {
        // PWM with 10% duty cycle
        digitalWrite(ANTENNA_BURN_1, HIGH);
        delay(5);
        digitalWrite(ANTENNA_BURN_1, LOW);
        delay(45);
        digitalWrite(ANTENNA_BURN_1, HIGH);
        delay(5);
        digitalWrite(ANTENNA_BURN_1, LOW);
        delay(45);
    }
    antenna1_deployed = 1;
}

void burn::burnAB2() {
    for (unsigned int k = 0; k < ANTENNA_BURN_TIME/100; ++k) {
        // PWM with 10% duty cycle
        digitalWrite(ANTENNA_BURN_2, HIGH);
        delay(5);
        digitalWrite(ANTENNA_BURN_2, LOW);
        delay(45);
        digitalWrite(ANTENNA_BURN_2, HIGH);
        delay(5);
        digitalWrite(ANTENNA_BURN_2, LOW);
        delay(45);
    }
    antenna2_deployed = 1;
}

void burn::burnDB1() {
    for (unsigned int k = 0; k < ANTENNA_BURN_TIME/100; ++k) {
        // PWM with 10% duty cycle
        digitalWrite(DEPLOY_BURN_1, HIGH);
        delay(5);
        digitalWrite(DEPLOY_BURN_1, LOW);
        delay(45);
        digitalWrite(DEPLOY_BURN_1, HIGH);
        delay(5);
        digitalWrite(DEPLOY_BURN_1, LOW);
        delay(45);
    }
    sprites_armed = 1;
}

void burn::burnDB2() {
    for (unsigned int k = 0; k < SPRITE_BURN_TIME/100; ++k) {
        // PWM with 30% duty cycle
        digitalWrite(DEPLOY_BURN_2, HIGH);
        delay(15);
        digitalWrite(DEPLOY_BURN_2, LOW);
        delay(35);
        digitalWrite(DEPLOY_BURN_2, HIGH);
        delay(15);
        digitalWrite(DEPLOY_BURN_2, LOW);
        delay(35);
      }
      sprites_deployed = 1;
}

bool burn::AB1deployed() {
    return antenna1_deployed;
}

bool burn::AB2deployed() {
    return antenna2_deployed;
}

bool burn::spritesArmed() {
    return sprites_armed;
}

bool burn::spritesDeployed() {
    return sprites_deployed;
}
