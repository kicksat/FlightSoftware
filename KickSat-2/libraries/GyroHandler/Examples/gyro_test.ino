#include <GyroHandler.h>

GyroHandle gyroscope;

void setup() {
  SerialUSB.begin(115200);
  while (!SerialUSB);

  SerialUSB.println("Gyroscope Test"); SerialUSB.println("");

  /* Initialise the sensor */
  if(!gyroscope.begin()){
    /* There was a problem detecting the FXAS21002C ... check your connections */
    SerialUSB.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while(1);
  } else {
    SerialUSB.println("Gyro initialized");
  }
}

void loop() {
  /* Get a new sensor event */
  float buf[3];
  gyroscope.read(buf);

  /* Display the results (speed is measured in rad/s) */
  SerialUSB.print("X: "); SerialUSB.print(buf[0]); SerialUSB.print("  ");
  SerialUSB.print("Y: "); SerialUSB.print(buf[1]); SerialUSB.print("  ");
  SerialUSB.print("Z: "); SerialUSB.print(buf[2]); SerialUSB.print("  ");
  SerialUSB.println("rad/s ");
  delay(500);
}
