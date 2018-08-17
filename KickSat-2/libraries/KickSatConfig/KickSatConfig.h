//KickSatConfig.h


#ifndef KICKSATCONFIG_h
#define KICKSATCONFIG_h

#include "Arduino.h"
#include "SD.h"
#include "KickSatLog.h"

#define CS SPI_CS_SD
#define HOLD_BYTE 'h'//0x1E
#define STANDBY_BYTE 's'//0x02
#define ARMED_BYTE 'a'//0x06
#define DEPLOYED_BYTE 'd'//0x0E
#define STATUS_LOC 0
#define AB1_LOC 1
#define AB2_LOC 2
#define DB1_LOC 3
#define DB2_LOC 4
#define DB3_LOC 5
#define TIMER_LOC 6
#define FLAG_TRUE '1'
#define FLAG_FALSE '0'
#define NUM_FILES 3
#define NUM_ENTRIES 7


class KickSatConfig {
public:
  int checkAntennaTimer();
  void incrementAntennaTimer();
  byte getStatus();
  bool getAB1status();
  bool getAB2status();
  bool getDB1status();
  bool getDB2status();
  bool getDB3status();
  bool getHoldstatus();
  bool getStandbyStatus();
  bool getArmedStatus();
  bool getDeployedStatus();

  void setAB1Deployed();
  void setAB2Deployed();
  void setDB1Deployed();
  void setDB2Deployed();
  void setDB3Deployed();
  void setHold();
  void setStandby();
  void setArmed();
  void setDeployed();

  bool init();
  void startSD();
  void endSD();

  bool errorCorrectOpening();
  bool errorCorrectContents();

  bool initFile(String fileName);

  //private:

  bool available(String filename);
  bool writeByteToThree(byte data, int location);
  bool readByteFromThree(byte data[3], int location);
  bool writeByte(String filename, byte data, int location);
  byte readByte(String filename, int location);
};

extern KickSatConfig configFile; // Create config object


#endif
