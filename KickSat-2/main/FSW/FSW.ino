///////////////////////////////////////////////////////////////////
//////////////////// KICKSAT-2 FLIGHT SOFTWARE ////////////////////
///////////////////////////////////////////////////////////////////
/*
Last update on: 8-11-18
by: Ralen
*/

//////////////
// Includes //
///////////////////////////////////////////////////////////////////
#include <IMUHandler.h>
#include "sd_log.h"
#include <RTCCounter.h>
#include <uplink.h>

/////////////////
// Definitions //
///////////////////////////////////////////////////////////////////

///////////////////////////////////
// Declaration of global objects //
///////////////////////////////////////////////////////////////////
sd_log SDLogger; // creates sd log object
IMUHandle IMU; // create IMU object
Counter watchdogTimer; // creates timer object
Counter beaconTimer; // creates timer object
Counter listenTimer; // creates timer object

/////////////////////////////////
// Initialize global variables //
///////////////////////////////////////////////////////////////////
int current_log = 0;
int iteration = 0;
int n = 5;
float threshold = 2.5;
float v = 0.000;

///////////////////////
// Declare functions //
///////////////////////////////////////////////////////////////////
void watchdog(); // Function that runs every time watchdog timer triggers
void readHealth(); // Function to read health data
float imuData[9];
bool LEDSTATE = false;

// SETUP //
///////////////////////////////////////////////////////////////////
void setup() {
  SerialUSB.begin(115200); // Restart SerialUSB
  while(!SerialUSB); // Wait for serial USB port to open
  SerialUSB.println("Serial Initialized");
  delay(5000); // Provides user with time to open Serial Monitor
  pinMode(LED_BUILTIN, OUTPUT);
  ///////////////////////////////////////////////////////////////////

  if(!IMU.begin()){ // Initialize imu)
    SerialUSB.println("IMU Could Not Be Intialized");
  } else {
    SerialUSB.println("IMU Intialized");
  }
  SDLogger.sd_init(); // Init SD card

  //////////////////
  // Init objects //
  ///////////////////////////////////////////////////////////////////
  watchdogTimer.init(1,watchdog); // timer delay, seconds
  beaconTimer.init(30); // timer delay, seconds
  ///////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////

//////////
// LOOP //
///////////////////////////////////////////////////////////////////
void loop() {

  // if (beaconTimer.check()) { readHealth(); }  // Read Health data

  if (beaconTimer.check()) {
    IMU.read(imuData); // Read IMU data
    // delay(1000);
    // SerialUSB.println();
    for(int i = 0; i < 9; i++) { // Print to Serial IMU data
      SerialUSB.print(i);SerialUSB.print(": ");SerialUSB.println(imuData[i]);
    }
    SerialUSB.println();

    float batt_level = read_battery();

    // readHealth(SDLogger.logData); // TODO: Fix


    SDLogger.sd_refresh();
    SDLogger.logData.log_num = current_log;
    SDLogger.write_log(SDLogger.logData);
    // SerialUSB.print("Wrote Log ");
    // SerialUSB.println(SDLogger.logData.log_num);
    // SerialUSB.println(SDLogger.read_entry(current_log));
    // delay(1000);
    SDLogger.sd_end();
    current_log++;

    // TODO: read the most recent header from the SD card @emma

    // String header_chirp = "I'm doing fine. Thanks for checking, earthling";
    // TODO: @emma
    // serial_transmit(header_chirp);
    // END of SD Stuff

    output_menu();
    uint8_t command = listenForUplink();
    processUplink(command);

  }  // Read IMU data // TODO: Temporary, replace with readHealth


  // sleepTimer.sleep(); // Go into sleep mode until next interrupt


}

// handles command one to send down the sensor data
void send_sensor_data()
{
  while(1)
  {
    float command1_threshold = 2.055; // TODO: measure the actual battery level needed to complete the sensor downlink
    v = read_battery(); // TODO: make this function correct... it does not work as is (board A)
    SerialUSB.print("CMD 1 Threshold: ");
    SerialUSB.println(command1_threshold);
    SerialUSB.print("Battery Voltage: ");
    SerialUSB.println(v);
    if(v < command1_threshold)
    {
      SerialUSB.println("Going to sleep...");
      // TODO: go to sleep for 30 seconds
      SerialUSB.println("Awake!");
    }
    else
    {
      SerialUSB.println("Battery is charged enough to send sensor info");
      break;
    }
  }

  // out of the while(1) loop, send the sensor data

  // TODO: Read sensor data from SD card @ Emma
  // TODO: @max @connor --> do we want to send another command for which sensor to send data on at the top of this sensor
  // this way the function would begin with listening for which sensor number to send data about

  // Send down the sensor data via Serial (radio)
  String sensor_data_string = "Rad: 759 kRad";
  // translate to ax25 packet;
  serial_transmit(sensor_data_string);

}

void serial_transmit(String to_send)
{
  SerialUSB.print("Send via Radio: '");
  SerialUSB.print(to_send);
  SerialUSB.println("'");
}

/*
* Read battery function for reading the battery voltage!
*/
float read_battery() { // TODO: this needs to go into a battery library
  //  int batt_val = analogRead(VBATT);
  //  float batt_voltage = batt_val * (3.3 / 1023.0) * (426.0/110.0);
  //  SerialUSB.println("VBATT: ");
  //  SerialUSB.println(batt_voltage);
  return 5;
}

void watchdog() { // Function that runs every time watchdog timer triggers
  // SerialUSB.println("Watchdog");
  if (LEDSTATE) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  LEDSTATE = !LEDSTATE;
}

// void readHealth() {
//   IMU.read(SDLogger.logData.IMU_data); // Read IMU data // TODO: Fix input, currently a valid input but it should be
// }
