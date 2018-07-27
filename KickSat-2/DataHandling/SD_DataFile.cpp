//SD_DataFile.cpp

#include "SD_DataFile.h"
#include <SD.h>

//constructor, sets up file type and name
SD_DataFile::SD_DataFile(uint16_t dw, String fn) {
  _dataWidth = dw;
  _fileName = fn;
  _numEntries = 0;
}

//refreshes the numEntries variable
//TODO: do a better job of initializing the DataFile object so this isn't necessary
void SD_DataFile::refresh() {
  _dataFile = SD.open(_fileName);
  _numEntries = _dataFile.size() / _dataWidth;
  _dataFile.close();
}

//writes from the passed array into the next data entry slot on the data file
//make sure the passed array is the right size
//returns true if success, false otherwise
bool SD_DataFile::writeDataEntry(byte* data) {
  _dataFile = SD.open(_fileName, FILE_WRITE);
  if (_dataFile) {
    _dataFile.write(data, _dataWidth);
    _dataFile.close();
    _numEntries++;
    return true;
  }
  return false;
}

//reads the data file entry specified by the int index into the buffer buf
//returns true if success, false otherwise
bool SD_DataFile::readDataEntry(int index, byte* buf) {
  if (index < _numEntries) {
    _dataFile = SD.open(_fileName, FILE_READ);
    if (_dataFile) {
      _dataFile.seek(index * _dataWidth);
      _dataFile.read(buf, _dataWidth);
      _dataFile.close();
      return true;
    }
  }
  return false;
}

bool SD_DataFile::readLineIndex(int lineNum, int index, int len, byte* buf){
  if (index < _numEntries) {
    _dataFile = SD.open(_fileName, FILE_READ);
    if (_dataFile) {
      _dataFile.seek(lineNum * _dataWidth + index);
      _dataFile.read(buf, len);
      _dataFile.close();
      return true;
    }
  }
  return false;
}

