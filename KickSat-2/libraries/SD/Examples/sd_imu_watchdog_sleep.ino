// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "sd_log.h"
#include <SD.h>
#include <ax25.h>
#include "RTCCounter.h"

//////////////////////////
// LSM9DS1 Library Init //
//////////////////////////
// Use the LSM9DS1 class to create an object. [imu] can be
// named anything, we'll refer to that throught the sketch.
LSM9DS1 imu;
sd_log mySd_log;
Counter watchdogTimer; // creates timer object
Counter beaconTimer; // creates timer object


///////////////////////
// Example I2C Setup //
///////////////////////
// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG 0x6B // Would be 0x6A if SDO_AG is LOW

////////////////////////////
// Sketch Output Settings //
////////////////////////////
#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250 // 250 ms between prints

// Earth's magnetic field varies by location. Add or subtract
// a declination to get a more accurate heading. Calculate
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.


int current_log = 0;
int iteration = 0;
int n = 5;
float threshold = 2.5;
float v = 0.000;

void watchdog() {
  SerialUSB.println("Watchdog");
}


void setup()
{
 SerialUSB.begin(115200);
 delay(3000);

 init_imu();

 mySd_log.sd_init();

  watchdogTimer.init(1,watchdog); // timer delay, seconds
  beaconTimer.init(10); // timer delay, seconds

}

void loop()
{

  if (beaconTimer.update()) { // Checks time for interrupt
    SerialUSB.println("Beacon things!");
  }

 float batt_level = read_battery();

 read_IMU(mySd_log.logData.IMU_data);


 mySd_log.sd_refresh();
 mySd_log.logData.log_num = current_log;
 mySd_log.write_log(mySd_log.logData);
 SerialUSB.print("Wrote Log ");
 SerialUSB.println(mySd_log.logData.log_num);
 SerialUSB.println(mySd_log.read_entry(current_log));
 delay(1000);
 mySd_log.sd_end();

 current_log++;
 sleepTimer.sleep(); // Go into sleep mode until next interrupt

}

void read_IMU(float buf[]){
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

void getGyro(float buf[])
{
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
 SerialUSB.print(imu.calcGyro(imu.gx), 2);
 SerialUSB.print(", ");
 SerialUSB.print(imu.calcGyro(imu.gy), 2);
 SerialUSB.print(", ");
 SerialUSB.println(imu.calcGyro(imu.gz), 2);
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

void getAccel(float buf[])
{
 // To read from the accelerometer, you must first call the
 // readAccel() function. When this exits, it'll update the
 // ax, ay, and az variables with the most current data.
 imu.readAccel();

 // Now we can use the ax, ay, and az variables as we please.
 // Either print them as raw ADC values, or calculated in g's.
 SerialUSB.print("A: ");
#ifdef PRINT_CALCULATED
 // If you want to print calculated values, you can use the
 // calcAccel helper function to convert a raw ADC value to
 // g's. Give the function the value that you want to convert.
 SerialUSB.print(imu.calcAccel(imu.ax), 2);
 SerialUSB.print(", ");
 SerialUSB.print(imu.calcAccel(imu.ay), 2);
 SerialUSB.print(", ");
 SerialUSB.println(imu.calcAccel(imu.az), 2);
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

void getMag(float buf[])
{
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
 SerialUSB.print(imu.calcMag(imu.mx), 2);
 SerialUSB.print(", ");
 SerialUSB.print(imu.calcMag(imu.my), 2);
 SerialUSB.print(", ");
 SerialUSB.println(imu.calcMag(imu.mz), 2);
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

// Calculate pitch, roll, and heading.
// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
 float roll = atan2(ay, az);
 float pitch = atan2(-ax, sqrt(ay * ay + az * az));

 float heading;
 if (my == 0)
   heading = (mx < 0) ? PI : 0;
 else
   heading = atan2(mx, my);

 heading -= DECLINATION * PI / 180;

 if (heading > PI) heading -= (2 * PI);
 else if (heading < -PI) heading += (2 * PI);
 else if (heading < 0) heading += 2 * PI;

 // Convert everything from radians to degrees:
 heading *= 180.0 / PI;
 pitch *= 180.0 / PI;
 roll  *= 180.0 / PI;

 SerialUSB.print("Pitch, Roll: ");
 SerialUSB.print(pitch, 2);
 SerialUSB.print(", ");
 SerialUSB.println(roll, 2);
 SerialUSB.print("Heading: "); SerialUSB.println(heading, 2);
}

/*
*  Cool function to make initializing the IMU easy in the setup function!!
*/
void init_imu() {
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

/*
* Read battery function for reading the battery voltage!
*/
float read_battery() {
//  int batt_val = analogRead(VBATT);
//  float batt_voltage = batt_val * (3.3 / 1023.0) * (426.0/110.0);
//  SerialUSB.println("VBATT: ");
//  SerialUSB.println(batt_voltage);
 return 5;
}
