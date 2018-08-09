/**
SAMD RTC Timer Lib, RTCCounter.cpp
Purpose: Creates RTC timers for the SAMD micro

@author Ralen Toledo
@version 2.0 08/08/18
*/

#if defined(ARDUINO_ARCH_SAMD) // This library only works for SAMD architecture

// Includes
// #include <time.h>
#include <Arduino.h>
#include "RTCCounter.h"

//////////////////
/* DO NOT CHANGE*/
#define OVERFLOW          4294967295  // 32-bit overflow
#define OSCULP32KCLOCK    32768       // Frequency of the OSCULP32K clock source, in Hz
/* DO NOT CHANGE*/
//////////////////

///////////////////////////////////
// BEGIN VARIABLE INITIALIZATION //
///////////////////////////////////

// Declare/initialize variables
bool _configuredRTC = false; // Defines state of the RTC; false = not configured
uint32_t timerCounter [MAXTIMERS]; // Iterating counter for each timer, in seconds
uint32_t timerTrigger [MAXTIMERS]; // User defined triggers, in seconds
uint32_t timerLast [MAXTIMERS]; // Time of last trigger, in seconds
uint32_t timerOverflow [MAXTIMERS]; // Allows storage of counters in the case of overflow
bool overflowed [MAXTIMERS]; // Flag for overflow
bool timerFlag [MAXTIMERS]; // Flags for each timer, returns true if trigger is reached
bool timerEnable [MAXTIMERS]; // Enables for each timer, returns true if timer is enables
int numberOfTimers = 0; // Iterates number of timers initialized
bool sleepState = false;
uint32_t debugCounter = 0; // Counter that increments every time RTC_Handler is called, for debugging only
voidFuncPtr timerCallback[MAXTIMERS]; // Array of callback pointers for timers

/////////////////////////////////
// END VARIABLE INITIALIZATION //
/////////////////////////////////

/////////////////////////////////
// BEGIN TIME OBJECT FUNCTIONS //
/////////////////////////////////

void Timer::resetTimer() { // Resets timer flag and counters, but not callback or trigger
  timerCounter[_timerID] = 0; // Iterating counter for each timer, in seconds
  timerLast[_timerID] = 0; // Time since last trigger, in seconds
  timerOverflow[_timerID] = 0; // Sets overflow to 0, only to be used in case of an overflow, in seconds
  overflowed[_timerID] = 0; // flag for overflow
  timerFlag[_timerID] = false; // Flags for each timer, returns true if trigger is reached
  timerEnable[_timerID] = true; // Enables for each timer, returns true if timer is enables
  if (SERIALUSBENABLE) {
    SerialUSB.print("Reset:");SerialUSB.println(_timerID);
  }
}

void Timer::resetTimerFlag() { // Resets timer flag
  timerFlag[_timerID] = false; // Flags for each timer, returns true if trigger is reached
  if (SERIALUSBENABLE) {
    SerialUSB.print("FlagReset:");SerialUSB.println(_timerID);
  }
}

bool Timer::update() { // Checks flag of timer
  if (timerFlag[_timerID]) { // If flag is true
    timerFlag[_timerID] = false; // Reset flag
    return true; // Return flag state
  } else {
    return false; // Return flag state
  }
}

void Timer::disableTimer() { // Disables timers
  timerEnable[_timerID] = false; // Disable timer
  if (SERIALUSBENABLE) {
    SerialUSB.print("Disabled:");SerialUSB.println(_timerID);
  }
}

void Timer::enableTimer() { // Enables timer
  timerEnable[_timerID] = true; // Enable timer
  if (SERIALUSBENABLE) {
    SerialUSB.print("Enabled:");SerialUSB.println(_timerID);
  }
}

void Timer::setTime(uint32_t time) { // Sets new trigger time for timer
  timerTrigger[_timerID] = time; // Sets trigger time
  if (SERIALUSBENABLE) {
    SerialUSB.print("SetTime:");SerialUSB.println(_timerID);
  }
}

void Timer::setCallback(voidFuncPtr callback) { // Stores callback function
  timerCallback[_timerID] = callback; // Stores callback
  if (SERIALUSBENABLE) {
    SerialUSB.print("SetCallback:");SerialUSB.println(_timerID);
  }
}

void Timer::removeCallback() { // Removes callback function
  timerCallback[_timerID] = NULL; // Removes callback
  if (SERIALUSBENABLE) {
    SerialUSB.print("RemovedCallback:");SerialUSB.println(_timerID);
  }
}

///////////////////////////////
// END TIME OBJECT FUNCTIONS //
///////////////////////////////

////////////////////////////////////
// BEGIN COUNTER OBJECT FUNCTIONS //
////////////////////////////////////

