// sd_log.h - Library for reading from and writing to the log on the SD card

#ifndef SD_LOG_h
#define SD_LOG_h

#include "Arduino.h"
#include "spi_io.h" // https://github.com/1nv1/ulibSD


class sd_log 
{
  public:
    void write_log(); //takes all data and logs it
    //void writeLogWithSensor(bool sensor1, bool sensor2, bool sensor3, bool sensor4, bool sensor5); //allows specific sensors to be set on or off
    string read_entry(int entryIndex);  //reads a specific entry indexed from current entry
    string[] data_dump(int numEntries); //reads the last numEntries logs and outputs in string array
    
    /* Functions to update flag status */ 
    void set_antenna_flag(bool flag); 
    void set_bw1_flag(bool flag); 
    void set_bw2_flag(bool flag);
    void set_bw3_flag(bool flag);
    void set_early_mode(bool flag);
    void set_standby_mode(bool flag);
    void set_deploying_mode(bool flag);
    void set_deployed_flag(bool flag);
    void set_varChanged_flag(bool flag); //error checking must be set same time stamp that flag is changed
    // takes in a byte of information and counts the number of ones --> returns a bool
    bool byte_to_bool(byte inBool);

    // member variables
    bool antenna_flag;
    bool bw1_flag;
    bool bw2_flag;
    bool bw3_flag;
    bool mode_isEarly;
    bool mode_isStandby;
    bool mode_isDeploying;
    bool didDeploy_flag;
    bool varChanged_flag;
};

#endif
