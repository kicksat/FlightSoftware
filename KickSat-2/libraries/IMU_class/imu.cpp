/*
IMU CLASS
*/

#include "imu.h"



void imu::read_IMU(float buf[])
{
  float gyroData[3];
  float accelData[3];
  float magData[3];
  getGyro(gyroData);
  getAccel(accelData);
  getMag(magData);
  buf[0] = gyroData[0];
  buf[1] = gyroData[1];
  buf[2] = gyroData[2];
  buf[3] = accelData[0];
  buf[4] = accelData[1];
  buf[5] = accelData[2];
  buf[6] = magData[0];
  buf[7] = magData[1];
  buf[8] = magData[2];
}

void imu::getGyro(float buf[])
{

	if(imu.gyroAvailable())
	{

	}
	else
	{
		SerialUSB.println("gyro unavailable");
		return;
	}
 // To read from the gyroscope, you must first call the
 // readGyro() function. When this exits, it'll update the
 // gx, gy, and gz variables with the most current data.
 imu.readGyro();

 // Now we can use the gx, gy, and gz variables as we please.
 // Either print them as raw ADC values, or calculated in DPS.
 SerialUSB.print("G: ");
#ifdef PRINT_CALCULATED
 // If you want to print calculated values, you can use the
 // calcGyro helper function to convert a raw ADC value to
 // DPS. Give the function the value that you want to convert.
 SerialUSB.print(imu.calcGyro(imu.gx), float_accuracy);
 SerialUSB.print(", ");
 SerialUSB.print(imu.calcGyro(imu.gy), float_accuracy);
 SerialUSB.print(", ");
 SerialUSB.println(imu.calcGyro(imu.gz), float_accuracy);
 buf[0] = imu.calcGyro(imu.gx);
 buf[1] = imu.calcGyro(imu.gy);
 buf[2] = imu.calcGyro(imu.gz);
#elif defined PRINT_RAW
 SerialUSB.print(imu.gx);
 SerialUSB.print(", ");
 SerialUSB.print(imu.gy);
 SerialUSB.print(", ");
 SerialUSB.println(imu.gz);
 buf[0] = imu.gx;
 buf[1] = imu.gy;
 buf[2] = imu.gz;
#endif
}

void imu::getAccel(float buf[])
{
 // To read from the accelerometer, you must first call the
 // readAccel() function. When this exits, it'll update the
 // ax, ay, and az variables with the most current data.
	if(imu.accelAvailable())
	{

	}
	else
	{
		SerialUSB.println("accel unavailable");
		return;
	}
 imu.readAccel();

 // Now we can use the ax, ay, and az variables as we please.
 // Either print them as raw ADC values, or calculated in g's.
 SerialUSB.print("A: ");
#ifdef PRINT_CALCULATED
 // If you want to print calculated values, you can use the
 // calcAccel helper function to convert a raw ADC value to
 // g's. Give the function the value that you want to convert.
 SerialUSB.print(imu.calcAccel(imu.ax), float_accuracy);
 SerialUSB.print(", ");
 SerialUSB.print(imu.calcAccel(imu.ay), float_accuracy);
 SerialUSB.print(", ");
 SerialUSB.println(imu.calcAccel(imu.az), float_accuracy);
 buf[0] = imu.calcAccel(imu.ax);
 buf[1] = imu.calcAccel(imu.ay);
 buf[2] = imu.calcAccel(imu.az);
#elif defined PRINT_RAW
 SerialUSB.print(imu.ax);
 SerialUSB.print(", ");
 SerialUSB.print(imu.ay);
 SerialUSB.print(", ");
 SerialUSB.println(imu.az);
 buf[0] = imu.ax;
 buf[1] = imu.ay;
 buf[2] = imu.az;
#endif

}

void imu::getMag(float buf[])
{
	if(imu.magAvailable())
	{

	}
	else
	{
		SerialUSB.println("mag unavailable");
		return;
	}
 // To read from the magnetometer, you must first call the
 // readMag() function. When this exits, it'll update the
 // mx, my, and mz variables with the most current data.
 imu.readMag();

 // Now we can use the mx, my, and mz variables as we please.
 // Either print them as raw ADC values, or calculated in Gauss.
 SerialUSB.print("M: ");
#ifdef PRINT_CALCULATED
 // If you want to print calculated values, you can use the
 // calcMag helper function to convert a raw ADC value to
 // Gauss. Give the function the value that you want to convert.
 SerialUSB.print(imu.calcMag(imu.mx), float_accuracy);
 SerialUSB.print(", ");
 SerialUSB.print(imu.calcMag(imu.my), float_accuracy);
 SerialUSB.print(", ");
 SerialUSB.println(imu.calcMag(imu.mz), float_accuracy);
 buf[0] = imu.calcMag(imu.mx);
 buf[1] = imu.calcMag(imu.my);
 buf[2] = imu.calcMag(imu.mz);
#elif defined PRINT_RAW
 SerialUSB.print(imu.mx);
 SerialUSB.print(", ");
 SerialUSB.print(imu.my);
 SerialUSB.print(", ");
 SerialUSB.println(imu.mz);
 buf[0] = imu.mx;
 buf[1] = imu.my;
 buf[2] = imu.mz;
#endif
}

/*
*  Cool function to make initializing the IMU easy in the setup function!!
*/
void imu::init_imu() 
{
 // Before initializing the IMU, there are a few settings
 // we may need to adjust. Use the settings struct to set
 // the device's communication mode and addresses:
 imu.settings.device.commInterface = IMU_MODE_I2C;
 imu.settings.device.mAddress = LSM9DS1_M;
 imu.settings.device.agAddress = LSM9DS1_AG;
 // The above lines will only take effect AFTER calling
 // imu.begin(), which verifies communication with the IMU
 // and turns it on.
 if (!imu.begin()) {
   SerialUSB.println("Failed to communicate with LSM9DS1.");
   SerialUSB.println("Double-check wiring.");
   SerialUSB.println("Default settings in this sketch will " \
                 "work for an out of the box LSM9DS1 " \
                 "Breakout, but may need to be modified " \
                 "if the board jumpers are.");
   while (1) {
     // stay here
     SerialUSB.println("Imu did not init");
     delay(5000);
   }
 }
}