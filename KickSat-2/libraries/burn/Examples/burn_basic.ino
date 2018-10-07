#include <RTCCounter.h>
#include <burn.h>
Counter watchdogTimer;
burn burntest;

void setup() {
  pinMode(WDT_WDI, OUTPUT); // Set watchdog pin mode to output
  digitalWrite(WDT_WDI, LOW);
  watchdogTimer.init(1000, watchdog); // timer delay, milliseconds

  
  SerialUSB.begin(115200); // Restart SerialUSB
  while(!SerialUSB); // Wait for SerialUSB USB port to open
  SerialUSB.println("SerialUSB Initialized");
  
  delay (1000);
}
int cnt;
void loop() {
  SerialUSB.println(cnt);
  burntest.burnSpriteOne(); //CHOOSE WHICH WIRE TO BURN
  delay(3000);
  cnt++;
}

void watchdog() {
  digitalWrite(WDT_WDI, HIGH);
  delayMicroseconds(2);
  digitalWrite(WDT_WDI, LOW);
  SerialUSB.println("Toggled WDT");
}
