// sd_log.h - Library for reading from and writing to the log on the SD card

#ifndef SD_LOG_h
#define SD_LOG_h

#include "Arduino.h"
#include "SD_DataFile.h"

#define CS SPI_CS_SD

#define ENTRY_LEN 126
#define LOG_NUM_INDEX 3
#define STATUS_BYTE_INDEX 8 + 20
#define I_BATT_INDEX 12 + 20
#define V_BATT_INDEX 16 + 20
#define I_SOLAR_INDEX 20 + 20
#define DT_INDEX 25
#define LAT_INDEX 33
#define LON_INDEX 41
#define ALT_INDEX 49
#define GYR_X_INDEX 58
#define GYR_Y_INDEX 63
#define GYR_Z_INDEX 68
#define ACC_X_INDEX 78
#define ACC_Y_INDEX 83
#define ACC_Z_INDEX 88
#define CMP_X_INDEX 98
#define CMP_Y_INDEX 103
#define CMP_Z_INDEX 108
#define COM_INDEX 117

#define FLOAT_LEN 4
#define INT_LEN 2
#define COM_LEN 8
#define BYTE_LEN 1
#define NUM_DEC_IN_FLOAT 6

#define LOG_NAME "TestLog0.txt"

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
  bool sd_init();
  bool sd_end();
  void write_log(Log_Data data); //takes all data and logs it
  String read_entry(int entryIndex);  //reads a specific entry indexed from current entry
  bool data_dump(int startEntry, int numEntries, String buf[]); //reads the last numEntries logs and outputs in string array
  String read_GPS(int entryIndex);
  String read_IMU(int entryIndex);
  String read_header(int entryIndex, byte bytes[]);
  //member variables
  SD_DataFile dataLog = SD_DataFile(ENTRY_LEN, LOG_NAME);

private:
  String encode_int(int value);
  String encode_float(float value);
  void add_String_Entry(String input, int entry_num, byte array1[]);
  float decode_float(byte* data);
  int decode_int(byte* data);
  String log_to_String();
  float read_float(byte buf[], int index);
  int read_int(byte buf[], int index);
};

#endif
