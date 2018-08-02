/*
 * Standby Mode
 *
 */

// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <kicksat.h>

////////////////////////////
// Sketch Output Settings //
////////////////////////////
#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250 // 250 ms between prints

/////////////////////////////////
// GLOBAL VARIABLE DEFINITIONS //
/////////////////////////////////
int sensor_number;
float sensor_data;
int next_beacon; // this is an instruction from Ground Station for what to send on next downlink
float battery_level;
int log_number;
beacon myBeacon = beacon(); // create the beacon object
LSM9DS1 imu; // IMU object
/////////////////////////////////////
// RND GLOBAL VARIABLE DEFINITIONS //
/////////////////////////////////////


void setup()
{
  ///////////////////////////
  // Serial Initialization //
  ///////////////////////////
  SerialUSB.begin(115200);
  while(!SerialUSB) {} // wait for SerialUSB connectiong to initialize
  delay(5000); // provides time to open serial monitor and prevents serial port overload
  SerialUSB.println("Serial Initialized");
  ///////////////////////////////
  // End Serial Initialization //
  ///////////////////////////////

  ////////////////////////
  // IMU Initialization //
  ////////////////////////
  init_imu();
  ////////////////////////////
  // End IMU Initialization //
  ////////////////////////////

}

void loop()
{

  SerialUSB.print("Gyro:");
  SerialUSB.print(return_gx());
  SerialUSB.print(",");
  SerialUSB.print(return_gy());
  SerialUSB.print(",");
  SerialUSB.print(return_gz());
  SerialUSB.println();
  SerialUSB.print("Accel:");
  SerialUSB.print(return_ax());
  SerialUSB.print(",");
  SerialUSB.print(return_ay());
  SerialUSB.print(",");
  SerialUSB.print(return_az());
  SerialUSB.println();

  delay(4000);
  SerialUSB.println();

}
