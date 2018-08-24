/**
Read and write lib for SD cards, SDReadWrite.h
Purpose: Library for reading from and writing to the log on the SD cards

@author Emma Morgan
@version 1.0 08/10/18
*/

/*
Last update on: 8-12-18
by: Ralen
*/

#ifndef SDREADWRITE_h
#define SDREADWRITE_h

#include <SD.h>

class SDReadWriteHandler {
  public:
    uint16_t _dataWidth;
    uint16_t _numEntries;
    String _fileName;
    File _fileHandle;

    DataFile(uint16_t dw, String fn);
    bool writeDataEntry(uint8_t* data);
    bool readDataEntry(uint32_t index, uint8_t* buf);
    bool readLineIndex(uint32_t lineNum, uint32_t index, uint32_t len, uint8_t* buf);
    bool refresh();
};

#endif
