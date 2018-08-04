/**
    SAMD Timer Lib, InterruptTimers.cpp
    Purpose: Creates timers for the SAMD micro

    @author Ralen Toledo
    @version 1.0 08/03/18 
*/

#include <Arduino.h>
#include <string.h>
#include "InterruptTimers.h"


// Define/initialize variables
bool sysTimerState = 0; // defines state of the system (millisecond) timer; 1=ON, 0=OFF
bool timerFlag = 0; // Timer flag
uint32_t timerCounters [MAXNUMTIMERS]; // Iterating counter for each timer, in seconds
int timerTriggers [MAXNUMTIMERS]; // User defined triggers, in seconds
uint32_t timerLastInterrupt [MAXNUMTIMERS]; // Time since last trigger, in seconds
uint32_t timerOverflow [MAXNUMTIMERS]; // Allows storage of counters in the case of overflow
bool overflowed [MAXNUMTIMERS]; // Flag for overflow
bool timerFlags [MAXNUMTIMERS]; // Flags for each timer, returns true if trigger is reached
int numberOfTimers = 0; // Iterates number of timers initialized


void timer::init(int requestedTimerDelay) { // Initializes timer
  if (!sysTimerState) { // If the system timer is not already started
    startSysTimer(); // Starts system millisecond timer
    SerialUSB.println("System timer started");
  }

  timerID = numberOfTimers; // Defines ID of timer
  numberOfTimers++; // Iterates number of timers initialized
  timerCounters[timerID] = 0; // Iterating counter for each timer, in seconds
  timerTriggers[timerID] = requestedTimerDelay; // User defined triggers, in seconds
  timerLastInterrupt[timerID] = 0; // Time since last trigger, in seconds
  timerOverflow[timerID] = 0; // Sets overflow to 0, only to be used in case of an overflow, in seconds
  overflowed[timerID] = 0; // flag for overflow
  timerFlags[timerID] = 0; // Flags for each timer, returns true if trigger is reached

}


void timer::resetTimer() { // Resets timer flag and counters

  timerCounters[timerID] = 0; // Iterating counter for each timer, in seconds
  timerLastInterrupt[timerID] = 0; // Time since last trigger, in seconds
  timerFlags[timerID] = 0; // Flags for each timer, returns true if trigger is reached

}


void timer::resetTimerFlag() { // Resets timer flag

  timerFlags[timerID] = 0; // Flags for each timer, returns true if trigger is reached

}


bool timer::update() { // Checks flag of timer

  if (timerFlags[timerID]) {
    timerFlags[timerID] = 0;
    return true;
  } else {
    return false;
  }

}


void startSysTimer() {

  if (!sysTimerState) { // If the system timer is not already started

    // Initialize arrays
    memset(timerCounters, 0, sizeof(timerCounters)); // Sets entire array to 0
    memset(timerTriggers, 0, sizeof(timerTriggers)); // Sets entire array to 0
    memset(timerLastInterrupt, 0, sizeof(timerLastInterrupt)); // Sets entire array to 0
    memset(timerOverflow, 0, sizeof(timerOverflow)); // Sets entire array to 0
    memset(overflowed, 0, sizeof(overflowed)); // Sets entire array to 0
    memset(timerFlags, 0, sizeof(timerFlags)); // Sets entire array to 0

    // Initialize timer TC5
    GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5));
    while ( GCLK->STATUS.bit.SYNCBUSY);

    tcReset(); //reset TC5

    // Use the 16-bit timer
    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;

    // Use match mode so that the timer counter resets when the count matches the compare register
    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;

    // Set prescaler to 1024
    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;

    // Enable TC5
    TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;


    // Sets timer frequency
    TC5->COUNT16.CC[0].reg = (SystemCoreClock / (TIMER_PRESCALER_DIV * FREQUENCY)) - 1;
    while (tcIsSyncing()); // wait until TC5 is done syncing

    // Configure interrupt request
    NVIC_DisableIRQ(TC5_IRQn);
    NVIC_ClearPendingIRQ(TC5_IRQn);
    NVIC_SetPriority(TC5_IRQn, 0);
    NVIC_EnableIRQ(TC5_IRQn);

    // Enable the compare interrupt request
    TC5->COUNT16.INTENSET.bit.MC0 = 1;

    while (tcIsSyncing()); // wait until TC5 is done syncing

    // Records initialization of the timer
    sysTimerState = true;
  }

}

// Function that is used to check if TC5 is done syncing
// returns true when it is done syncing
bool tcIsSyncing() {
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

// This function enables TC5 and waits for it to be ready
void tcStartCounter() {
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; //set the CTRLA register
  while (tcIsSyncing()); //wait until snyc'd
}

// Reset TC5
void tcReset() {
  TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (tcIsSyncing());
  while (TC5->COUNT16.CTRLA.bit.SWRST);
}

// Disable TC5
void tcDisable() {
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());

}


void TC5_Handler() {

  /////////////////////////////
  // BEGIN USER HANDLER CODE //
  /////////////////////////////

  ////////////////////////////////////
  // Begin iterating timer counters //
  ////////////////////////////////////

  for (size_t i = 0; i < numberOfTimers; i++) {

    if (timerLastInterrupt[i] > OVERFLOW) {timerLastInterrupt[i] = OVERFLOW;} // Catch unexpected values for last interrupt time

    if (overflowed[i]) { // Handle overflowed counters

      timerCounters[i]++; // Increment counter
      timerOverflow[i]++; // Increment overflow counter, getting closer to last interrupt time
      if (timerOverflow[i] >= timerTriggers[i]) { // Handle residual overflowed counters
        timerFlags[i] = 1; // Set interrupt flag
        timerLastInterrupt[i] = timerCounters[i]; // Update last interrupt time
        overflowed[i] = 0; // Reset overflow flag
        timerOverflow[i] = 0; // Reset overflow counter
      }

    } else { // Nominal case, not overflowed

      if (timerCounters[i] < OVERFLOW) { // overflow value for 32 bit unsigned integers, nominal case
        timerCounters[i]++; // Increment counter
        if (timerCounters[i]-timerLastInterrupt[i] >= timerTriggers[i]) { // If trigger is hit
          timerFlags[i] = 1; // Set interrupt flag
          timerLastInterrupt[i] = timerCounters[i]; // Update last interrupt time
        }
      } else { // overflow value for 32 bit unsigned integers, catch overflowed counters
        timerOverflow[i] = timerCounters[i]+1-timerLastInterrupt[i]; // Set overflow counter
        timerCounters[i] = 0; // Reset counter
        overflowed[i] = 1; // Set overflow flag
      }

    }

  }
  //////////////////////////////////
  // End iterating timer counters //
  //////////////////////////////////

  // WatchDog here

  ///////////////////////////
  // END USER HANDLER CODE //
  ///////////////////////////

  TC5->COUNT16.INTFLAG.bit.MC0 = 1; //don't change this, it's part of the timer code
}
