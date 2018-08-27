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
#include <FSWvariables.h>
#include <IMUHandler.h>
#include <KickSatLog.h>
#include <RTCCounter.h>
#include <uplink.h>
#include <burn.h>
#include <KickSatConfig.h>
#include <BattHandler.h>
//#include <kickSatGPS.h>
#include <SD_DataFile.h>
#include <beacon.h>
//#include <KickSat_Sensor.h>

/////////////////
// Definitions //
/////////////////
#define HOLDTIME 10000 // Time to hold after initial deployment, in milliseconds // TODO: change to X milliseconds (X minute) for flight
#define HOLDTIMESTEP 1000 // Time step for recording time, during hold mode, in config file
#define BEACONTIMER 10000 // Frequency of beacon, in milliseconds // TODO: change to 60000 milliseconds (1 minute) for flight
#define ANTENNAWAITTIME 10000 // Time to wait while antenna is being deployed, milliseconds
#define BATTERYTHRESHOLD 2.055 // Battery must be above this threshold to exit standby mode
#define LISTENINGDURATION 5000 // Defined in milliseconds // TODO: change to 30000 milliseconds (30 seconds) for flight
#define ARMINGDURATION 15000 // Defined in milliseconds // TODO: change to 180000 milliseconds (3 minutes) for flight
#define SENSOR_DATA_WIDTH 3

///////////////////////////////////
// Declaration of global objects //
///////////////////////////////////
SdFat SD;

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
int sensorNum = 0;
extern SD_DataFile sensorLog1(SPI_CS_SD, SENSOR_DATA_WIDTH, "sl1.txt", SD);
extern SD_DataFile sensorLog2(SPI_CS_SD, SENSOR_DATA_WIDTH, "sl2.txt", SD);
extern SD_DataFile sensorLog3(SPI_CS_SD, SENSOR_DATA_WIDTH, "sl3.txt", SD);
extern SD_DataFile sensorLog4(SPI_CS_SD, SENSOR_DATA_WIDTH, "sl4.txt", SD);


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

  // Initialize Serial
  SerialUSB.begin(115200); // Restart SerialUSB
  while(!SerialUSB); // Wait for SerialUSB USB port to open
  SerialUSB.println("SerialUSB Initialized");
  delay(5000); // Provides user with time to open SerialUSB Monitor or upload before sleep

  SD.begin(SPI_CS_SD);

  // Init objects
  if(logfile.init()) { // Initialize log file
    SerialUSB.println("Log Card Initialized");
  } else {
    SerialUSB.println("Log Card Not Initialized");
  }

  if(configFile.init()) { // Initialize SD card and config file
    SerialUSB.println("Config File Initialized");
  } else {
    SerialUSB.println("Config File Not Initialized");
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
  // If SD cannot be initialized, hold mode is skipped, otherwise we wait for HOLDTIME
  if (!configFile.getHoldstatus()) { // If the satellite has just deployed and not been in HOLD mode yet (HOLD mode is the mandatory delay after deployment)
    SerialUSB.println("Entering HOLD mode");
    configFile.setHold(); // Set currrent status as being in Hold Mode
    checkConfigStatus(); // Prints out current status of all flags
    holdTimer.init(HOLDTIMESTEP,holdModeCallback); // timer delay, milliseconds
    timeout.start(HOLDTIME); // Sets up a backup timer to escape hold mode (while loop) (in case SD card is corrupted or something else unexpected)
    while(configFile.getHoldstatus()) { // Hold here until we hit our hold timeout
      if(HOLDFLAG) { // If we're in hold mode
        configFile.incrementAntennaTimer(); // increment hold timer in the config file
        HOLDFLAG = false; // Reset hold flag
        SerialUSB.print("Incremented timer: "); SerialUSB.println(configFile.checkAntennaTimer()); // Print out recorded time
      }
      if (timeout.triggered() || configFile.checkAntennaTimer() >= HOLDTIME/HOLDTIMESTEP) { // If the timeout is triggered or the timer value in the config file hits the desired value
        configFile.setStandby(); // Switches to standby mode
        checkConfigStatus(); // Print out config status
        break; // Break out of hold mode while loop
      }
    }
  }


  ////////////////////////
  // Antenna Deployment //
  ////////////////////////
  // If SD is working, reads from it for antenna status, otherwise default values are false
  if (!configFile.getAB1status() || !configFile.getAB2status()) {
    SerialUSB.println("Deploying Antennas");
    deployAntennas(); // Deploy antennas
    SerialUSB.println("Antennas deployed");
  }


  // TODO: Sleep mode commented out only while testing all other software functionality
  //      sleepTimer.sleep(); // Sleep while waiting to exit hold mode


  // Begin beacon timer
  beaconTimer.init(BEACONTIMER); // timer delay, millisecs

}
///////////////
// END SETUP //
///////////////


