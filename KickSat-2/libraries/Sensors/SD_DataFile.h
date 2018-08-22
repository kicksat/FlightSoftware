/// sd_log.h - Library for reading from and writing to the log on the SD card

#ifndef SD_DATAFILE_h
#define SD_DATAFILE_h

#include <SD.h>

class SD_DataFile
{
  public:
    SD_DataFile(int cs_pin, uint16_t dw, String fn);
    bool writeDataEntry(byte* data);
    bool readDataEntry(int index, byte* buf);
    bool readLineIndex(int lineNum, int index, int len, byte* buf);
    bool refresh();

    // member variables
    uint16_t _dataWidth;
    uint16_t _numEntries;
    String _fileName;
    File _dataFile;
    int _chipSelectPin;
};

#endif 
