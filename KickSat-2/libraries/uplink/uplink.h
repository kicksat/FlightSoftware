/**
Uplink handler for KickSat, uplink.h
Purpose: Library for handling the reading and processing of uplink to the satellite
@author Ralen Toledo
@version 1.0 08/12/18
*/

#ifndef UPLINK_h
#define UPLINK_h

#include <KickSatLog.h>
#include <ChecksumHandler.h>
#include <KickSatConfig.h>
#include <RTCCounter.h>
#include <FSWvariables.h>
#include <beacon.h>
// #include <ax25.h>
// #include <RadioHead.h>

#define NUM_COMMANDS 4//redacted
#define NUM_COMMANDS_ARM 1
#define COMMAND_WIDTH 3//redacted
#define UINT8_TRUE 0
#define UINT8_FALSE 255
#define METADATA_WIDTH 255

const byte commandDict[NUM_COMMANDS][COMMAND_WIDTH] = {{'d','o','g'},{'c','a','t'},{'p','i','g'},{'o','w','l'}/* REDACTED */}; // TODO: replace redacted section
const byte commandDictArm[NUM_COMMANDS_ARM][COMMAND_WIDTH] = {{'b','r','n'}};

String beacons[BEACONS_PER_DOWNLINK];

void processUplink(char* buf, uint8_t command);
uint8_t parseUplink(char* buf);
uint8_t listenForUplink(char *buf, uint8_t duration);
uint8_t listenForUplinkArmingMode(char *buf, uint8_t duration);
uint8_t parseUplink(char* buf);
uint8_t extractArmedCommand(byte* buf);
uint8_t extractCommand(byte* buf);
int getSensorMetadata(char* buf, char* metadata);


///////////////////////
// Listen for uplink //
///////////////////////
uint8_t listenForUplink(char *buf, uint8_t duration) {
  //SerialUSB.println("Listening for uplink...");
  timeout.start(duration);
  while(1) { // Wait for uplink, retreive from buffer
    // if (radio.available()) { // TODO: This function needs to be created and replaces if (SerialUSB.available())
    if (SerialUSB.available()) { // TODO: change if radio is available, not serial
      uint8_t command = parseUplink(buf);
      if (command != UINT8_FALSE) {
        return command;
      }
    }
    if (timeout.triggered()) { // Checks time for timeout
      SerialUSB.println("Uplink Timeout");
      return UINT8_FALSE;
    }
  }
}


/////////////////////////////////////
// Listen for uplink in armingmode //
/////////////////////////////////////
uint8_t listenForUplinkArmingMode(char *buf, uint8_t duration) {
  timeout.start(duration); // Start timeout timer for the listening duration
  while(1) { // Wait for uplink, retreive from buffer
    // if (radio.available()) { // TODO: change if radio is available, not serial
    if (SerialUSB.available()) { // If data is in buffer to be read
      uint8_t command = parseUplink(buf); // Read from buffer and validate uplink
      processUplink(buf, command); // Process uplink while in arming mode
    }
    if (configFile.getDB1status() && configFile.getDB2status() && configFile.getDB3status()) {
      return UINT8_TRUE;
    }
    if (timeout.triggered()) { // Checks time for timeout
      SerialUSB.println("Uplink Timeout");
      break;
    }
  }
  if (configFile.getDB1status() || configFile.getDB2status() || configFile.getDB3status()) {
    return UINT8_TRUE;
  } else {
    return UINT8_FALSE;
  }
}


//////////////////
// Parse uplink //
//////////////////
uint8_t parseUplink(char* buf) {
  uint32_t i = 0;
  //SerialUSB.println("read from buffer: ");
  while(SerialUSB.available() > 0) { // Read until the entire buffer has been read
    buf[i] = SerialUSB.read(); // Reads the available char (ASCII)
    SerialUSB.print(buf[i]);
    i++;
  }

  buf[i] = Checksum.calculateChecksum((byte*)buf, i);/////Added in so that the checksum works, usually will be included in uplink
  i++;
  buf[i] = '\0';
  SerialUSB.println();

  // Checksum data
  if (Checksum.evaluateChecksum((byte*)buf, i)) { // If uplink is valid, to be replaced by checksum validation
    SerialUSB.print("ACK:"); // If checksum is valid, respond with ACK
    SerialUSB.println(buf);
    // radio.send("ACK") // If checksum is valid, respond with ACK // TODO: This function doesn't exist but should
    uint8_t command;
    if(configFile.getArmedStatus()){
      command = extractArmedCommand((byte*)buf);
    }else{
      command = extractCommand((byte*)buf); // returns index of command if found, UINT8_FALSE if no command exists
    }
    //SerialUSB.println(command);
    if(command != UINT8_FALSE){
      SerialUSB.print("Command: ");
      for(int f = 0; f < COMMAND_WIDTH; f++){
        SerialUSB.print((char)commandDict[command][f]);
      }
      SerialUSB.println();

      return command;
    }
  }
  SerialUSB.print("NACK:");
  SerialUSB.println(buf);
  // radio.send("NACK") // If checksum is not valid, respond with NACK // TODO: This function doesn't exist but should
  return UINT8_FALSE; // Return unsuccessfull uplink
  ////////////////////////////////////////////////////////
}

