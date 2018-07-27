// sd_log.h - Library for reading from and writing to the log on the SD card

#ifndef SD_LOG_h
#define SD_LOG_h

#include "Arduino.h"
#include "SD_DataFile.h"

class sd_log{
  public:

    struct Log_Data{
      int log_num;
      byte status_byte;
      byte power_data[3];
      float GPS_data[3];
      float IMU_data[9];
      byte command_data[8];
    }logData;

    sd_log();
    void write_log(Log_Data data); //takes all data and logs it
    String read_entry(int entryIndex);  //reads a specific entry indexed from current entry
    char* data_dump(int numEntries); //reads the last numEntries logs and outputs in string array

    //member variables
    SD_DataFile dataLog = SD_DataFile(126, "L");

  private:
    String encode_int(int value);
    String encode_float(float value);
    void add_String_Entry(String input, int entry_num, byte array1[]);
    float decode_float(byte* data);
    int decode_int(byte* data);
    String log_to_String();
};

#endif
