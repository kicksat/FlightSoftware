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
// #include <ax25.h>
// #include <RadioHead.h>
#define NUM_COMMANDS //redacted
#define COMMAND_WIDTH //redacted


ChecksumHandler checksumHandler;
bool armingMode;

const byte commandDict[NUM_COMMANDS][COMMAND_WIDTH] = {/* REDACTED */};

bool parseUplink(char *buf);
void processUplink(char *buf);

///////////////////////
// Listen for uplink //
///////////////////////
bool listenForUplink(char *buf, uint8_t duration) {
  timeout.start(duration); // Start timeout timer for the listening duration
  while(1) { // Wait for uplink, retreive from buffer
    // if (radio.available()) { // TODO: change if radio is available, not serial
    if (SerialUSB.available()) { // If data is in buffer to be read
      if (parseUplink(buf)) { // Read from buffer and validate uplink
        return true;
      }
    }
    if (timeout.triggered()) { // Checks time for timeout
      SerialUSB.println("Uplink Timeout");
      return false;
    }
  }
}

/////////////////////////////////////
// Listen for uplink in armingmode //
/////////////////////////////////////
bool listenForUplinkArmingMode(char *buf, uint8_t duration) {
  timeout.start(duration); // Start timeout timer for the listening duration
  while(1) { // Wait for uplink, retreive from buffer
    // if (radio.available()) { // TODO: change if radio is available, not serial
    if (SerialUSB.available()) { // If data is in buffer to be read
      if (parseUplink(buf)) { // Read from buffer and validate uplink
        processUplink(); // Process uplink while in arming mode
      }
    }
    if (timeout.triggered()) { // Checks time for timeout
      SerialUSB.println("Uplink Timeout");
      break;
    }
    if (getDB1status() && getDB2status() && getDB3status()) {
      return true;
    }
  }
  if (getDB1status() || getDB2status() || getDB3status()) {
    return true;
  }
}

//////////////////
// Parse uplink //
//////////////////
bool parseUplink(char *buf) {
  // Read available data from buffer
  // TODO: change from Serial.read to radio.read(char *buf)
  uint32_t i = 0;
  // while(radio.available() > 0) { // Read until the entire buffer has been read // TODO: This function doesn't exist but should
  //   buf[i] = radio.read(); // Reads the available char (ASCII) // TODO: This function doesn't exist but should
  while(SerialUSB.available() > 0) { // Read until the entire buffer has been read
    buf[i] = SerialUSB.read(); // Reads the available char (ASCII)
    i++;
  }
  buf[i] = '\0';

  // Checksum data
  // if (checksum(buf)) // Verifies checksum of read uplink // TODO: Incorporate real checksum check here for input
  if (checksumHandler.evaluateChecksum(buf, i)) { // If uplink is valid, to be replaced by checksum validation
    SerialUSB.print("ACK:"); // If checksum is valid, respond with ACK
    SerialUSB.println(buf);
    // radio.send("ACK") // If checksum is valid, respond with ACK // TODO: This function doesn't exist but should
    return true; // Return successfull uplink
  } else {
    SerialUSB.print("NACK:");
    SerialUSB.println(buf);
    // radio.send("NACK") // If checksum is not valid, respond with NACK // TODO: This function doesn't exist but should
    return false; // Return unsuccessfull uplink
  }
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
}

////////////////////
// Process uplink //
////////////////////
void processUplink(char *buf) {

  uint8_t command = extractCommand(buf);
  switch(command) { // respond to the command
    case 0: // Burn wires
    SerialUSB.println("Command: Burn Wire");
    byte metadata = buf[COMMAND_WIDTH]; //grab the first byte after the command, this is the matadata byte
    if (metadata == 0x01) { // Burn wire one
      setDB1Deployed();
    } else if (metadata == 0x02) { // Burn wire two
      setDB2Deployed();
    } else if (metadata == 0x03) { // Burn wire three
      setDB3Deployed();
    } else if (metadata == 0x0F) { // Burn all wires
      setDB1Deployed();
      setDB2Deployed();
      setDB3Deployed();
    }
    break;

    case 1: // Begin downlink
    SerialUSB.println("Command: Start Downlink");
    //TODO: increase the frequency of beacons and begin sending older log data
    //NOTE: this is its own function, which shits out a bunch of data
    break;

    case 2:
    SerialUSB.println("Command: Uplink Sensor Config");
    //TODO: read data from uplink and write new data to sensor config files
    //IMPORTANT: any uplink can be no longer than 64 bytes so configs must be short
    break;

    case 3: // Enter arming mode, exit standby mode
    SerialUSB.println("Command: Enter Arming Mode");
      setArmed(); // Set flag to enter arming mode
    break;


    case 4: // No command --> go back to sleep and go through another standby mode loop
    SerialUSB.println("KickSat is alive!");
    // radio.send(ax25("KickSat is alive!")); // Send health data through radio // TODO: This function doesn't exist yet but should and should be syntactically incorrect
    break;

    default: // No command --> go back to sleep and go through another standby mode loop
    //could potentially trigger due to failed command read
    SerialUSB.println("Doing no command... going to sleep");
    break;
  }
}

#endif
