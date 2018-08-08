
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
    delay(5000); // Provides user with time to open Serial Monitor
  }
  SerialUSB.println("Serial Initialized");
  watchdogTimer.init(1,watchdog); // timer delay, seconds
  SerialUSB.println("Watchdog Timer Initialized");
  beaconTimer.init(10); // timer delay, seconds
  SerialUSB.println("Beacon Timer Initialized");
  SerialUSB.println("Setup Complete");
}

void loop() {

  if (beaconTimer.update()) { // Checks time for interrupt
    SerialUSB.println("Beacon things!");
  }
  delay(1000);
  sleepTimer.sleep(); // Go into sleep mode until next interrupt

}
