
#include "Arduino.h"
#include "InterruptTimers.h"

timer timer0; // creates timer object
timer timer1; // creates timer object

void setup() {
  SerialUSB.begin(115200);
  delay(5000);
  SerialUSB.println("Serial Initialized");
  // startSysTimer(); // Initiates the system timer without a specific timer object, included in timer0.init() call
  timer0.init(1); // timer delay, seconds (int)
  // timer1.init(3); // timer delay, seconds (int)
  SerialUSB.println("Timer initialized");
}

void loop() {

  // if (timer0.update()) { // Checks time for interrupt
  //   SerialUSB.println("timer0 interrupt");
  // }
  // if (timer1.update()) { // Checks time for interrupt
  //   SerialUSB.println("timer1 interrupt");
  // }

}
