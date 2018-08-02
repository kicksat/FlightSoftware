
#include <SparkFunLSM9DS1.h>

/* ***********************************************
*  IMU SETUP
* ***********************************************
*/

//////////////////////////
// LSM9DS1 Library Init //
//////////////////////////
// Use the LSM9DS1 class to create an object. [imu] can be
// named anything, we'll refer to that throught the sketch.


// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG 0x6B // Would be 0x6A if SDO_AG is LOW

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

///////////////////
// IMU FUNCTIONS //
///////////////////

float return_gx(LSM9DS1 imu) // Calculates gyroscope, x-component
float return_gy(LSM9DS1 imu) // Calculates gyroscope, y-component
float return_gz(LSM9DS1 imu) // Calculates gyroscope, z-component
float return_ax(LSM9DS1 imu) // Calculates accelerometer, x-component
float return_ay(LSM9DS1 imu) // Calculates accelerometer, y-component
float return_az(LSM9DS1 imu) // Calculates accelerometer, z-component
float return_mx(LSM9DS1 imu) // Calculates magnetometer, x-component
float return_my(LSM9DS1 imu) // Calculates magnetometer, y-component
float return_mz(LSM9DS1 imu) // Calculates magnetometer, z-component
float return_roll() // Calculates roll
float return_pitch() // Calculates pitch
float return_yaw() // Calculates yaw
void init_imu(LSM9DS1 imu) // Initialize the IMU

///////////////////////
// END IMU FUNCTIONS //
///////////////////////
