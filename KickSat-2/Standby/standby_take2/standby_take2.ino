/*
 * Standby Mode Take II
 * 
 */

 /* ***********************************************
  *  IMU SETUP
  * ***********************************************
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
// TODO @ZAC is this fine... does this matter? do we want to dynamically update based on interpolated lat/long values

  /* ***********************************************
  *  END IMU SETUP
  * ***********************************************
  */


void setup() 
{
  
  SerialUSB.begin(115200);

  delay(4000);

  SerialUSB.println("Alive");

  init_imu(); 
}

void loop()
{
  SerialUSB.print("Gx: ");
  SerialUSB.println(return_gx());

  SerialUSB.print("Ax: ");
  SerialUSB.println(return_ax());
  SerialUSB.print("Ay: ");
  SerialUSB.println(return_ay());
  SerialUSB.print("Az: ");
  SerialUSB.println(return_az());

  

  
  
  delay(4000);
}

////////////////////////////////////////////////
/* reading and returning IMU values */
////////////////////////////////////////////////
float return_gx()
{
  imu.readGyro();
  return imu.calcGyro(imu.gx);
}

float return_gy()
{
  imu.readGyro();
  return imu.calcGyro(imu.gy);
}

float return_gz()
{
  imu.readGyro();
  return imu.calcGyro(imu.gz);
}

float return_ax()
{
  imu.readAccel();
  return imu.calcAccel(imu.ax);
}

float return_ay()
{
  imu.readAccel();
  return imu.calcAccel(imu.ay);
}

float return_az()
{
  imu.readAccel();
  return imu.calcAccel(imu.az);
}

float return_mx()
{
  imu.readMag();
  return imu.calcMag(imu.mx);
}

float return_my()
{
  imu.readMag();
  return imu.calcMag(imu.my);
}

float return_mz()
{
  imu.readMag();
  return imu.calcMag(imu.mz);
}

float return_roll()
{
  float roll = atan2(return_ay(), return_az());
  roll  *= 180.0 / PI;  
  return roll;
}

float return_pitch()
{
  float pitch = atan2(-return_ax(), sqrt(return_ay() * return_ay() + return_az() * return_az()));
  pitch *= 180.0 / PI;
  return pitch;
}

float return_yaw()
{
  float ax1 = return_ax();
  float ay1 = return_ay();
  float az1 = return_az();
  float mx1 = return_mx();
  float my1 = return_my();
  float mz1 = return_mz();
  
  float roll = atan2(ay1, az1);
  float pitch = atan2(-ax1, sqrt(ay1 * ay1 + az1 * az1));
  
  float yaw;
  if (my1 == 0)
    yaw = (mx1 < 0) ? 180.0 : 0;
  else
    yaw = atan2(mx1, my1);
    
  yaw -= DECLINATION * PI / 180;
  
  if (yaw > PI) yaw -= (2 * PI);
  else if (yaw < -PI) yaw += (2 * PI);
  else if (yaw < 0) yaw += 2 * PI;
  
  // Convert everything from radians to degrees:
  yaw *= 180.0 / PI;

  return yaw;
}

/*
 * END OF IMU FUNCTIONS
 */

void init_imu()
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
  if (!imu.begin())
  {
    SerialUSB.println("Failed to communicate with LSM9DS1.");
    SerialUSB.println("Double-check wiring.");
    SerialUSB.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1)
    {
      // stay here
      SerialUSB.println("Imu did not init");
      delay(5000);
    }
  }
}

