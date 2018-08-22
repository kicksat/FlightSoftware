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
#include <IMUHandler.h>
#include <KickSatLog.h>
#include <RTCCounter.h>
#include <uplink.h>
//#include <burn.h>
#include <KickSatConfig.h>
#include <BattHandler.h>
//#include <kickSatGPS.h>

/////////////////
// Definitions //
/////////////////
#define HOLDTIME 10 // Time to hold after initial deployment, in seconds
#define BEACONTIMER 10000 // Frequency of beacon, in milliseconds
#define ANTENNAWAITTIME 1000 // Frequency of save to config during antenna wait
#define BATTERYTHRESHOLD 2.055 // Battery must be above this threshold to exit standby mode
#define LISTENINGDURATION 5000 // Defined in milliseconds
#define ARMINGDURATION 15000 // Defined in milliseconds // 3 minutes

///////////////////////////////////
// Declaration of global objects //
///////////////////////////////////
//IMUHandle IMU; // create IMU object
Counter watchdogTimer; // creates timer object to trigger watchdog
Counter beaconTimer; // timer object to indicate when it is time to beacon
Counter listenTimer; // timer object for listen duration
Counter holdTimer; //timer object to indicate when to increment hold timer in config during hold mode
BattHandle power;
IMUHandle IMU;
//GPSHandle kickSatGPS;

/////////////////////////////////
// Initialize global variables //
/////////////////////////////////
char buf[MAXCHARS]; // Create global variable for buffer from SD read function, this can be piped into radio.send()
//bool armingMode;

////////////////////
// Declare Flags //
///////////////////
bool WDTFLAG = false; // Flag that allows toggling of the watchdog state
bool HOLDFLAG = false; //Flag that indicates it is time to increment the hold timer

///////////////////////
//Function Prototypes//
///////////////////////
void watchdog();
void holdModeCallback();
void deployAntennas();
void checkConfigStatus();
void listenForUplinkArmingMode(char buf[], int armingDuration);
void createRandomData();

