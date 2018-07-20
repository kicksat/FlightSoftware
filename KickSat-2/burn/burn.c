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

    pinMode(BURN_RELAY_A, OUTPUT);
    digitalWrite(BURN_RELAY_A, LOW);

    pinMode(BURN_RELAY_B, OUTPUT);
    digitalWrite(BURN_RELAY_B, LOW);

}

void burn::burnAntennas() {
    burnAB1();
    burnAB2();
}

void burn::burnAB1() {
    digitalWrite(BURN_RELAY_A, HIGH);
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
    digitalWrite(BURN_RELAY_A, LOW);
    antenna1_deployed = 1;    //change to write to config file
}

void burn::burnAB2() {
    digitalWrite(BURN_RELAY_B, HIGH);
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
    digitalWrite(BURN_RELAY_B, LOW);
    antenna2_deployed = 1;    //write to config file
}

void burn::burnDB1() {
    digitalWrite(BURN_RELAY_A, HIGH);
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
    digitalWrite(BURN_RELAY_A, LOW);
    db1_burned = 1;   //write to config file
}

void burn::burnDB2() {
    digitalWrite(BURN_RELAY_B, HIGH);
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
      digitalWrite(BURN_RELAY_B, LOW);
      db2_burned = 1;   //write to config file
}



bool burn::AB1deployed() {
    return antenna1_deployed; //read from config
}

bool burn::AB2deployed() {
    return antenna2_deployed; //read from config
}

bool burn::DB1burned() {
    return db1_burned;  //read from config
}

bool burn::DB2burned() {
    return db2_burned;  //read from config
}