//////////
// LOOP //
//////////
void loop() {
  // Check if the beaconTimer has triggered and the battery is above the threshold voltage
  if (beaconTimer.check()) { //&& batteryAboveThreshold()) { // TODO: Do we want to check battery threshold here?
    SerialUSB.println("****************************BEACON TIME***********************************");

    /////////////////////////////////////
    // Read all health and sensor data //
    /////////////////////////////////////
    //    byte sensorData[30];
    //    int numDataPoints = kSensor1.operate(sensorData);
    //    for (int i = 0; i < numDataPoints; i++) {
    //      sensorLog1.writeDataEntry(&(sensorData[i*3]));
    //    }
    //
    //    numDataPoints = kSensor2.operate(sensorData);
    //    for (int i = 0; i < numDataPoints; i++) {
    //      sensorLog2.writeDataEntry(&(sensorData[i*3]));
    //    }

    //    numDataPoints = kSensor3.operate(sensorData);
    //    for (int i = 0; i < numDataPoints; i++) {
    //      sensorLog3.writeDataEntry(&(sensorData[i*3]));
    //    }
    //
    //    numDataPoints = kSensor4.operate(sensorData);
    //    for (int i = 0; i < numDataPoints; i++) {
    //      sensorLog4.writeDataEntry(&(sensorData[i*3]));
    //    }


    createRandomData(); // Temporary solution TODO: Complete data collection for all sensors
    power.read(data.powerData); // Read power data
    //kickSatGPS.read(data.gpsData); // Read IMU data TODO: Is this function working?
    IMU.read(data.imuData); // Read IMU data


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
    int len = 0;
    if (sensorNum == 1) {
      len = beacon(buf, sensorLog1);
    } else if (sensorNum == 2) {
      len = beacon(buf, sensorLog2);
    } else if (sensorNum == 3) {
      len = beacon(buf, sensorLog3);
    } else if (sensorNum == 4) {
      len = beacon(buf, sensorLog4);
    }
   sensorNum++;
   if(sensorNum > 3){  //cycles through every sensor
     sensorNum = 0;
   }
   SerialUSB.print("Beacon: ");
   for(int i = 0; i < len; i++){
    SerialUSB.print(buf[i]);
   }
   SerialUSB.println();
//   int len = result.length(); TODO: can use these 2 lines of code to convert string to char array, needed to use radio.send(const uint8_t* data, uint8_t len)
//   result.toCharArray(output, len);
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
      SerialUSB.println("Armed Mode Entered");
      uint8_t commandArm = listenForUplinkArmingMode(buf, ARMINGDURATION); // Listen for burn commands and stores flags for each burn wire request
      if(commandArm != UINT8_FALSE) { // If a burn command was received
        if (configFile.getDB1FlagStatus()) { // Read config file for flag to burn sprite burn wire #1 // TODO: Decide whether or not to add a battery threshold check
          burn.burnSpriteOne(); // Burn sprite burn wire #1
          configFile.setDB1Deployed(); // Record the burn
          checkConfigStatus(); // Print out config status
        }
        if (configFile.getDB2FlagStatus()) { // Read config file for flag to burn sprite burn wire #2 // TODO: Decide whether or not to add a battery threshold check
          burn.burnSpriteTwo(); // Burn sprite burn wire #2
          configFile.setDB2Deployed(); // Record the burn
          checkConfigStatus(); // Print out config status
        }
        if (configFile.getDB3FlagStatus()) { // Read config file for flag to burn sprite burn wire #3 // TODO: Decide whether or not to add a battery threshold check
          burn.burnSpriteThree(); // Burn sprite burn wire #3
          configFile.setDB3Deployed(); // Record the burn
          checkConfigStatus(); // Print out config status
        }
      }

      configFile.setStandby(); // Go back to standby mode

    }

    //////////////////////
    // Enter sleep mode //
    //////////////////////
    // TODO: Sleep mode commented out only while testing all other software functionality
    // sleepTimer.sleep(); // Go into sleep mode until next interrupt

  }
}