///////////
// SETUP //
///////////
void setup() {
  // Define pin modes
  pinMode(LED_BUILTIN, OUTPUT); // Defines builtin LED pin mode to output
  pinMode(WDT_WDI, OUTPUT); // Set watchdog pin mode to output

  // Begin timers
  watchdogTimer.init(1000, watchdog); // timer delay, seconds
  timeout.init();

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

//  if(kickSatGPS.init()){ // Initialize IMU
//    SerialUSB.println("IMU Intialized");
//  } else {
//    SerialUSB.println("IMU Could Not Be Intialized");
//  }

  ///////////////
  // HOLD MODE //
  ///////////////
  // Goes into HOLD mode unpon initial deployment, flag is set to not enter this flag more than once
  if (!configFile.getHoldstatus()) { // If the satellite has just deployed and not been in HOLD mode yet (HOLD mode is the mandatory delay after deployment)
    SerialUSB.println("Entering HOLD mode");
    configFile.setHold();
    checkConfigStatus();
    holdTimer.init(ANTENNAWAITTIME,holdModeCallback); // timer delay, seconds
    //    timeout.start(HOLDTIME); // Sets up a backup timer to escape hold mode (while loop) (in case SD card is corrupted or something else unexpected)
    while(configFile.getHoldstatus()) { // Hold here until we hit our hold timeout
      if(HOLDFLAG){
        configFile.incrementAntennaTimer();
        HOLDFLAG = false;
        SerialUSB.print("incremented timer: ");
        SerialUSB.println(configFile.checkAntennaTimer());
      }
      if (configFile.checkAntennaTimer() >= HOLDTIME) {// timer value in config file >= number of hold timer intervals in holdtime
        /* deploys antennas after hold time */
        deployAntennas();
        //SerialUSB.println("Antennas deployed");
      }
      //      sleepTimer.sleep(); // Sleep
      //      if (timeout.triggered()) { // Checks time for timeout
      //        /* write hold status as completed (we only hold once) */
      //        break;
      //      }
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
  if (beaconTimer.check() ){ //&& batteryAboveThreshold()) {
    SerialUSB.println("****************************BEACON TIME***********************************");   

    /////////////////////////////////////
    // Read all health and sensor data //
    /////////////////////////////////////
    createRandomData(); // Temporary solution TODO: Complete data collection for all sensors
    power.read(data.powerData); // Read IMU data TODO: This function doesn't exist but should
    //kickSatGPS.read(data.gpsData); // Read IMU data TODO: This function doesn't exist but should
    IMU.read(data.imuData); // Read IMU data


    //////////////////////////////////
    // Save all data to the SD card //
    //////////////////////////////////
    if(logfile.available()) {
      SerialUSB.println("**********CREATING LOG************");
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
      uint8_t command = listenForUplink(buf, LISTENINGDURATION);
      if(command != UINT8_FALSE){
        SerialUSB.println("******************UPLINK RECIEVED*********************");
        processUplink(buf, command); // Process uplink
      }

    ///////////////////////////////////
    // Enter arming mode upon request//
    ///////////////////////////////////
    if (configFile.getArmedStatus()) { //if status byte is set to arming mode
      //timeout.reset();
      //timeout.init();
      SerialUSB.println("We entered the loop");
      uint8_t commandArm = listenForUplink(buf, ARMINGDURATION); // Listen for burn commands and stores flags for each burn wire request
        if(commandArm != UINT8_FALSE){
          SerialUSB.println("******************UPLINK RECIEVED*********************");
          processUplink(buf, commandArm);
        }
        if (configFile.getDB1FlagStatus()) { // Read config file for flag to burn sprite burn wire #1 // TODO: Decide whether or not to add a battery threshold check
          //burn.burnSpriteOne(); // Burn sprite burn wire #1
          configFile.setDB1Deployed();//record the burn
          checkConfigStatus();
        }
        if (configFile.getDB2FlagStatus()) { // Read config file for flag to burn sprite burn wire #2 // TODO: Decide whether or not to add a battery threshold check
          //burn.burnSpriteTwo(); // Burn sprite burn wire #2
          configFile.setDB2Deployed();//record the burn
          checkConfigStatus();
        }
        if (configFile.getDB3FlagStatus()) { // Read config file for flag to burn sprite burn wire #3 // TODO: Decide whether or not to add a battery threshold check
          //burn.burnSpriteThree(); // Burn sprite burn wire #3
          configFile.setDB3Deployed();//record the burn
          checkConfigStatus();
       }
       configFile.setStandby();

    }

  //////////////////////
  // Enter sleep mode //
  //////////////////////
  // sleepTimer.sleep(); // Go into sleep mode until next interrupt

 }
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
 HOLDFLAG = true; // Sets flag to tell main code to increment time in config file on SD card
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

void checkConfigStatus(){
  SerialUSB.println();
  SerialUSB.print("Status: ");
  SerialUSB.println((char)configFile.getStatus());
  SerialUSB.print("AB1: ");
  SerialUSB.print(configFile.getAB1status());
  SerialUSB.print(", AB2: ");
  SerialUSB.println(configFile.getAB2status());
  SerialUSB.print("DB1: ");
  SerialUSB.print(configFile.getDB1status());
  SerialUSB.print(", DB2: ");
  SerialUSB.print(configFile.getDB2status());
  SerialUSB.print(", DB3: ");
  SerialUSB.println(configFile.getDB3status());
  SerialUSB.print("FB1: ");
  SerialUSB.print(configFile.getDB1FlagStatus());
  SerialUSB.print(", FB2: ");
  SerialUSB.print(configFile.getDB2FlagStatus());
  SerialUSB.print(", FB3: ");
  SerialUSB.println(configFile.getDB3FlagStatus());
//  SerialUSB.print("Hold mode: ");
//  SerialUSB.print(configFile.getHoldstatus());
//  SerialUSB.print(", Standby mode: ");
//  SerialUSB.print(configFile.getStandbyStatus());
//  SerialUSB.print(", Armed mode: ");
//  SerialUSB.print(configFile.getArmedStatus());
//  SerialUSB.print(", Deployed mode: ");
//  SerialUSB.println(configFile.getDeployedStatus());
  SerialUSB.print("Antenna Timer: ");
  SerialUSB.println(configFile.checkAntennaTimer());
  SerialUSB.println();
  delay(500);
}


