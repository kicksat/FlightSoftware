//sd_log.cpp


#include "sd_log.h"


sd_log :: sd_log(){
  dataLog.refresh();
}


void sd_log :: write_log(Log_Data data){
  byte output[ENTRY_LEN];
  add_String_Entry("[LN", 0, output);
  add_String_Entry(encode_int(data.log_num), LOG_NUM_INDEX, output);
  add_String_Entry(",SB", LOG_NUM_INDEX +LOG_NUM_LEN, output);
  output[STATUS_BYTE_INDEX ] = data.status_byte;
  add_String_Entry(",IB", STATUS_BYTE_INDEX  + BYTE_LEN, output);
  output[I_BATT_INDEX ] = data.power_data[0];
  add_String_Entry(",VB", I_BATT_INDEX  + BYTE_LEN, output);
  output[V_BATT_INDEX ] = data.power_data[1];
  add_String_Entry(",IS", V_BATT_INDEX + BYTE_LEN, output);
  output[I_SOLAR_INDEX] = data.power_data[2];
  add_String_Entry(",D&T", I_SOLAR_INDEX + BYTE_LEN , output);
  add_String_Entry(encode_float(data.GPS_data[0]), DT_INDEX, output);
  add_String_Entry(",LAT", DT_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.GPS_data[1]), LAT_INDEX, output);
  add_String_Entry(",LON", LAT_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.GPS_data[2]), LON_INDEX, output);
  add_String_Entry(",ALT", LON_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.GPS_data[3]), ALT_INDEX, output);
  add_String_Entry(",GYR[", ALT_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.IMU_data[0]), GYR_X_INDEX, output);
  output[GYR_X_INDEX + FLOAT_LEN] = ',';
  add_String_Entry(encode_float(data.IMU_data[1]), GYR_Y_INDEX, output);
  add_String_Entry(",", GYR_Y_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.IMU_data[2]), GYR_Z_INDEX, output);
  add_String_Entry("],ACC[", GYR_Z_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.IMU_data[3]), ACC_X_INDEX, output);
  add_String_Entry(",", ACC_X_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.IMU_data[4]), ACC_Y_INDEX, output);
  add_String_Entry(",", ACC_Y_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.IMU_data[5]), ACC_Z_INDEX, output);
  add_String_Entry("],CMP[", ACC_Z_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.IMU_data[6]), CMP_X_INDEX, output);
  add_String_Entry(",", CMP_X_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.IMU_data[7]), CMP_Y_INDEX, output);
  add_String_Entry(",", CMP_Y_INDEX + FLOAT_LEN, output);
  add_String_Entry(encode_float(data.IMU_data[8]), CMP_Z_INDEX, output);
  add_String_Entry("],COM", CMP_Z_INDEX + FLOAT_LEN, output);
  for(int i = 0; i < COM_LEN; i++){
    output[COM_INDEX+i] = data.command_data[i];
  }
  output[COM_INDEX + COM_LEN] = ']';

  dataLog.writeDataEntry((byte*)output);
 // dataLog.writeDataEntry((byte*)output);

  SerialUSB.println("log entry");
  for(int i = 0; i < ENTRY_LEN; i++){
    SerialUSB.print((char)output[i]);
  }
  SerialUSB.println(" ");
}

String sd_log :: read_entry(int entryIndex){
  byte buf[ENTRY_LEN];
  String output = "";
  dataLog.readDataEntry(entryIndex, buf);
  byte lNum[LOG_NUM_LEN] = {buf[LOG_NUM_INDEX], buf[LOG_NUM_INDEX + 1], buf[LOG_NUM_INDEX + 2]};
  logData.log_num = decode_int(lNum);
  logData.status_byte = buf[STATUS_BYTE_INDEX];
  logData.power_data[0] = buf[I_BATT_INDEX];
  logData.power_data[1] = buf[V_BATT_INDEX];
  logData.power_data[2] = buf[I_SOLAR_INDEX];
  logData.GPS_data[0] = read_float(buf, DT_INDEX);
  logData.GPS_data[1] = read_float(buf, LAT_INDEX);
  logData.GPS_data[2] = read_float(buf, LON_INDEX);
  logData.GPS_data[3] = read_float(buf, ALT_INDEX);
  logData.IMU_data[0] = read_float(buf, GYR_X_INDEX);
  logData.IMU_data[1] = read_float(buf, GYR_Y_INDEX);
  logData.IMU_data[2] = read_float(buf, GYR_Z_INDEX);
  logData.IMU_data[3] = read_float(buf, ACC_X_INDEX);
  logData.IMU_data[4] = read_float(buf, ACC_Y_INDEX);
  logData.IMU_data[5] = read_float(buf, ACC_Z_INDEX);
  logData.IMU_data[6] = read_float(buf, CMP_X_INDEX);
  logData.IMU_data[7] = read_float(buf, CMP_Y_INDEX);
  logData.IMU_data[8] = read_float(buf, CMP_Z_INDEX);
  for(int i = 0; i < COM_LEN; i++){
    logData.command_data[i] = buf[COM_INDEX + i];
  }

  SerialUSB.println("read result:");
  for(int i = 0; i < ENTRY_LEN; i++){
    SerialUSB.print((char)buf[i]);
  }
  SerialUSB.println(" ");
  String converted = log_to_String();
  SerialUSB.println("converted ");
  SerialUSB.println(converted);
  return converted;
}