void Counter::init(uint32_t time, voidFuncPtr callback) { // Initializes timer
  if (!_configuredRTC) { // If the RTC is not already started
    configureRTC(); // Configure RTC
  }
  _timerID = numberOfTimers; // Defines ID of timer
  numberOfTimers++; // Iterates number of timers initialized
  timerCounter[_timerID] = 0; // Iterating counter for each timer, in seconds
  timerTrigger[_timerID] = time; // User defined triggers, in seconds
  timerLast[_timerID] = 0; // Time since last trigger, in seconds
  timerOverflow[_timerID] = 0; // Sets overflow to 0, only to be used in case of an overflow, in seconds
  overflowed[_timerID] = 0; // flag for overflow
  timerFlag[_timerID] = false; // Flags for each timer, returns true if trigger is reached
  timerEnable[_timerID] = true; // Enables for each timer, returns true if timer is enables
  timerCallback[_timerID] = callback; // Stores callback
  if (SERIALUSBENABLE) {
    SerialUSB.print("Timer Object Configured:");SerialUSB.println(_timerID);
  }
}

//////////////////////////////////
// END COUNTER OBJECT FUNCTIONS //
//////////////////////////////////

///////////////////////////////////////
// BEGIN SLEEPTIMER OBJECT FUNCTIONS //
///////////////////////////////////////

void SleepTimer::sleep(voidFuncPtr callback) { // Initializes timer
  if (SERIALUSBENABLE) {
    SerialUSB.println("Entering Sleep Mode:");
  }
  if (!_configuredRTC) { // If the RTC is not already started
    configureRTC(); // Configure RTC
  }
  _wakeupCallback = callback; // Store callback
  sleepState = true; // Turn on sleep state
  enterSleep(); // Enter sleep mode
}

void enterSleep() { // Enters sleep mode: stops CPU, interrupts and regulator continue
  USBDevice.standby(); // Disable USB in sleep mode
  SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk; // Disables deep sleep mode
  PM->SLEEP.reg = 2; // Sets Idle Mode 2, most power saving
  __DSB(); // Data Synchronization Barrier, waits for previous commands to complete
  __WFI(); // Wait For Interupt, sleep in the meantime
  if (SERIALUSBENABLE) {
    USB->DEVICE.CTRLA.reg |= USB_CTRLA_ENABLE; // Enable USB
    if (!SerialUSB) { // Check if Serial is initiated
      SerialUSB.begin(115200); // Restart SerialUSB
      while(!SerialUSB); // Wait for serial USB port to open
    }
    SerialUSB.println("Exiting Sleep Mode:");
  }
}

/////////////////////////////////////
// END SLEEPTIMER OBJECT FUNCTIONS //
/////////////////////////////////////

/////////////////////////
// BEGIN RTC FUNCTIONS //
/////////////////////////

void configureRTC() { // Attach clock to 32kHz (32768Hz) Ultra Low Power Internal Oscillator (OSCULP32K)
  if (!_configuredRTC) { // If the system timer is not already started

    if (SERIALUSBENABLE) {
      SerialUSB.println("Configuring RTC");
    }

    // Initialize arrays
    memset(timerCounter, 0, sizeof(timerCounter)); // Sets entire array to 0
    memset(timerTrigger, 0, sizeof(timerTrigger)); // Sets entire array to 0
    memset(timerLast, 0, sizeof(timerLast)); // Sets entire array to 0
    memset(timerOverflow, 0, sizeof(timerOverflow)); // Sets entire array to 0
    memset(overflowed, 0, sizeof(overflowed)); // Sets entire array to 0
    memset(timerFlag, 0, sizeof(timerFlag)); // Sets entire array to 0
    memset(timerEnable, 0, sizeof(timerEnable)); // Sets entire array to 0

    // Configure generic clock for the RTC using 32kHz (32768Hz) Ultra Low Power Internal Oscillator (OSCULP32K)
    GCLK->GENCTRL.reg = (uint32_t)(GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_RUNSTDBY);
    while (syncingGCLK()); // wait until GCLK is done syncing
    GCLK->CLKCTRL.reg = (uint16_t)(GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2 | GCLK_CLKCTRL_ID_RTC);

    // Configure RTC in Mode 0 for only compare match interupt events
    resetRTC(); // Reset RTC timer
    RTC->MODE0.INTENCLR.reg |= RTC_MODE0_INTENCLR_CMP0; // Clear the Compare 0 Interrupt Enable bit and disable the corresponding interrupt
    RTC->MODE0.COMP[0].reg = (uint32_t)((OSCULP32KCLOCK / (RTCPRESCALER * RTCCLOCK)) - 1); // Sets the counter compare value
    while (syncingRTC()); // wait until GCLK is done syncing
    RTC->MODE0.CTRL.reg = (uint16_t)(RTC_MODE0_CTRL_ENABLE | ((uint8_t)(log(RTCPRESCALER)/log(2)) << RTC_MODE0_CTRL_PRESCALER_Pos) | RTC_MODE0_CTRL_MATCHCLR); // Sets prescalar, enables match compare reset and enables Mode 0
    while (syncingRTC()); // wait until RTC is done syncing
    NVIC_DisableIRQ(RTC_IRQn); // Disable interrupt
    NVIC_ClearPendingIRQ(RTC_IRQn); // Clear all pending interrupts
    NVIC_SetPriority(RTC_IRQn, 0); // Set timer with max priority
    NVIC_EnableIRQ(RTC_IRQn); // Enable interrupt
    RTC->MODE0.INTENSET.reg |= RTC_MODE0_INTENSET_CMP0; // Set the Compare 0 Interrupt Enable bit and enable the Compare 0 interrupt
    _configuredRTC = true; // Records configuration state of RTC

    if (SERIALUSBENABLE) {
      SerialUSB.println("RTC Timer Enabled");
    }
  }
}

