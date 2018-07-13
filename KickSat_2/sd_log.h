//sd_log.h - Library for reading from and writing to the log on the SD card

#ifndef SD_LOG_h
#define SD_LOG_h

#include "Arduino.h"
#include "spi_io.h"



  class sd_log {
    public:
      void writeLog(); //takes all data and logs it
      //void writeLogWithSensor(bool sensor1, bool sensor2, bool sensor3, bool sensor4, bool sensor5); //allows specific sensors to be set on or off
      string readEntry(int entryIndex);  //reads a specific entry indexed from current entry
      string[] dataDump(int numEntries); //reads the last numEntries logs and outputs in string array
      
      /* Functions to update flag status */ 
      
      void setAntenna(bool flag); 
      void setBW1(bool flag); 
      void setBW2(bool flag);
      void setBW3(bool flag);
      void setModeEarly(bool flag);
      void setModeStandby(bool flag);
      void setModeArming(bool flag);
      void flagChanged(bool flag); //error checking must be set same time stamp that flag is changed
    
  };

#endif