float sd_log :: read_float(byte buf[], int index){
  byte floatInput[FLOAT_LEN] = {buf[index], buf[index + 1], buf[index + 2], buf[index + 2]};
  float output = decode_float(floatInput);
  return output;
}

bool sd_log :: data_dump(int startEntry, int numEntries, String buf[]){
  for(int i = 0; i < numEntries; i++){
    buf[i] = read_entry(i + startEntry);
  }
 return true; //make error case
}

String sd_log::read_GPS(int entryIndex){
  String output = "";
  byte dt[FLOAT_LEN];
  byte lat[FLOAT_LEN];
  byte lon[FLOAT_LEN];
  byte alt[FLOAT_LEN];
  dataLog.readLineIndex(entryIndex, DT_INDEX, FLOAT_LEN, dt);
  dataLog.readLineIndex(entryIndex, LAT_INDEX, FLOAT_LEN, lat);
  dataLog.readLineIndex(entryIndex, LON_INDEX, FLOAT_LEN, lon);
  dataLog.readLineIndex(entryIndex, ALT_INDEX, FLOAT_LEN, alt);
  logData.GPS_data[0] = decode_float(dt);
  logData.GPS_data[1] = decode_float(lat);
  logData.GPS_data[2] = decode_float(lon);
  logData.GPS_data[3] = decode_float(alt);
  output += "D&T";
  output += String(logData.GPS_data[0], NUM_DEC_IN_FLOAT);
  output += ",LAT";
  output += String(logData.GPS_data[1], NUM_DEC_IN_FLOAT);
  output += ",LON";
  output += String(logData.GPS_data[2], NUM_DEC_IN_FLOAT);
  output += ",ALT";
  output += String(logData.GPS_data[3], NUM_DEC_IN_FLOAT);

  SerialUSB.print("GPS data line:");
  SerialUSB.println(entryIndex);
  SerialUSB.println(output);

  return output;

}

String sd_log::read_IMU(int entryIndex){

}

char* sd_log::read_header(int entryIndex){

}

void sd_log :: add_String_Entry(String input, int entry_num, byte array1[]){
  for(int i = 0; i < input.length(); i++){
    array1[entry_num + i] = input.charAt(i);
  }
}

String sd_log :: log_to_String(){
  String output = "";
  output+= "[LN";
  output+= logData.log_num;
  output+= ",SB";
  output+= (char)logData.status_byte;
  output += ",IB";
  output+= (char)logData.power_data[0];
  output+= ",VB";
  output += (char)logData.power_data[1];
  output+= ",IS";
  output+= (char)logData.power_data[2];
  output+= ",D&T";
  output += String(logData.GPS_data[0], NUM_DEC_IN_FLOAT);
  output += ",LAT";
  output+= String(logData.GPS_data[1], NUM_DEC_IN_FLOAT);
  output+= ",LON";
  output += String(logData.GPS_data[2], NUM_DEC_IN_FLOAT);
  output += ",ALT";
  output+= String(logData.GPS_data[3], NUM_DEC_IN_FLOAT);
  output+= ",GYR[";
  output += String(logData.IMU_data[0], NUM_DEC_IN_FLOAT);
  output += ",";
  output += String(logData.IMU_data[1], NUM_DEC_IN_FLOAT);
  output += ",";
  output += String(logData.IMU_data[2], NUM_DEC_IN_FLOAT);
  output +=  "],ACC[";
  output+= String(logData.IMU_data[3], NUM_DEC_IN_FLOAT);
  output+= ",";
  output+= String(logData.IMU_data[4], NUM_DEC_IN_FLOAT);
  output+= ",";
  output+= String(logData.IMU_data[5], NUM_DEC_IN_FLOAT);
  output+= "],CMP[";
  output+= String(logData.IMU_data[6], NUM_DEC_IN_FLOAT);
  output+= ",";
  output+= String(logData.IMU_data[7], NUM_DEC_IN_FLOAT);
  output+=  ",";
  output+= String(logData.IMU_data[8], NUM_DEC_IN_FLOAT);
  output+=  "],COM";
  output+= (char)logData.command_data[0];
  output+= (char)logData.command_data[1];
  output+= (char)logData.command_data[2];
  output+= (char)logData.command_data[3];
  output+= (char)logData.command_data[4];
  output+= (char)logData.command_data[5];
  output+= (char)logData.command_data[6];
  output+= (char)logData.command_data[7];
  output+= "]";
  return output;
}

//====INTS
typedef union {
   int val;
   uint8_t bytes[2];
} intunion_t;

String sd_log :: encode_int(int value) {
  intunion_t n;
  n.val = value;
  String result = "";

  for (int i = 1; i >= 0; i--) {
    result += (char)n.bytes[i];
  }

  return result;
}


//====FLOATS
typedef union {
   float val;
   uint8_t bytes[4];
} floatunion_t;

String sd_log :: encode_float(float value) {
  floatunion_t flt;
  flt.val = value;
  String result = "";

  for (int i = 3; i >= 0; i--) {
    result += (char)flt.bytes[i];
  }

  return result;
}

float sd_log :: decode_float(byte* data) {
  floatunion_t flt;
  for (int i = 0; i < 4; i++) {
    flt.bytes[3 - i] = data[i];
  }
  return flt.val;
}

int sd_log :: decode_int(byte* data) {
  intunion_t n;
  for (int i = 0; i < 2; i++) {
    n.bytes[1 - i] = data[i];
  }
  return n.val;
}
