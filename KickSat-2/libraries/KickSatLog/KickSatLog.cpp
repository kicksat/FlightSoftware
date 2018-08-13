/**
Data handler for SD card, KickSatLog.cpp
Purpose: Library for formatting and piping read and write requests on the SD card for KickSat

@author Ralen Toledo & Antony Gillette
@version 1.0 08/12/18
*/

/*
Note: The average entry size is 175 bytes
Note: Saving one entry per minute, allows ~40 years of data entry
*/

#include "KickSatLog.h"

bool SDStatus = false; // Global variable for SD initialization status, true=SD initialized
struct LogData data; // Structure for all data storage


// Initialize SD card
bool LogHandler::init() {
  if (!SDStatus) { // If the SD card is not initialized
    pinMode(CS, OUTPUT); // Set pinmode for the SD card CS to output
    resetLogStruct(); // Sets all data to zero in the data structure
    startSD(); // Starts communication with the SD card
    SDStatus = SD.begin(CS); // Record initialization of the SD card
    endSD(); // Ends communication with the SD card
  }
  return SDStatus; // Returns true if SD card initializes
}


// Open (or make, if it doesn't already exist) the log file, verifying that it is read/write capable
bool LogHandler::available() {
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  File logFileHandle = SD.open(LOGNAME, FILE_WRITE); // Open file (or create new file if it doesn't exist)
  if (!logFileHandle) { // If file is not accessible
    SD.remove(LOGNAME); // Delete old file
    File logFileHandle = SD.open(LOGNAME, FILE_WRITE); // Create new file
  }
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    logFileHandle.close(); // Close file
  }
  endSD(); // Ends communication with the SD card
  return fileStatus; // Returns file status
}


// Starts communication with the SD card
void LogHandler::startSD() {
  digitalWrite(CS, LOW); // Set chip select to LOW to start communication with SD card
}


// Ends communication with the SD card
void LogHandler::endSD() {
  digitalWrite(CS, HIGH); // Set chip select to HIGH to end communication with SD card
}


// Function to append data to log file from the current values in the data structure
bool LogHandler::appendData() {
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  File logFileHandle = SD.open(LOGNAME, FILE_WRITE); // Open file for writing
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    char buf[256]; // Creates empty array that will be filled and then written to SD, max size of 256 is more than enough (avg size is 180 bytes)
    // Writes data structure to buffer (char) using the sprintf function
    sprintf(buf, "[LN%u,SB%u,IB%d,VB%d,IS%d,D&T%.6f,LAT%.6f,LON%.6f,ALT%.6g,GYR[%.6g,%.6g,%.6g],ACCL[%.6g,%.6g,%.6g],CMP[%.6g,%.6g,%.6g],COM%u%u%u%u%u%u%u%u]\n",data.logNum,data.status,data.powerData[0],data.powerData[1],data.powerData[2],data.gpsData[0],data.gpsData[1],data.gpsData[2],data.gpsData[3],data.imuData[0],data.imuData[1],data.imuData[2],data.imuData[3],data.imuData[4],data.imuData[5],data.imuData[6],data.imuData[7],data.imuData[8],data.commandData[0],data.commandData[1],data.commandData[2],data.commandData[3],data.commandData[4],data.commandData[5],data.commandData[6],data.commandData[7]);
    String entry(buf); // Convert buf from char array to string, neccessary because the string function converts floats (which are not handled by println) to string
    entry.trim(); // Trim excess whitespace in string
    entry.toCharArray(buf, 256); // Converts string back to character array for radio transmission
    logFileHandle.println(buf); // Print to log file on the SD card
    SerialUSB.println(buf); // Print to Serial
    logFileHandle.close(); // Close file
    data.logNum++; // Increment log number since last reset
  }
  endSD(); // Ends communication with the SD card
  return fileStatus; // Returns file status
}


// Function to append data to log file from the provided buffer, useful for recording uplink commands
bool LogHandler::appendBuffer(char *buf) {
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  File logFileHandle = SD.open(LOGNAME, FILE_WRITE); // Open file for writing
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    logFileHandle.println(buf); // Print to log file on the SD card
    SerialUSB.println(buf); // Print to Serial
    logFileHandle.close(); // Close file
  }
  endSD(); // Ends communication with the SD card
  return fileStatus; // Returns file status
}


