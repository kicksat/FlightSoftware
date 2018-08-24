////KickSatConfig.h
//
//
//#ifndef KICKSATCONFIG_h
//#define KICKSATCONFIG_h
//
//#include <Arduino.h>
//#include <SdFat.h>
//#include <KickSatLog.h>
//
//
//#define CS SPI_CS_SD
//
////These are the bytes that represent various modes
////Note: currently characters for readability TODO: change to final flight indicators (if desired- will work either way)
//#define HOLD_BYTE 'h'//0x1E
//#define STANDBY_BYTE 's'//0x02
//#define ARMED_BYTE 'a'//0x06
//#define DEPLOYED_BYTE 'd'//0x0E
//
////defines the location in the config file of flags and other status indicators
//#define STATUS_LOC 0
//#define AB1_LOC 1 //Antenna status indicates that the antenna burn has been completed
//#define AB2_LOC 2
//#define DB1_LOC 3 //burn status indicate that a burn has been completed when true
//#define DB2_LOC 4
//#define DB3_LOC 5
//#define DB1_FLAG_LOC 6  //Burn flags indicate that a burn has been requested when true
//#define DB2_FLAG_LOC 7
//#define DB3_FLAG_LOC 8
//#define TIMER_LOC 9
//
//#define FLAG_TRUE '1' //true and false are declared as characters for readability TODO: change to flight true/false (if desired- will work either way)
//#define FLAG_FALSE '0'
//
//#define NUM_FILES 3 //number of files
//#define NUM_ENTRIES 10 //number of bytes in each file
//
//byte buf[NUM_FILES];
//int votes[NUM_FILES]; //represents the number of times each byte is represented
//char entryBuf[NUM_ENTRIES]; // Creates a default zero array to write to the file
//bool boolBuf[NUM_FILES];
//
//class KickSatConfig {
//public:
//  KickSatConfig(SdFat _sd);
//  int checkAntennaTimer();
//  void incrementAntennaTimer();
//
//  //returns the byte stored in the current status byte (represents current mode)
//  byte getStatus();
//
//  //getters: each function will return true if that flag is set
//  bool getAB1status();
//  bool getAB2status();
//  bool getDB1status(); //indicates it has been burned
//  bool getDB2status();
//  bool getDB3status();
//  bool getDB1FlagStatus(); //indicates burn has been requested
//  bool getDB2FlagStatus();
//  bool getDB3FlagStatus();
//
//  //getters for status byte will return true if it is in requested mode, otherwise false
//  bool getHoldstatus();
//  bool getStandbyStatus();
//  bool getArmedStatus();
//  bool getDeployedStatus();
//
//  //setters: will set the selected flag true
//  void setAB1Deployed();
//  void setAB2Deployed();
//  void setDB1Deployed(); //indicates it has been burned
//  void setDB2Deployed();
//  void setDB3Deployed();
//  void setDB1Flag(); //indicates burn has been requested
//  void setDB2Flag();
//  void setDB3Flag();
//
//  //status byte setters- changes status byte to specified mode
//  void setHold();
//  void setStandby();
//  void setArmed();
//  void setDeployed();
//
//  bool init(); //sets up config files and initializes sd if it hasn't already happened. Use during setup
//
//  void startSD(); //manually sets chip select low (active low)
//  void endSD();  //manually sets chip select high
//
//  bool errorCorrect(); //when this function is called it checks that all files can open and replaces a file if it cannot open
//  bool errorCorrectContents(); //checks bytes in all three files and uses voting system to make sure they all match
//
//
//  bool available(String filename);
//
//private:
//
//  bool initFile(String fileName); //sets up a single file, initializes NUM_ENTRIES to default value
//  bool writeByteToAll(byte data, int location); //writes a single byte to all three files
//  bool readByteFromAll(byte* data, int location);//reads a byte from all three files, automatically error corrects if one is different
//  bool writeByte(String filename, byte data, int location); //writes a single byte to a single file
//  bool readByte(String filename, int location, byte* buf); //reads a single byte from a single file
//
//  SdFat SD;
//  byte AB1status;
//  byte AB2status;
//  byte DB1status;
//  byte DB2status;
//  byte DB3status;
//  byte DB1FlagStatus;
//  byte DB2FlagStatus;
//  byte DB3FlagStatus;
//  byte statusByte;
//  int antennaTimer;
//
//
//};
//
//
//
//#endif
