///////////////////////////////////////////////////////////////////
//////////////////// KICKSAT-2 FLIGHT SOFTWARE ////////////////////
///////////////////////////////////////////////////////////////////
/*
Last update on: 8-12-18
by: Ralen
*/

//////////////
// Includes //
///////////////////////////////////////////////////////////////////
#include <IMUHandler.h>
#include <KickSatLog.h>
#include <RTCCounter.h>
#include <uplink.h>

/////////////////
// Definitions //
///////////////////////////////////////////////////////////////////
#define BATTERYTHRESHOLD 2.055 // Battery must be above this threshold to exit standby mode

///////////////////////////////////
// Declaration of global objects //
///////////////////////////////////////////////////////////////////
IMUHandle IMU; // create IMU object
Counter watchdogTimer; // creates timer object
Counter beaconTimer; // creates timer object
Counter listenTimer; // creates timer object

/////////////////////////////////
// Initialize global variables //
///////////////////////////////////////////////////////////////////
char buf[MAXCHARS]; // Create global variable for buffer from SD read function, this can be piped into radio.send()

///////////////////////
// Declare functions //
///////////////////////////////////////////////////////////////////
void watchdog(); // Function that runs every time watchdog timer triggers
bool WDTFLAG = false; // Flag that allows toggling of the watchdog state

// SETUP //
///////////////////////////////////////////////////////////////////
void setup() {
  SerialUSB.begin(115200); // Restart SerialUSB
  while(!SerialUSB); // Wait for serial USB port to open
  SerialUSB.println("Serial Initialized");
  delay(5000); // Provides user with time to open Serial Monitor
  pinMode(LED_BUILTIN, OUTPUT); // Defines builtin LED pin mode to output
  pinMode(WDT_WDI, OUTPUT); // Set watchdog pin mode to output
  ///////////////////////////////////////////////////////////////////

  if(IMU.begin()){ // Initialize IMU
    SerialUSB.println("IMU Intialized");
  } else {
    SerialUSB.println("IMU Could Not Be Intialized");
  }
  if(logfile.init()) { // Initialize SD card
    SerialUSB.println("SD Card Initialized");
  } else {
    SerialUSB.println("SD Card Not Initialized");
  }

  //////////////////
  // Init objects //
  ///////////////////////////////////////////////////////////////////
  watchdogTimer.init(1,watchdog); // timer delay, seconds
  beaconTimer.init(10); // timer delay, seconds
  ///////////////////////////////////////////////////////////////////
}
///////////////////////////////////////////////////////////////////

//////////
// LOOP //
///////////////////////////////////////////////////////////////////
void loop() {

  // Check if the beaconTimer has triggered and the battery is above the threshold voltage
  if (beaconTimer.check() && batteryAboveThreshold) {

    /////////////////////////////////////
    // Read all health and sensor data //
    /////////////////////////////////////
    createRandomData(); // Temporary solution TODO: Complete data collection for all sensors
    // power.read(data.powerData); // Read IMU data TODO: This function doesn't exist but should
    // GPS.read(data.gpsData); // Read IMU data TODO: This function doesn't exist but should
    IMU.read(data.imuData); // Read IMU data


    //////////////////////////////////
    // Save all data to the SD card //
    //////////////////////////////////
    if(logfile.available()) {
      logfile.appendData();
    }

    //////////////////////////////////////////
    // Format health data and send to radio //
    //////////////////////////////////////////
    if(logfile.available()) {
      logfile.compileHealth(buf);
    }
    // radio.send(ax25(buf)); // Send health data through radio // TODO: This function doesn't exist yet but should

    //////////////////////////
    // Enter listening mode //
    //////////////////////////
    if (listenForUplink(buf)) {
      // processUplink();
    }

  }

  //////////////////////
  // Enter sleep mode //
  //////////////////////
  // sleepTimer.sleep(); // Go into sleep mode until next interrupt

}


void createRandomData() { // Temporary until we are reading from each sensor
  data.status = random(0,10);
  for(uint8_t i = 0; i < 3; i++){
    data.powerData[i] = random(0,100);
  }
  for(uint8_t i = 0; i < 4; i++){
    data.gpsData[i] = random(0,200)/13.87;
  }
  for(uint8_t i = 0; i < 9; i++){
    data.imuData[i] = random(0,100)/9.123;
  }
  for(uint8_t i = 0; i < 8; i++){
    data.commandData[i] = random(0,9);
  }
}


bool batteryAboveThreshold() {
  // return readBattery() > BATTERYTHRESHOLD // TODO: Read battery doesn't exist but should
  return 3 > BATTERYTHRESHOLD;
}

// // handles command one to send down the sensor data
// void send_sensor_data()
// {
//   while(1)
//   {
//     float command1_threshold = 2.055; // TODO: measure the actual battery level needed to complete the sensor downlink
//     v = read_battery(); // TODO: make this function correct... it does not work as is (board A)
//     SerialUSB.print("CMD 1 Threshold: ");
//     SerialUSB.println(command1_threshold);
//     SerialUSB.print("Battery Voltage: ");
//     SerialUSB.println(v);
//     if(v < command1_threshold)
//     {
//       SerialUSB.println("Going to sleep...");
//       // TODO: go to sleep for 30 seconds
//       SerialUSB.println("Awake!");
//     }
//     else
//     {
//       SerialUSB.println("Battery is charged enough to send sensor info");
//       break;
//     }
//   }
//
//   // out of the while(1) loop, send the sensor data
//
//   // TODO: Read sensor data from SD card @ Emma
//   // TODO: @max @connor --> do we want to send another command for which sensor to send data on at the top of this sensor
//   // this way the function would begin with listening for which sensor number to send data about
//
//   // Send down the sensor data via Serial (radio)
//   String sensor_data_string = "Rad: 759 kRad";
//   // translate to ax25 packet;
//   serial_transmit(sensor_data_string);
//
// }

void watchdog() { // Function that runs every time watchdog timer triggers
  if (WDTFLAG) {
    digitalWrite(LED_BUILTIN, HIGH); // Toggles builtin LED
    digitalWrite(WDT_WDI, HIGH); // Toggles watchdog timer
  } else {
    digitalWrite(LED_BUILTIN, LOW); // Toggles builtin LED
    digitalWrite(WDT_WDI, LOW); // Toggles watchdog timer
  }
  WDTFLAG = !WDTFLAG; // Toggles watchdog flag
}
