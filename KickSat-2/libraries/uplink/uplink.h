/**
Uplink handler for KickSat, uplink.h
Purpose: Library for handling the reading and processing of uplink to the satellite
@author Ralen Toledo, Connor Settle
@version 1.0 08/12/18
*/

#ifndef UPLINK_h
#define UPLINK_h

#include <KickSatLog.h>
#include <Checksum.h>
// #include <ax25.h>
// #include <RadioHead.h>

#define LISTENINGDURATION 5 // Defined in seconds

Checksum checksumHandler;

const byte commandDictionary[7][12] = {// REDACTED //};

bool parseUplink(char *buf);
void processUplink(char *buf);

///////////////////////
// Listen for uplink //
//////////////////////////////////////////////////////////////////////////////
bool listenForUplink(char *buf) {
  timeout.start(LISTENINGDURATION); // Start timeout timer for the listening duration
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

bool parseUplink(char *buf) {
  ////////////////////////////////////////////////////////
  // TODO: change from Serial.read to radio.read(char *buf)
  uint32_t i = 0;
  // while(radio.available() > 0) { // Read until the entire buffer has been read // TODO: This function doesn't exist but should
  //   buf[i] = radio.read(); // Reads the available char (ASCII) // TODO: This function doesn't exist but should
  while(SerialUSB.available() > 0) { // Read until the entire buffer has been read
    buf[i] = SerialUSB.read(); // Reads the available char (ASCII)
    i++;
  }
  buf[i] = '\0';
  ////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////
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
  ////////////////////////////////////////////////////////
}

////////////////////
// Process uplink //
//////////////////////////////////////////////////////////////////////////////
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
  return -1;
}

void processUplink(char *buf) {

  uint8_t command = extractCommand(buf);

  // respond to the command
  switch(command)
  {
    // Burn wires
    case 0:
    SerialUSB.println("Command: Burn Wire");
    //TODO: burn the burn wires to deploy sprites
    break;
      
    // Begin downlink
    case 1:
    SerialUSB.println("Command: Start Downlink");
    //TODO: increase the frequency of chiprs and begin sending older log data
    break;
      
    case 2:
    SerialUSB.println("Command: Uplink Sensor Config");
    //TODO: read data from uplink and write new data to sensor config files
    //IMPORTANT: any uplink can be no longer than 64 bytes so configs must be short
    break;

    // Send the mission status files
    case 3:
    SerialUSB.println("Command: Downlink Mission Status");
    //TODO: send down current mission status byte
    //lets the ground station know what the satellite thinks its doing
    break;

    // Enter arming mode, exit standby mode
    case 4:
    SerialUSB.println("Command: Enter Arming Mode");
    // TODO: enter arming mode
    // send: "Entered arming mode"
    // This is a transition condition in the more general state diagram
    // we will exit standby mode here
    break;

    // Enter End of Life mode
    case 5:
    SerialUSB.println("Command: Enter End of Life Mode");
    // Send: ACK, are you sure you want to explode all of our hard work into pixie dust?
    // serial_transmit("Are you sure you want to kill KickSat II? (y/n)");
    // wait for response: if (response != "yes explode" ) --> go back into standby mode
    // command = return_selection();
    // SerialUSB.print("case 7, command: ");
    // SerialUSB.println(command);
    // if((command == 41) || (command == 73))
    // {
    //   // go into end of life mode
    //   // this is a state change into end of life mode
    //   SerialUSB.println("Killing the goddamn 3u that never deserved to live MUAHAHAHAHA!");
    // }
    // else
    // {
    //   // do not kill the satelite
    //
    //   command = 0;
    // }

    break;

    // No command --> go back to sleep and go through another standby mode loop
    case 6:
    //
    SerialUSB.println("Doing no command... going to sleep");
    break;

    // No command --> go back to sleep and go through another standby mode loop
    default:
    //could potentially trigger due to failed command read
    SerialUSB.println("Doing no command... going to sleep");
    break;
  }

}

#endif
