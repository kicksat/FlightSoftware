#include "Arduino.h"

#ifndef TIMER_H
#define TIMER_H

/*** DO NOT CHANGE ***/
#define TIMER_PRESCALER_DIV 1024 // Sets timer prescalar to 1024, allows largest timer step size, perfect for timers greater than a few seconds
#define FREQUENCY 1 // set timer resolution, in Hz, to be 1 millisecond (arbitrarily selected but 1 ms should be a high enough resolution for most missions)

#define MAXNUMTIMERS 10 // Max number of allowed timers, arbitrarily picked

// Define functions
void startSysTimer(); // Starts system timer
bool tcIsSyncing(); // Waits until system timer is synced
void tcStartCounter(); // Starts system timer counter
void tcReset(); // Resets system timer
void tcDisable(); // disables system timer
void TC5_Handler(); // Timer handler function, runs of each interrupt, custom code goes here


class timer {
  // Define private functions/variables
private:
  int timerID; // Unique timer ID, defined chronologically starting at 0, increasing by 1 for each timer;

  // Define public functions/variables
public:
  void init(int requestedTimerDelay);
  void resetTimer();
  void resetTimerFlag();
  bool update();
};

#endif
