/*
IMU.h
*/

#ifndef IMU_h
#define IMU_h

// includes from IMU
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG 0x6B // Would be 0x6A if SDO_AG is LOW

#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250 // 250 ms between prints

#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.

// how many decimal places to round floats to!!
#define float_accuracy 6


class imu
{
	public:

	LSM9DS1 imu;

	void read_IMU(float buf[]);

	void getGyro(float buf[]);

	void getAccel(float buf[]);

	void getMag(float buf[]);

	/*
	*  Cool function to make initializing the IMU easy in the setup function!!
	*/
	void init_imu();
};

#endif