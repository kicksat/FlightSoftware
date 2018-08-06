#ifndef SD_SENSOR_LOG_h
#define SD_SENSOR_LOG_h

#include "Arduino.h"
#include "SD_DataFile.h"

#define DATA_LEN 3


class sd_sensor_log{
  public:
    sd_sensor_log();
    bool write_sensor(byte data[], int sensor_num); //writes 3 byte data
    bool write_delimeter(int sensor_num);         //writes 3 byte delimeter (3 0xff)
    bool read_sensor(byte buf[], int sensor_num, int line_num); //reads single 3 byte line
    bool data_dump_sensor(byte* data[], int sensor_num, int start_line, int num_entries); //reads many lines, converts delimeters to just one byte (1 0xff)
    
    byte delimeter[DATA_LEN] = {0xff, 0xff, 0xff};
   
};

#endif
