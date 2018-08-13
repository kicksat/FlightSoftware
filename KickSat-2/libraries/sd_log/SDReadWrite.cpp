/**
Read and write lib for SD cards, SDReadWrite.cpp
Purpose: Library for reading from and writing to the log on the SD cards

@author Emma Morgan
@version 1.0 08/10/18
*/

/*
Last update on: 8-12-18
by: Ralen
*/


#include "SDReadWrite.h"
#include <SD.h>


// Constructor
SDReadWriteHandler::SDReadWriteHandler(uint16_t dataWidth, String fileName) {
  _dataWidth = dataWidth; // Length of each line in the file
  _fileName = fileName; // File name
  _numEntries = 0; // Number of lines in the file
}


// Writes a new entry/line to the file
bool SDReadWriteHandler::writeDataEntry(uint8_t* data) {
  _fileHandle = SD.open(_fileName, FILE_WRITE); // Open file on SD card in write mode and save status
  if (_fileHandle) { // If file opened
    _fileHandle.write(data, _dataWidth); // Write data to file
    _fileHandle.close(); // Close file
    _numEntries++; // Increment number of entries, lines in the file
    return true; // Return that the file could be opened
  }
  return false; // Return that the file could not be opened
}


// Reads the data file entry specified by the index into the buffer
bool SDReadWriteHandler::readDataEntry(uint32_t index, uint8_t* buf) {
  if (index < _numEntries) { // Checks that index is valid
    _fileHandle = SD.open(_fileName); // Open file on SD card in read mode and save status
    if (_fileHandle) { // If file opened
      _fileHandle.seek(index * _dataWidth); // Navigate to requested line
      _fileHandle.read(buf, _dataWidth); // Read requested line
      _fileHandle.close(); // Close file
      return true; // Return that the file could be opened
    }
  }
  return false; // Return that the file could not be opened
}

// Reads the data file entry specified by the line and index into the buffer
bool SDReadWriteHandler::readLineIndex(uint32_t lineNum, uint32_t index, uint32_t len, uint8_t* buf){
  if (lineNum < _numEntries && index < _numEntries) { // Checks that line and index are valid
    _fileHandle = SD.open(_fileName); // Open file on SD card in read mode and save status
    if (_fileHandle) { // If file opened
      _fileHandle.seek((lineNum * _dataWidth) + index); // Navigate to requested line and index
      _fileHandle.read(buf, len); // Read requested line
      _fileHandle.close(); // Close file
      return true; // Return that the file could be opened
    }
  }
  return false; // Return that the file could not be opened
}

// Refreshes the numEntries variable, representing the number of lines in the file
bool SDReadWriteHandler::refresh() {
  _fileHandle = SD.open(_fileName); // Open file on SD card in read mode and save status
  if(_fileHandle){ // If file opened
    _numEntries = _fileHandle.size() / _dataWidth; // Calculate and record the number of lines in the file
    _fileHandle.close(); // Close SD file
    return true; // Return that the file could be opened
  }
  return false; // Return that the file could not be opened
}
