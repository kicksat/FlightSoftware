
#include "RTCCounter.h"

Counter watchdogTimer; // creates timer object
Counter beaconTimer; // creates timer object

void watchdog() {
  SerialUSB.println("Watchdog");
}

void setup() {
  if (!SerialUSB) {
    SerialUSB.begin(115200); // Restart SerialUSB
    while(!SerialUSB); // Wait for serial USB port to open
    SerialUSB.println("Serial Initialized");
    delay(5000); // Provides user with time to open Serial Monitor
  }
  watchdogTimer.init(1000,watchdog); // timer delay, milliseconds
  SerialUSB.println("Watchdog Timer Initialized");
  beaconTimer.init(10000); // timer delay, milliseconds
  SerialUSB.println("Beacon Timer Initialized");
  SerialUSB.println("Setup Complete");
}

void loop() {
  if (beaconTimer.check()) { // Checks time for interrupt
    SerialUSB.println("Beacon things!");
  }
  timeout.start(5000); // start a timeout timer delay, milliseconds
  while(1) {
    if (timeout.triggered()) { // Checks time for timeout
      SerialUSB.println("TIMEOUT");
      break;
    }
  }
  // sleepTimer.sleep(); // Go into sleep mode until next interrupt
}
