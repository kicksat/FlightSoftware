//**********************************************************
// burn.c
//**********************************************************


#include "burn.h"
#include <time.h>
#include <stdio.h>
#include "sam.h"
 
void init() {
    REG_PORT_DIRSET0 |= ANTENNA_BURN_1;	//Set as output
    REG_PORT_OUTCLR0 |= ANTENNA_BURN_1; //initialize to zero
    
    REG_PORT_DIRSET0 |= ANTENNA_BURN_2;
    REG_PORT_OUTCLR0 |= ANTENNA_BURN_2;
    
    REG_PORT_DIRSET1 |= DEPLOY_BURN_1;
    REG_PORT_OUTCLR1 |= DEPLOY_BURN_1;
    
    REG_PORT_DIRSET0 |= DEPLOY_BURN_2;
    REG_PORT_OUTCLR0 |= DEPLOY_BURN_2;
	
	REG_PORT_DIRSET0 |= BURN_RELAY_A;
	REG_PORT_OUTCLR0 |= BURN_RELAY_A;
	
	REG_PORT_DIRSET0 |= BURN_RELAY_B;
	REG_PORT_OUTCLR0 |= BURN_RELAY_B;
}

void burnAntennas() {
    burnAB1();
    burnAB2();
}

void burnAB1() {
	REG_PORT_OUTSET0 |= BURN_RELAY_A;
    for (unsigned int k = 0; k < ANTENNA_BURN_TIME/100; ++k) {
        // PWM with 10% duty cycle
        REG_PORT_OUTSET0 |= ANTENNA_BURN_1;
        delay(5);
        REG_PORT_OUTCLR0 |= ANTENNA_BURN_1;
        delay(45);
        REG_PORT_OUTSET0 |= ANTENNA_BURN_1;
        delay(5);
        REG_PORT_OUTCLR0 |= ANTENNA_BURN_1;
        delay(45);
    }
	REG_PORT_OUTCLR0 |= BURN_RELAY_A;
    //antenna1_deployed = 1;
}

void burnAB2() {
	REG_PORT_OUTSET0 |= BURN_RELAY_B;
    for (unsigned int k = 0; k < ANTENNA_BURN_TIME/100; ++k) {
        // PWM with 10% duty cycle
        REG_PORT_OUTSET0 |= ANTENNA_BURN_2;
        delay(5);
        REG_PORT_OUTCLR0 |= ANTENNA_BURN_2;
        delay(45);
        REG_PORT_OUTSET0 |= ANTENNA_BURN_2;
        delay(5);
        REG_PORT_OUTCLR0 |= ANTENNA_BURN_2;
        delay(45);
    }
	REG_PORT_OUTCLR0 |= BURN_RELAY_B;
   // antenna2_deployed = 1;
}

void burnDB1(){
	REG_PORT_OUTSET0 |= BURN_RELAY_A;
    for (unsigned int k = 0; k < ANTENNA_BURN_TIME/100; ++k) {
        // PWM with 10% duty cycle
        REG_PORT_OUTSET1 |= DEPLOY_BURN_1;
        delay(5);
        REG_PORT_OUTCLR1 |= DEPLOY_BURN_1;
        delay(45);
        REG_PORT_OUTSET1 |= DEPLOY_BURN_1;
        delay(5);
        REG_PORT_OUTCLR1 |= DEPLOY_BURN_1;
        delay(45);
    }
	REG_PORT_OUTCLR0 |= BURN_RELAY_A;
    //sprites_armed = 1;
}

void burnDB2() {
	REG_PORT_OUTSET0 |= BURN_RELAY_B;
    for (unsigned int k = 0; k < SPRITE_BURN_TIME/100; ++k) {
        // PWM with 30% duty cycle
        REG_PORT_OUTSET0 |= DEPLOY_BURN_2;
        delay(15);
        REG_PORT_OUTCLR0 |= DEPLOY_BURN_2;
        delay(35);
        REG_PORT_OUTSET0 |= DEPLOY_BURN_2;
        delay(15);
        REG_PORT_OUTCLR0 |= DEPLOY_BURN_2;
        delay(35);
      }
	  REG_PORT_OUTCLR0 |= BURN_RELAY_B;
     // sprites_deployed = 1;
}

void delay(int milliseconds){
	time_t currentClock = clock();
	int currentTime = 1000 * (currentClock/CLOCKS_PER_SEC);
	while(clock()*1000/CLOCKS_PER_SEC < currentTime + milliseconds){
	}
}
