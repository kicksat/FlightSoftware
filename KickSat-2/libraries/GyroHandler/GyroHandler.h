/*
GyroHandler - gathers data from the 9DOF Gyro using the FXAS21002C Adafruit libraries

 Last update on: 16-9-18
             by: Andrea
*/

#ifndef GYRO_h
#define GYRO_h

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXAS21002C.h>

class GyroHandle {
	public:
	Adafruit_FXAS21002C gyroSensor = Adafruit_FXAS21002C(0x0021002C); // Create gyro object
	void read(float buf[]) {
		sensors_event_t event;
  		gyroSensor.getEvent(&event);
  		buf[0] = event.gyro.x;
  		buf[1] = event.gyro.y;
  		buf[2] = event.gyro.z;
	}
	uint16_t begin() {
	  return gyroSensor.begin();
	}
};

#endif