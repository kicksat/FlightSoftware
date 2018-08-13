#include "KickSatLog.h"

char buf[MAXCHARS]; // Create global variable for buffer from SD read function, this can be piped into radio.send()

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB);
  SerialUSB.println("Serial Initialized");
  delay(5000);


  if(logfile.init()) {
    SerialUSB.println("logfile initialized");
  } else {
    SerialUSB.println("logfile not initialized");
  }

}

void loop() {

  data.status = random(0,10);
  for(uint8_t i = 0; i < 3; i++){
    data.powerData[i] = random(0,100);
  }
  for(uint8_t i = 0; i < 4; i++){
    data.gpsData[i] = random(0,200)/13.87;
  }
  for(uint8_t i = 0; i < 9; i++){
    data.imuData[i] = random(0,100)/9.123;
  }
  for(uint8_t i = 0; i < 8; i++){
    data.commandData[i] = random(0,9);
  }

  if(logfile.available()) {
    SerialUSB.println("logfile available for appending data");
    logfile.appendData();
  } else {
    SerialUSB.println("logfile not available");
  }

  if(logfile.available()) {
    SerialUSB.println("logfile available for reading");
    logfile.read(5, buf);
    SerialUSB.println("Printing from buf");
    SerialUSB.println(buf);
    SerialUSB.println("End printing from buf");
  } else {
    SerialUSB.println("logfile not available");
  }

  if(logfile.available()) {
    SerialUSB.println("logfile available for reading health");
    logfile.compileHealth(buf);
    SerialUSB.println("Printing from buf for health");
    SerialUSB.println(buf);
    SerialUSB.println("End printing from buf for health");
  } else {
    SerialUSB.println("logfile not available");
  }

  if(logfile.available()) {
    SerialUSB.println("logfile available for appending buffer");
    logfile.appendBuffer("HELLO WORLD!!!!!!!");
  } else {
    SerialUSB.println("logfile not available");
  }

  delay(2000);

}
