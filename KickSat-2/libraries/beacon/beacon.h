/*
Beacon - Creates buffers with the data to be downlinked. Handles both regular and "machine-gun" beacon formatting

Last update on: 27-8-18
by: Andrea
*/

#ifndef BEACON_h
#define BEACON_h

#include <SD_DataFile.h>
#include <FSWvariables.h>

#define MAX_PACKET_SIZE       200 //TODO: look at radiohead and ax25-> radiohead is 255 find how much
#define SENSOR_DATA_WIDTH     3 
#define BEACONS_PER_DOWNLINK  10 //TODO: Is this the right amount of beacons/downlink?
#define MAX_LOG_SIZE          180 

//Formats a single beacon containing log and sensor data and stores it in a string
void beacon(String* output, SD_DataFile sensorLog){
  char logData[MAX_PACKET_SIZE];
  logfile.read(1, logData); //read 1 log entry and save to a buffer
  String result(logData); //convert to string
  int numEntries = (MAX_PACKET_SIZE - result.length())/SENSOR_DATA_WIDTH; //sensor entries to be appended to beacon 
  int currIndex = sensorLog.size() - 1;
  for(int i = 0; i < numEntries; i++) { //pad beacon with sensor data entries to have a packet of maximum size to downlink
    byte dataEntry[SENSOR_DATA_WIDTH];
    sensorLog.readDataEntry(currIndex, dataEntry);
    currIndex --;
    for(int j = 0; j < SENSOR_DATA_WIDTH; j++) {
      result += (char)dataEntry[j];
    }
  }
  *output = result;
}

//Formats multiple beacons containing log and sensor data to be downlinked 
void downLinkBeacon(SD_DataFile sensorLog, String* beacons) {
  char buf[BEACONS_PER_DOWNLINK*MAX_LOG_SIZE];
  logfile.read(BEACONS_PER_DOWNLINK, buf); //reads and stores many log entries into a buffer
  String allTheBeacons(buf); //convert to string
  int index = 0;
  for(int i = BEACONS_PER_DOWNLINK - 1; i >= 0; i--) { //split buffer into the different beacons and store each in beacons 
    int endlineIndex = allTheBeacons.indexOf('\n', index);
    beacons[i] = allTheBeacons.substring(index, endlineIndex);
    index = endlineIndex + 1;
  }
  int currIndex = sensorLog.size() - 1; //size() returns the number of entries in the sensor file
  for(int i = 0; i < BEACONS_PER_DOWNLINK; i++) { //pad each beacon with sensor data to always downlink packets of maximum size 
    int numEntries = (MAX_PACKET_SIZE - beacons[i].length())/SENSOR_DATA_WIDTH; //sensor entries to be added in current becaon
    for(int j = 0; j < numEntries; j++) {
      byte dataEntry[SENSOR_DATA_WIDTH];
      sensorLog.readDataEntry(currIndex, dataEntry);
      currIndex --;
      for(int k = 0; k < SENSOR_DATA_WIDTH; k++) { //append a sensor data entry to beacon
        beacons[i] += (char)dataEntry[k];
      }
    }
  }
}

#endif