// Read last N lines in the log file
bool LogHandler::read(uint16_t numLines, char *buf) {
  int32_t i = 0; // Initialize a file position index (indexed by byte)
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  uint16_t numNewLines = 0; // Counter that keeps track of how many lines have been found while reading a file in-reverse from the end-of-file (a line defined as anything between new-line characters)
  startSD(); // Starts communication with the SD card
  File logFileHandle = SD.open(LOGNAME); // Opens a file for reading
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    for (i = logFileHandle.size(); i >= logFileHandle.size() - MAXCHARS && i >= 0; i--) { // Read the file in reverse starting at the end of the file and iterate backwards one character at a time
      logFileHandle.seek(i); // Move our read pointer to that character
      if (logFileHandle.read() == '\n') { // Read that character, if it's a new-line character
      numNewLines++; // Iterate the counter for number of new-lines found
    }
    if (numLines + 1 == numNewLines) { // If we reach the dersired the lines found, exit (the +1 is required because the counter starts at zero)
      break; // Exit if loop
    }
  }
  if (i < 0) { // If we reached the beginning of the file in our search
    logFileHandle.seek(0); // Gets back one character that may be missed because of previous read
  }
  i = 0; // Redefine/reset file read pointer for our next operations
  while (logFileHandle.available() && i < MAXCHARS) { // Now reading forward (instead of backward like before), for every character available until the end-of-file
    buf[i] = logFileHandle.read(); // Read the character in file and save it to the buffer
    i++; // Iterate the file read position pointer
  }
  buf[i] = 0; // Add null character to end of message to represent the end of the message
  if (i > 0) { // If the length of the message is not zero (it would only equal zero if the file could not be opened)
    buf[i-1] = 0; // Remove the end-of-line character from the end of the message
  }
  SerialUSB.println(buf); // Print buffer to Serial
  logFileHandle.close(); // Close file
}
endSD(); // Ends communication with the SD card

return fileStatus; // Returns file status
}


/* This chunk of code is an alternate approach to searchin/reading in the file
// // Read last N lines in the log file
// bool LogHandler::read(uint16_t numLines) {
//   uint32_t i = 0;
//   bool fileStatus = false;
//   char currentChar;
//   char buf[10000];
//   startSD(); // Starts communication with the SD card
//   File logFileHandle = SD.open(LOGNAME, FILE_WRITE);
//   if (logFileHandle) {
//     fileStatus = true;
//     if (logFileHandle.seek(logFileHandle.size() - numLines*171 - 150) == false) {
//       logFileHandle.seek(0);
//       while (logFileHandle.available() && i < MAXCHARS) {
//         buf[i] = logFileHandle.read();
//         i++;
//       }
//       buf[i] = 0;
//       SerialUSB.println(buf);
//       return true;
//     }
//
//     while (logFileHandle.read() != '\n') {
//       if (!logFileHandle.available()) {
//         return false;
//       }
//     }
//
//     while (logFileHandle.available() && i < MAXCHARS) {
//       buf[i] = logFileHandle.read();
//       i++;
//     }
//     buf[i] = 0;
//
//     SerialUSB.println(buf);
//
//     logFileHandle.close(); // Close file
//   }
//   endSD(); // Ends communication with the SD card
//   return fileStatus;
// }
*/


// Sets all data to zero in the Data structure
void LogHandler::resetLogStruct() {
  data.logNum = 0;
  data.status = 0;
  for(uint32_t i = 0; i < 3; i++) {
    data.powerData[i] = 0;
  }
  for(uint32_t i = 0; i < 4; i++) {
    data.gpsData[i] = 0;
  }
  for(uint32_t i = 0; i < 9; i++) {
    data.imuData[i] = 0;
  }
  for(uint32_t i = 0; i < 8; i++) {
    data.commandData[i] = 0;
  }
}


// Function to compile and return health data for beacon
bool LogHandler::compileHealth(char *healthData) {
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  File logFileHandle = SD.open(LOGNAME, FILE_WRITE); // Open file for writing
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    char buf[256]; // Creates empty array that will be filled and then written to SD, max size of 256 is more than enough (avg size is 180 bytes)
    // Writes data structure to buffer (char) using the sprintf function
    sprintf(buf, "[LN%u,SB%u,IB%d,VB%d,IS%d]\n",data.logNum,data.status,data.powerData[0],data.powerData[1],data.powerData[2]);
    String entry(buf); // Convert buf from char array to string, neccessary because the string function converts floats (which are not handled by println) to string
    entry.trim(); // Trim excess whitespace in string
    entry.toCharArray(healthData, entry.length()+1); // Converts string back to character array for radio transmission
    SerialUSB.println(healthData); // Print to Serial
    logFileHandle.close(); // Close file
  }
  endSD(); // Ends communication with the SD card
  return fileStatus; // Returns file status
}


LogHandler logfile; // Creates an external, global logfile object
