/*
 * Standby Mode
 *
 */

// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <kicksat.h>

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
  if (!imu.begin())
  {
    SerialUSB.println("Failed to communicate with LSM9DS1.");
    SerialUSB.println("Double-check wiring.");
    SerialUSB.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
  }
  ////////////////////////////
  // End IMU Initialization //
  ////////////////////////////

}

void loop()
{

  SerialUSB.print("Gyro:");
  SerialUSB.print(imu.gx);
  SerialUSB.print(",");
  SerialUSB.print(imu.gy);
  SerialUSB.print(",");
  SerialUSB.print(imu.gz);
  SerialUSB.println();
  SerialUSB.print("Accel:");
  SerialUSB.print(imu.ax);
  SerialUSB.print(",");
  SerialUSB.print(imu.ay);
  SerialUSB.print(",");
  SerialUSB.print(imu.az);
  SerialUSB.println();

  delay(4000);
  SerialUSB.println();

}
