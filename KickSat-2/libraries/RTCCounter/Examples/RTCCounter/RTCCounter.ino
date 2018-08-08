
#include "RTCCounter.h"

Counter watchdogTimer; // creates timer object
Counter beaconTimer; // creates timer object
Counter funTimer; // creates timer object

void watchdog() {
  SerialUSB.println("Watchdog");
}

void beacon() {
  SerialUSB.println("Beacon");
}

void wakeup() {
  SerialUSB.println("Wakeup");
}

void setup() {
  if (!SerialUSB) {
    SerialUSB.begin(115200); // Restart SerialUSB
		while(!SerialUSB); // Wait for serial USB port to open
    delay(5000); // Provides user with time to open Serial Monitor
  }
  SerialUSB.println("Serial Initialized");
  watchdogTimer.init(1,watchdog); // timer delay, seconds
  SerialUSB.println("Watchdog Timer Initialized");
  beaconTimer.init(10,beacon); // timer delay, seconds
  SerialUSB.println("Beacon Timer Initialized");
  funTimer.init(3); // timer delay, seconds
  SerialUSB.println("Setup Complete");
}

void loop() {

  delay(1000);
  if (funTimer.update()) { // Checks time for interrupt
    SerialUSB.println("funTimer interrupt");
  }
  sleepTimer.sleep(wakeup); // Go into sleep mode until next interrupt

}
