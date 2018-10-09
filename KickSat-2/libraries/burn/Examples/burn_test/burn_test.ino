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


  for(int k = 30; k > 0; --k)
  {
    SerialUSB.println(k);
    delay (1000);
  }

  SerialUSB.println("Here we go...");
  burntest.burnAntennaOne();
  delay(5000);
  burntest.burnAntennaTwo();
  delay(5000);

  burntest.burnSpriteOne();
  delay(5000);
  burntest.burnSpriteTwo();
  delay(5000);
  burntest.burnSpriteThree();
}

void loop() {

}

void watchdog() {
  digitalWrite(WDT_WDI, HIGH);
  delayMicroseconds(2);
  digitalWrite(WDT_WDI, LOW);
  //SerialUSB.println("Toggled WDT");
}
