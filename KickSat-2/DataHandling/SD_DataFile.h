// SD_DataFile.h - Library for storing and reading structured sensor/log data

#ifndef SD_DATAFILE_h
#define SD_DATAFILE_h

//#include "Arduino.h"
#include <integer.h>
#include <sd_io.h>
#include <spi_io.h>
#include <SD.h>


class SD_DataFile 
{
  public:
    SD_DataFile(uint16_t dw);
    bool writeDataEntry(byte* data);
    bool readDataEntry();

    // member variables
    uint16_t _dataWidth;
};

#endif
