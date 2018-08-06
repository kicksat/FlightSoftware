#include "sd_sensor_log.h"
#include "wiring_private.h"

void setup() {
  SerialUSB.begin(9600);
  delay(2000);
  pinPeripheral(7, PIO_SERCOM_ALT);
  SD.begin(10);
  

  sd_sensor_log mySensorLog;
  byte buf1[3] = {'o', 'n', 'e'};
  byte buf2[3] = {'t', 'w', 'o'};
  byte readBuf1[3];
  byte readBuf2[3];
  bool result = mySensorLog.write_sensor(buf1, 1);
  mySensorLog.write_sensor(buf2, 2);
  SerialUSB.println(result);
  mySensorLog.read_sensor(readBuf1, 1, 0);
  mySensorLog.read_sensor(readBuf2, 2, 0);
  SerialUSB.println("sensor 1 line 0:");
  for(int i = 0; i < 3; i++){
    SerialUSB.print((char)readBuf1[i]);
  }
  SerialUSB.println("sensor 2 line 0:");
  for(int i = 0; i < 3; i++){
    SerialUSB.print((char)readBuf2[i]);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
