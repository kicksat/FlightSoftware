//SD_DataFile.cpp

#include "SD_DataFile.h"
#include <SDFat.h>

#define Serial SerialUSB

//constructor, sets up file type and name
SD_DataFile::SD_DataFile(int cs_pin, uint16_t dw, String fn, SdFat _sd) {
  _chipSelectPin = cs_pin;
  _dataWidth = dw;
  _fileName = fn;
  _numEntries = 0;
  SD = _sd;
  
  pinMode(_chipSelectPin, OUTPUT);
  digitalWrite(_chipSelectPin, HIGH);
}

//refreshes the numEntries variable
//TODO: do a better job of initializing the DataFile object so this isn't necessary
bool SD_DataFile::refresh() {
  SD.begin(_chipSelectPin);
  digitalWrite(_chipSelectPin, LOW);
  _dataFile = SD.open(_fileName);
  if(_dataFile){
    _numEntries = _dataFile.size() / _dataWidth;
    _dataFile.close();
    digitalWrite(_chipSelectPin, HIGH);
    return true;
  }
  digitalWrite(_chipSelectPin, HIGH);
  return false;
}

//returns the number of entries currently detected in the file
//NOTE: if refresh() has not been called, this will be incorrect
int SD_DataFile::size() {
  return _numEntries;
}

//writes from the passed array into the next data entry slot on the data file
//make sure the passed array is the right size
//returns true if success, false otherwise
bool SD_DataFile::writeDataEntry(byte* data) {
  SD.begin(_chipSelectPin);
  digitalWrite(_chipSelectPin, LOW);
  _dataFile = SD.open(_fileName, FILE_WRITE);
  if (_dataFile) {
    _dataFile.write(data, _dataWidth);
    _dataFile.close();
    _numEntries++;
    digitalWrite(_chipSelectPin, HIGH);
    return true;
  }
  digitalWrite(_chipSelectPin, HIGH);
  return false;
}

//reads the data file entry specified by the int index into the buffer buf
//returns true if success, false otherwise
bool SD_DataFile::readDataEntry(int index, byte* buf) {
  SD.begin(_chipSelectPin);
  digitalWrite(_chipSelectPin, LOW);
  if (index < _numEntries) {
    _dataFile = SD.open(_fileName, FILE_READ);
    if (_dataFile) {
      _dataFile.seek(index * _dataWidth);
      _dataFile.read(buf, _dataWidth);
      _dataFile.close();
      digitalWrite(_chipSelectPin, HIGH);
      return true;
    }
  }
  digitalWrite(_chipSelectPin, HIGH);
  return false;
}

//reads a single data entry from a line containing multiple entries
//reads an entry of length len from line lineNum starting at the specified index
//the data is stored in buf
bool SD_DataFile::readLineIndex(int lineNum, int index, int len, byte* buf){
  if (lineNum < _numEntries) {
    SD.begin(_chipSelectPin);
    digitalWrite(_chipSelectPin, LOW);
    _dataFile = SD.open(_fileName, FILE_READ);
    if (_dataFile) {
      _dataFile.seek((lineNum * _dataWidth) + index);
      _dataFile.read(buf, len);
      _dataFile.close();
      digitalWrite(_chipSelectPin, HIGH);
      return true;
    }
    digitalWrite(_chipSelectPin, HIGH);
  }
  return false;
}