// TODO: this is a temporary data generator for testing purposes only, needs to be deleted in final release
//////////////////////////////////////////////////////////////////////////////////////
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
  char filler = 'A';
  for(int i = 0; i < 50; i++){
    sensorLog1.refresh();
    byte buf[DATA_WIDTH];
    buf[0] = (byte)filler;
    buf[1] = (byte)filler;
    buf[2] = (byte)filler;
    sensorLog1.writeDataEntry(buf);
    filler++;
  }
  filler = 'A';
  for(int i = 0; i < 50; i++){
    sensorLog1.refresh();
    byte buf[DATA_WIDTH];
    buf[0] = (byte)filler;
    buf[1] = (byte)filler;
    buf[2] = (byte)filler;
    sensorLog2.writeDataEntry(buf);
    filler++;
  }
  filler = 'A';
  for(int i = 0; i < 50; i++){
    sensorLog1.refresh();
    byte buf[DATA_WIDTH];
    buf[0] = (byte)filler;
    buf[1] = (byte)filler;
    buf[2] = (byte)filler;
    sensorLog3.writeDataEntry(buf);
    filler++;
  }
  filler = 'A';
  for(int i = 0; i < 50; i++){
    sensorLog1.refresh();
    byte buf[DATA_WIDTH];
    buf[0] = (byte)filler;
    buf[1] = (byte)filler;
    buf[2] = (byte)filler;
    sensorLog4.writeDataEntry(buf);
    filler++;
  }
}
//////////////////////////////////////////////////////////////////////////////////////


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
void deployAntennas() {
  SerialUSB.println("Begining Antenna Deployment Procedure");
  timeout.start(ANTENNAWAITTIME); // Starts a timeout timer
  while(!configFile.getAB1status() || !configFile.getAB2status()) { // While either antenna burn wire is not burned
    if(!configFile.getAB1status() && batteryAboveThreshold()) { // Burn the first antenna wire if it hasn't already been done and the battery threshold is met
      timeout.pause(); // Pause timeout counter
      burn.burnAntennaOne(); // Burn
      timeout.resume(); // Resume timeout counter
      configFile.setAB1Deployed(); // Set flag that reports that the wire has been burned
      SerialUSB.println("Antenna One Burned");
      checkConfigStatus(); // Print out config status
    }
    if(!configFile.getAB2status() && batteryAboveThreshold()) { // Burn the second antenna wire if it hasn't already been done and the battery threshold is met
      timeout.pause(); // Pause timeout counter
      burn.burnAntennaTwo(); // Burn
      timeout.resume(); // Resume timeout counter
      configFile.setAB2Deployed(); // Set flag that reports that the wire has been burned
      SerialUSB.println("Antenna Two Burned");
      checkConfigStatus(); // Print out config status
    }
    if (configFile.getAB1status() && configFile.getAB2status()) { // Break if both antenna are deployed
      break;
    }
    if (timeout.triggered()) { // Checks time for timeout
      break;
    }
  }
}

void checkConfigStatus() {
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
  SerialUSB.print("Antenna Timer: ");
  SerialUSB.println(configFile.checkAntennaTimer());
  SerialUSB.println();
}
