/*
IMUHandler - Gathers and formats imu data from the LSM9DS1 using the SparkFunLSM9DS1 lib

 Last update on: 8-11-18
             by: Ralen
*/

#ifndef IMU_h
#define IMU_h

#include <Wire.h>
#include <SPI.h>
#include "SparkFunLSM9DS1.h"
#include "RTCCounter.h"

class IMUHandle {
	public:
	LSM9DS1 LSM9DS1Sensor; // Create IMU object
	void read(float buf[]);
	void getGyro(float buf[]);
	void getAccel(float buf[]);
	void getMag(float buf[]);
	uint16_t begin() {
	  return LSM9DS1Sensor.begin();
	}
};

#endif
