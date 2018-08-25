/// sd_log.h - Library for reading from and writing to the log on the SD card

#ifndef SD_DATAFILE_h
#define SD_DATAFILE_h
#include <Arduino.h>
#include <SdFat.h>

class SD_DataFile
{
  public:
    SD_DataFile(int cs_pin, uint16_t dw, String fn, SdFat _sd);
    bool refresh();//basically an init
    int size();//returns number of entries
    bool writeDataEntry(byte* data);
    bool readDataEntry(int index, byte* buf);
    bool readLineIndex(int lineNum, int index, int len, byte* buf);

  private:
    // member variables
    uint16_t _dataWidth;
    uint16_t _numEntries;
    String _fileName;
    File _dataFile;
    int _chipSelectPin;
    SdFat SD;
};

#endif