void setClockRTC(uint32_t time) { // Sets compare time
  RTC->MODE0.COUNT.reg = RTC_MODE0_COUNT_COUNT(time); // Clears the Compare 0 interrupt flag
  while (syncingRTC()); // wait until GCLK is done syncing
}

void setCompareRTC(uint32_t time) { // Sets compare time
  RTC->MODE0.COMP[0].reg = RTC_MODE0_COMP_COMP(time); // Clears the Compare 0 interrupt flag
  while (syncingRTC()); // wait until GCLK is done syncing
}

inline bool syncingGCLK() { // wait until GCLK is done syncing
  return (GCLK->STATUS.bit.SYNCBUSY);
}

inline bool syncingRTC() { // wait until RTC is done syncing
  return (RTC->MODE0.STATUS.bit.SYNCBUSY);
}

void disableRTC() { // Disable RTC
  RTC->MODE0.CTRL.reg &= ~RTC_MODE0_CTRL_ENABLE; // Disable RTC
  while (syncingRTC()); // wait until RTC is done syncing
}

void enableRTC() { // Enable RTC
  RTC->MODE0.CTRL.reg |= RTC_MODE0_CTRL_ENABLE; // Enable RTC
  while (syncingRTC()); // wait until RTC is done syncing
}

void resetFlagRTC() { // Reset interrupt flag for RTC
  RTC->MODE0.INTFLAG.reg |= RTC_MODE0_INTFLAG_CMP0; // Reset interrupt flag for RTC
}

void resetRTC() { // Software reset RTC
  RTC->MODE0.CTRL.reg |= RTC_MODE2_CTRL_SWRST; // Software reset RTC
  while (syncingRTC()); // wait until RTC is done syncing
}

void removeResetRTC() { // Remove software reset of RTC
  RTC->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_SWRST; // Remove software reset of RTC
  while (syncingRTC());
}

void incrementTimers(int i) {
  if (timerEnable[i]) { // If the timer is enabled
    if (timerLast[i] > OVERFLOW) {timerLast[i] = OVERFLOW;} // Catch unexpected values for last interrupt time
    if (overflowed[i]) { // Handle overflowed counters
      timerCounter[i]++; // Increment counter
      timerOverflow[i]++; // Increment overflow counter, getting closer to last interrupt time
    } else { // Nominal case, not overflowed
      if (timerCounter[i] < OVERFLOW) { // overflow value for 32 bit unsigned integers, nominal case
        timerCounter[i]++; // Increment counter
      } else { // overflow value for 32 bit unsigned integers, catch overflowed counters
        timerOverflow[i] = timerCounter[i]+1-timerLast[i]; // Set overflow counter
        timerCounter[i] = 0; // Reset counter
        overflowed[i] = 1; // Set overflow flag
      } // if (timerCounter[i] < OVERFLOW)
    } // if (overflowed[i])
  } // if (timerEnable[i])
}

void compareTriggers(int i) { // Compare trigger to counter value and handles interrupt
  if (timerCounter[i]-timerLast[i] >= timerTrigger[i] || (overflowed[i] && (timerOverflow[i] >= timerTrigger[i]))) { // Catches if the counter is >= the trigger
    timerFlag[i] = 1; // Set interrupt flag
    timerLast[i] = timerCounter[i]; // Update last interrupt time
    overflowed[i] = 0; // Reset overflow flag
    timerOverflow[i] = 0; // Reset overflow counter
    if (timerCallback[i] != NULL) {
      timerCallback[i](); // Calls callback
    }
  }
}

void RTC_Handler() { // Interrupt handler for RTC
  if (sleepState) { // If in sleep mode
    sleepState = false; // Disable sleep state
    if (sleepTimer._wakeupCallback != NULL) {
      sleepTimer._wakeupCallback();
    }
  }
  for (size_t i = 0; i < numberOfTimers; i++) { // Begin iterating timer counters
    incrementTimers(i); // Increment (counters)
    compareTriggers(i); // Compare trigger to counter value and calls interrupt handler accordingly
  } // End iterating timer counters
  resetFlagRTC(); // Clear RTC interrupt flag
}

///////////////////////
// END RTC FUNCTIONS //
///////////////////////

// Create class objects
SleepTimer sleepTimer;

#endif // ARDUINO_ARCH_SAMD
