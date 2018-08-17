/////////////////////////////////////////////////////////////////
////////////////// KICKSAT-2 FLIGHT SOFTWARE ////////////////////
/////////////////////////////////////////////////////////////////

/*
Last update on: 8-12-18
by: Ralen
*/

//////////////
// Includes //
//////////////
#include "IMUHandler.h"
#include <KickSatLog.h>
#include <RTCCounter.h>
#include <uplink.h>
#include <burn.h>
#include <KickSatConfig.h>
#include <BattHandler.h>
#include <kickSatGPS.h>

/////////////////
// Definitions //
/////////////////
#define HOLDTIME 10000 // Time to hold after initial deployment, in milliseconds
#define BEACONTIMER 10000 // Frequency of beacon, in milliseconds
#define ANTENNAWAITTIME 15000 // Frequency of beacon, in milliseconds
#define BATTERYTHRESHOLD 2.055 // Battery must be above this threshold to exit standby mode
#define LISTENINGDURATION 5000 // Defined in milliseconds
#define ARMINGDURATION 5000 // Defined in milliseconds // 3 minutes

///////////////////////////////////
// Declaration of global objects //
///////////////////////////////////
//IMUHandle IMU; // create IMU object
Counter watchdogTimer; // creates timer object
Counter beaconTimer; // creates timer object
Counter listenTimer; // creates timer object
BattHandle power;
GPSHandle kickSatGPS;

/////////////////////////////////
// Initialize global variables //
/////////////////////////////////
char buf[MAXCHARS]; // Create global variable for buffer from SD read function, this can be piped into radio.send()
//bool armingMode;

///////////////////////
// Declare functions //
///////////////////////
bool WDTFLAG = false; // Flag that allows toggling of the watchdog state

///////////
// SETUP //
///////////
void setup() {
  // Define pin modes
  pinMode(LED_BUILTIN, OUTPUT); // Defines builtin LED pin mode to output
  pinMode(WDT_WDI, OUTPUT); // Set watchdog pin mode to output

  // Begin timers
  watchdogTimer.init(1000,watchdog); // timer delay, seconds

  // Initialize Serial
  SerialUSB.begin(115200); // Restart SerialUSB
  while(!SerialUSB); // Wait for SerialUSB USB port to open
  SerialUSB.println("SerialUSB Initialized");
  delay(5000); // Provides user with time to open SerialUSB Monitor or upload before sleep

  // Init objects
  if(configFile.init()) { // Initialize SD card and config file
    SerialUSB.println("Config File Initialized");
  } else {
    SerialUSB.println("Config File Not Initialized");
  }

  if(logfile.init()) { // Initialize log file
    SerialUSB.println("Log Card Initialized");
  } else {
    SerialUSB.println("Log Card Not Initialized");
  }

  if(IMU.begin()){ // Initialize IMU
    SerialUSB.println("IMU Intialized");
  } else {
    SerialUSB.println("IMU Could Not Be Intialized");
  }

  if(kickSatGPS.init()){ // Initialize IMU
    SerialUSB.println("IMU Intialized");
  } else {
    SerialUSB.println("IMU Could Not Be Intialized");
  }

  ///////////////
  // HOLD MODE //
  ///////////////
  // Goes into HOLD mode unpon initial deployment, flag is set to not enter this flag more than once
  if (!configFile.getHoldstatus()) { // If the satellite has just deployed and not been in HOLD mode yet (HOLD mode is the mandatory delay after deployment)
    SerialUSB.println("Entering HOLD mode");
    configFile.setHold();
    holdTimer.init(HOLDTIME,holdModeCallback); // timer delay, seconds
    //    timeout.start(HOLDTIME); // Sets up a backup timer to escape hold mode (while loop) (in case SD card is corrupted or something else unexpected)
    while(configFile.getHoldstatus()) { // Hold here until we hit our hold timeout
      if(incrementTimerFlag){
        configFile.incrementAntennaTimer();
        incrementTimerFlag = false;
        SerialUSB.println("incremented timer");
      }
      if (configFile.checkAntennaTimer() >= HOLDTIME) {// timer value in config file >= number of hold timer intervals in holdtime
        /* deploys antennas after hold time */
        deployAntennas();
        SerialUSB.println("Antennas deployed");
      }
      //      sleepTimer.sleep(); // Sleep
      //      if (timeout.triggered()) { // Checks time for timeout
      //        /* write hold status as completed (we only hold once) */
      //        break;
      //      }
    }
  }
}



