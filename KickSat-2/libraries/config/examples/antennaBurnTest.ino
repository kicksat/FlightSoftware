#include "RTCCounter.h"
#include "KickSatConfig.h"

#define HOLDTIME 10000 // Time to hold after initial deployment, in minutes NOTE: this number CAN'T be bigger than 255
#define BATTERYTHRESHOLD 2.055 // Battery must be above this threshold to exit standby mode

//watchdog stuff
int cnt;
void watchdog();
bool LEDSTATE = false;
Counter watchdogTimer; // creates timer object


bool incrementTimerFlag = false;

Counter beaconTimer; // creates timer object
Counter holdTimer;


void setup(){
  pinMode(LED_BUILTIN, OUTPUT); // Defines builtin LED pin mode to output
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
  configFile.writeByteToThree((byte)0, TIMER_LOC);

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

void loop(){

}


bool batteryAboveThreshold() {
  //return power.readBattVoltage() > BATTERYTHRESHOLD; // TODO: Read battery doesn't exist but should
  return 3 > BATTERYTHRESHOLD;
}


void holdModeCallback() {
  //sets flag to tell main code to increment time in config file on SD card
  incrementTimerFlag = true;
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

//prints out all values stored in config file and tests all getters
void checkConfigStatus(){
  SerialUSB.println();
  SerialUSB.print("Status: ");
  SerialUSB.println((char)configFile.getStatus());
  SerialUSB.print("AB1 deployed: ");
  SerialUSB.println(configFile.getAB1status());
  SerialUSB.print("AB2 deployed: ");
  SerialUSB.println(configFile.getAB2status());
  SerialUSB.print("Hold mode: ");
  SerialUSB.println(configFile.getHoldstatus());
  SerialUSB.print("Standby mode: ");
  SerialUSB.println(configFile.getStandbyStatus());
  SerialUSB.print("Armed mode: ");
  SerialUSB.println(configFile.getArmedStatus());
  SerialUSB.print("Deployed mode: ");
  SerialUSB.println(configFile.getDeployedStatus());
  SerialUSB.print("Antenna Timer: ");
  SerialUSB.println(configFile.checkAntennaTimer());
  SerialUSB.println();
  delay(500);
}

void watchdog() { // Function that runs every time watchdog timer triggers
  if (LEDSTATE) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  LEDSTATE = !LEDSTATE;
}
