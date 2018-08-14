/**
Data handler for SD card, KickSatLog.h
Purpose: Library for formatting and piping read and write requests on the SD card for KickSat

@author Ralen Toledo & Antony Gillette
@version 1.0 08/12/18
*/

/*
Note: The average entry size is 175 bytes
Note: Saving one entry per minute, allows ~40 years of data entry
*/

#ifndef KICKSATLOG_h
#define KICKSATLOG_h

#include "Arduino.h"
#include "SD.h"

// User-defined file name
///////////////////////////////////
#define LOGNAME "log.log" // TODO: Select final file name
#define MAXCHARS 18000 // Never downlink more than this characters (roughly 100 lines), crashes if 50000
///////////////////////////////////


#define CS SPI_CS_SD // Sets the chip select for the KickSat board


struct LogData {
  uint16_t logNum;
  uint8_t status;
  float powerData[3];
  uint16_t dateTime[2];
  float gpsData[3];
  float imuData[9];
};


extern bool SDStatus; // Makes an external, global variable for SD initialization status, true=SD initialized
extern struct LogData data; // Structure for all data storage


class LogHandler {
public:
  bool init(void); // Initialize SD card
  bool available(void);// Open (or make, if it doesn't already exist) the log file, verifying that it is read/write capable
  void startSD(void); // Starts communication with the SD card
  void endSD(void); // Ends communication with the SD card
  bool appendData(void); // Function to append data to log file from the current values in the data structure
  bool appendBuffer(char *buf); // Function to append data to log file from the provided buffer, useful for recording uplink commands
  bool read(uint16_t numLines, char *buf); // Read last N lines in the log file
  void resetLogStruct(void); // Sets all data to zero in the Data structure
  bool compileHealth(char *buf); // Function to compile and return health data for beacon
};

extern LogHandler logfile; // Creates an external, global logfile object


#endif