// Begin beacon timer
beaconTimer.init(BEACONTIMER); // timer delay, seconds

}
///////////////////////////////////////////////////////////////////

//////////
// LOOP //
//////////
void loop() {

  // Check if the beaconTimer has triggered and the battery is above the threshold voltage
  if (beaconTimer.check() && batteryAboveThreshold) {

    /////////////////////////////////////
    // Read all health and sensor data //
    /////////////////////////////////////
    createRandomData(); // Temporary solution TODO: Complete data collection for all sensors
    power.read(data.powerData); // Read IMU data TODO: This function doesn't exist but should
    kickSatGPS.read(data.gpsData); // Read IMU data TODO: This function doesn't exist but should
    //IMU.read(data.imuData); // Read IMU data


    //////////////////////////////////
    // Save all data to the SD card //
    //////////////////////////////////
    if(logfile.available()) {
      logfile.appendData();
    }
    // TODO: add append/save sensor data

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
    if (listenForUplink(buf, LISTENINGDURATION)) {
      processUplink(); // Process uplink
    }

    ///////////////////////////////////
    // Enter arming mode upon reques //
    ///////////////////////////////////
    if (getArmedStatus()) {
      listenForUplinkArmingMode(buf, ARMINGDURATION)); // Listen for burn commands and stores flags for each burn wire request
      if (getDB1status()) { // Read config file for flag to burn sprite burn wire #1 // TODO: Decide whether or not to add a battery threshold check
        burn.burnSpriteOne(); // Burn sprite burn wire #1
      }
      if (getDB2status()) { // Read config file for flag to burn sprite burn wire #2 // TODO: Decide whether or not to add a battery threshold check
        burn.burnSpriteTwo(); // Burn sprite burn wire #2
      }
      if (getDB3status()) { // Read config file for flag to burn sprite burn wire #3 // TODO: Decide whether or not to add a battery threshold check
        burn.burnSpriteThree(); // Burn sprite burn wire #3
      }
      setStandby(); // Return to stanby mode
    }
  }

  //////////////////////
  // Enter sleep mode //
  //////////////////////
  // sleepTimer.sleep(); // Go into sleep mode until next interrupt

}

void createRandomData() { // Temporary until we are     createRandomData(); // Temporary solution TODO: Complete data collection for all sensorsreading from each sensor
  data.status = random(0,10);
  for(uint8_t i = 0; i < 3; i++){
    data.powerData[i] = random(0,100)/13.87;
  }
  for(uint8_t i = 0; i < 2; i++){
    data.dateTime[i] = random(100000,999999);
  }
  for(uint8_t i = 0; i < 3; i++){
    data.gpsData[i] = random(0,200)/13.87;
  }
  for(uint8_t i = 0; i < 9; i++){
    data.imuData[i] = random(0,100)/9.123;
  }
}


bool batteryAboveThreshold() {
  return power.readBattVoltage() > BATTERYTHRESHOLD; // Read battery voltage and compare it to threshold
}


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

void holdModeCallback() {
 incrementTimerFlag = true; // Sets flag to tell main code to increment time in config file on SD card
}

/////////////////////
// DEPLOY ANTENNAS //
/////////////////////
// Deploys antenna and updates antenna flags
void deployAntennas(){
  SerialUSB.println("Begining Antenna Deployment Procedure");
  timeout.start(ANTENNAWAITTIME); // Starts a timeout timer
  while(!configFile.getAB1status() || !configFile.getAB2status()) { // While either antenna burn wire is not burned
    if(!configFile.getAB1status() && batteryAboveThreshold()) { // Burn the first antenna wire if it hasn't already been done and the battery threshold is met
    //burn.burnAntennaOne();
    configFile.setAB1Deployed();
    SerialUSB.println("Antenna One Burned");
    checkConfigStatus();
  }
  if(!configFile.getAB2status() && batteryAboveThreshold()) { // Burn the second antenna wire if it hasn't already been done and the battery threshold is met
  //burn.burnAntennaTwo();
  configFile.setAB2Deployed();
  SerialUSB.println("Antenna Two Burned");
  checkConfigStatus();
}
if (timeout.triggered()) { // Checks time for timeout
  break;
}
}
configFile.setStandby(); //updates mode to standby
checkConfigStatus();
}
