#include <LSM9DS1_Types.h>
#include <SparkFunLSM9DS1.h>
#include <LSM9DS1_Registers.h>

/* This is a trimmed down version of Sparkfun's code, found at
https://github.com/sparkfun/LSM9DS1_Breakout 
only meant to return Ax, Ay, Az (in g's) from seperate functions, 
making them easy to implement later in another file
*/

// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

//////////////////////////
// LSM9DS1 Library Init //
//////////////////////////
// Use the LSM9DS1 class to create an object. [imu] can be
// named anything, we'll refer to that throught the sketch.
LSM9DS1 imu;

///////////////////////
// Example I2C Setup //
///////////////////////
// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

////////////////////////////
// Sketch Output Settings //
////////////////////////////
#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250 // 250 ms between prints
static unsigned long lastPrint = 0; // Keep track of print time

// Earth's magnetic field varies by location. Add or subtract 
// a declination to get a more accurate heading. Calculate 
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.

void setup() 
{
  
  SerialUSB.begin(115200);
  
  // Before initializing the IMU, there are a few settings
  // we may need to adjust. Use the settings struct to set
  // the device's communication mode and addresses:
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  // The above lines will only take effect AFTER calling
  // imu.begin(), which verifies communication with the IMU
  // and turns it on.
  if (!imu.begin())
  {
    SerialUSB.println("Failed to communicate with LSM9DS1.");
    SerialUSB.println("Double-check wiring.");
    SerialUSB.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1)
      ;
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if ( imu.accelAvailable() )
  {
    // To read from the accelerometer, first call the
    // readAccel() function. When it exits, it'll update the
    // ax, ay, and az variables with the most current data.
    imu.readAccel();
    printAx();
    printAy();
    printAz();
    delay(100);
  }
  
}


float getAx()
{
  return imu.calcAccel(imu.ax);
}

void printAx()
{
  SerialUSB.print("Ax: ");
  SerialUSB.println(getAx(), 2);
}

float getAy()
{
  return imu.calcAccel(imu.ay);
}

void printAy()
{
  SerialUSB.print("Ay: ");
  SerialUSB.println(getAy(), 2);
}

float getAz()
{
  return imu.calcAccel(imu.az);
}

void printAz()
{
  SerialUSB.print("Az: ");
  SerialUSB.println(getAz(), 2);
}