///////////////////
// Parse command //
///////////////////
uint8_t extractCommand(byte* buf) {
  for (int i = 0; i < NUM_COMMANDS; i++) {
    bool flag = true;
    for (int n = 0; n < COMMAND_WIDTH; n++) {
      if (commandDict[i][n] != buf[n]) {
        flag = false;
        break;
      }
    }
    if (flag) {
      return i;
    }
  }
  SerialUSB.println("INVALID COMMAND");
  return UINT8_FALSE;
}

uint8_t extractArmedCommand(byte* buf) {
  for (int i = 0; i < NUM_COMMANDS_ARM; i++) {
    bool flag = true;
    for (int n = 0; n < COMMAND_WIDTH; n++) {
      if (commandDictArm[i][n] != buf[n]) {
        flag = false;
        break;
      }
    }
    if (flag) {
      return i + NUM_COMMANDS;
    }
  }
  SerialUSB.println("INVALID COMMAND");
  return UINT8_FALSE;
}


////////////////////
// Process uplink //
////////////////////
void processUplink(char* buf, uint8_t command) {

  char metadata[METADATA_WIDTH];
  int metadataLen;
  byte metadataByte;

  switch(command) { // respond to the command

    case 0: // Downlink
      SerialUSB.println("Command: Start Downlink");
      // uint8_t sensor_num = metadata[0]; //the first byte of metadata is the number of the sensor board we're rewriting to
      //  if (sensor_num == 1) {
      //    downLinkBeacon(sensorLog1, beacons);
      //  } else if (sensor_num == 2) {
      //    downLinkBeacon(sensorLog2, beacons);
      //  } else if (sensor_num == 3) {
      //    downLinkBeacon(sensorLog3, beacons);
      //  } else if (sensor_num == 4) {
      //    downLinkBeacon(sensorLog4, beacons);
      //  }
      //  for(int i = 0; i < BEACONS_PER_DOWNLINK; i++){
      //    SerialUSB.println(beacons[i]);
      //  }
    break;

    case 1: // set Arming mode
      SerialUSB.println("Command: Enter Arming Mode");
      configFile.setArmed(); // Set flag to enter arming mode
    break;

    case 2: // reflash sensor config
      metadataLen = getSensorMetadata(buf, metadata);
      SerialUSB.println("Command: Uplink Sensor Config");
      // uint8_t sensor_num = metadata[0]; //the first byte of metadata is the number of the sensor board we're rewriting to
      // if (sensor_num == 1) {
      //   kSensor1.rewriteConfigs((byte*)&(metadata[1]), metadataLen - 1);
      // } else if (sensor_num == 2) {
      //   kSensor2.rewriteConfigs((byte*)&(metadata[1]), metadataLen - 1);
      // } else if (sensor_num == 3) {
      //   kSensor3.rewriteConfigs((byte*)&(metadata[1]), metadataLen - 1);
      // } else if (sensor_num == 4) {
      //   kSensor4.rewriteConfigs((byte*)&(metadata[1]), metadataLen - 1);
      // }

      //TODO: read data from uplink and write new data to sensor config files
      //IMPORTANT: any uplink can be no longer than 64 bytes so configs must be short
    break;

    case 3: // No command --> go back to sleep and go through another standby mode loop
      SerialUSB.println("KickSat is alive! going back to sleep");
      // radio.send(ax25("KickSat is alive!")); // Send health data through radio // TODO: This function doesn't exist yet but should and also is currently probably syntactically incorrect
    break;

    case 4: // Burn wires
      SerialUSB.println("Command: Burn Wire");
      metadataByte = buf[COMMAND_WIDTH]; //grab the first byte after the command, this is the burn metadata byte
      if (metadataByte == (byte)'1') { // Burn wire one
        configFile.setDB1Flag();
      } else if (metadataByte == (byte)'2') { // Burn wire two
        configFile.setDB2Flag();
      } else if (metadataByte == (byte)'3') { // Burn wire three
        configFile.setDB3Flag();
      } else if (metadataByte == (byte)'a') { // Burn all wires
        configFile.setDB1Flag();
        configFile.setDB2Flag();
        configFile.setDB3Flag();
      }
    break;

    default: // No command --> go back to sleep and go through another standby mode loop
      //could potentially trigger due to failed command read
      SerialUSB.println("Doing no command... going to sleep");
    break;
    
  }
}

int getSensorMetadata(char* buf, char* metadata){
  int i = 0;
  char currChar = 1;
  SerialUSB.print("Sensor Metadata: ");
  while(currChar != '\0'){
    currChar = buf[i+COMMAND_WIDTH];
    metadata[i] = currChar;
    SerialUSB.print(metadata[i]);
    i++;
  }
  SerialUSB.println();
  SerialUSB.print("Metadata length: ");
  SerialUSB.println(i-2);
  return (i-2);
}


#endif
