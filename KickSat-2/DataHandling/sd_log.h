// sd_log.h - Library for reading from and writing to the log on the SD card

#ifndef SD_LOG_h
#define SD_LOG_h

#include "Arduino.h"
#include "SD_DataFile.h"

#define ENTRY_LEN 126
#define GPS_LEN 30
#define GPS_INDEX 22
#define LOG_NUM_INDEX 3
#define LOG_NUM_LEN 3
#define STATUS_BYTE_INDEX 9
#define I_BATT_INDEX 13
#define V_BATT_INDEX 17
#define I_SOLAR_INDEX 21
#define DT_INDEX 26
#define LAT_INDEX 34
#define LON_INDEX 42
#define ALT_INDEX 50
#define GYR_X_INDEX 59
#define GYR_Y_INDEX 64
#define GYR_Z_INDEX 69
#define ACC_X_INDEX 79
#define ACC_Y_INDEX 84
#define ACC_Z_INDEX 89
#define CMP_X_INDEX 99
#define CMP_Y_INDEX 104
#define CMP_Z_INDEX 109
#define COM_INDEX 118
#define FLOAT_LEN 4
#define COM_LEN 8
#define BYTE_LEN 1
#define NUM_DEC_IN_FLOAT 6



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
    bool data_dump(int startEntry, int numEntries, String buf[]); //reads the last numEntries logs and outputs in string array
    String read_GPS(int entryIndex);
    String read_IMU(int entryIndex);
    char* read_header(int entryIndex);

    //member variables
    SD_DataFile dataLog = SD_DataFile(ENTRY_LEN, "Loggo");

  private:
    String encode_int(int value);
    String encode_float(float value);
    void add_String_Entry(String input, int entry_num, byte array1[]);
    float decode_float(byte* data);
    int decode_int(byte* data);
    String log_to_String();
    float read_float(byte buf[], int index);
};

#endif
