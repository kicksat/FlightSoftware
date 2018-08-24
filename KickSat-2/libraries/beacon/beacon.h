#ifndef BEACON_h
#define BEACON_h

#include <KickSatLog.h>
#include <SD_DataFile.h>
#include <SdFat.h>
#include <FSWvariables.h>

#define MAX_PACKET_SIZE 200 //TODO: look at radiohead and ax25-> radiohead is 255 find how much
#define DATA_WIDTH 3
#define BEACONS_PER_DOWNLINK 10
#define MAX_LOG_SIZE 180


//This function takes in a character array and formats it into data ready to beacon then returns the length
int beacon(char* output, SD_DataFile sensorLog){
  char logData[MAX_PACKET_SIZE];
  logfile.read(1, logData);
  String result(logData);
  int numEntries = (MAX_PACKET_SIZE - result.length())/DATA_WIDTH;
  int currIndex = sensorLog.size() - 1;
  for(int i = 0; i < numEntries; i++){
    byte dataEntry[DATA_WIDTH];
    sensorLog.readDataEntry(currIndex, dataEntry);
    currIndex --;
    for(int j = 0; j < DATA_WIDTH; j++){
       result += (char)dataEntry[j];
    }
  }
  int len = result.length();
  result.toCharArray(output, len);
  return len;
}


void downLinkBeacon(SD_DataFile sensorLog, String* beacons){
    char buf[BEACONS_PER_DOWNLINK*MAX_LOG_SIZE];
    //String beacons[BEACONS_PER_DOWNLINK];
    logfile.read(BEACONS_PER_DOWNLINK, buf);
    String allTheBeacons(buf);
    int index = 0;
    for(int i = BEACONS_PER_DOWNLINK - 1; i >= 0; i--){
      int endlineIndex = allTheBeacons.indexOf('\n', index);
      beacons[i] = allTheBeacons.substring(index, endlineIndex);
      index = endlineIndex + 1;
    }
    int currIndex = sensorLog.size()-1;
    for(int i = 0; i < BEACONS_PER_DOWNLINK; i++){
       int numEntries = (MAX_PACKET_SIZE - beacons[i].length())/DATA_WIDTH;
       for(int k = 0; k < numEntries; k++){
          byte dataEntry[DATA_WIDTH];
          sensorLog.readDataEntry(currIndex, dataEntry);
          currIndex --;
          for(int j = 0; j < DATA_WIDTH; j++){
             beacons[i] += (char)dataEntry[j];
          }
      }
    }

}




#endif
