/**
SAMD RTC Timer Lib, RTCCounter.cpp
Purpose: Creates RTC timers for the SAMD micro

@author Ralen Toledo
@version 2.0 08/08/18
*/

#ifndef RTCCOUNTER_H
#define RTCCOUNTER_H

#ifdef ARDUINO_ARCH_AVR // Not supported
#error The library is not compatible with AVR boards
#endif

#if defined(ARDUINO_ARCH_SAMD) // This library only works for SAMD architecture

#include <Arduino.h>
// #include <string.h>


// Define library settings
#define MAXTIMERS         20    // Max number of allowed timers, arbitrarily picked
#define SERIALUSBENABLE   0     // If 1, enable printing to serial
#define RTCCLOCK          1     // Desired frequency of the RTC, in Hz
#define RTCPRESCALER      1024  // Prescalar of RTC, the prescalar must have a base of 2 (e.g. 2^10=1024)


// Declare RTC functions
void configureRTC(void);
void RTC_Handler(void); // RTC handle function, runs on each interrupt
void setClockRTC(uint32_t time);
void setCompareRTC(uint32_t time);
inline bool syncingGCLK(void);
inline bool syncingRTC(void);
void disableRTC(void);
void enableRTC(void);
void resetFlagRTC(void);
void resetRTC(void);
void removeResetRTC(void);
void incrementTimers(int i);
void compareTriggers(int i);
void interruptCounter(int i);
void interruptSleepTimer(int i);
void enterSleep(void);
typedef void(*voidFuncPtr)(void); // Defines array for function variables


class Timer {
  public: // Define public functions/variables
  int _timerID; // Unique timer ID, defined chronologically starting at 0, increasing by 1 for each timer;
  voidFuncPtr nullCallback = NULL; // Null callback
  void resetTimer(void);
  void resetTimerFlag(void);
  bool update(void);
  void disableTimer(void);
  void enableTimer(void);
  void setTime(uint32_t time);
  void setCallback(voidFuncPtr callback);
  void removeCallback(void);
};


class Counter : public Timer {
  public: // Define public functions/variables
  void init(uint32_t time, voidFuncPtr callback);
  void init(uint32_t time) { init(time, nullCallback); }
  void init(int time, voidFuncPtr callback) { init((uint32_t)time, callback); }
  void init(int time) { init((uint32_t)time, nullCallback); }
};


class SleepTimer {
  public: // Define public functions/variables
  voidFuncPtr _wakeupCallback = NULL; // Wakeup callback
  void sleep(voidFuncPtr callback);
  void sleep() { sleep(_wakeupCallback); }
};


extern SleepTimer sleepTimer; // Creates external class object, accessible in main script


#endif // ARDUINO_ARCH_SAMD
#endif // RTCCOUNTER_H
