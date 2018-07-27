//sd_log.cpp

#include "sd_log.h"

sd_log :: sd_log(){
  dataLog.refresh();
}


void sd_log :: write_log(Log_Data data){
  byte output[147];
  add_String_Entry("[LN", 0, output);
  add_String_Entry(encode_int(data.log_num), 3, output);
  add_String_Entry(",SB", 5, output);
  output[8] = data.status_byte;
  add_String_Entry(",IB", 9, output);
  output[12] = data.power_data[0];
  add_String_Entry(",VB", 13, output);
  output[16] = data.power_data[1];
  add_String_Entry(",IS", 17, output);
  output[20] = data.power_data[2];
  add_String_Entry(",D&T", 21, output);
  add_String_Entry(encode_float(data.GPS_data[0]), 25, output);
  add_String_Entry(",LAT", 29, output);
  add_String_Entry(encode_float(data.GPS_data[1]), 33, output);
  add_String_Entry(",LON", 37, output);
  add_String_Entry(encode_float(data.GPS_data[2]), 41, output);
  add_String_Entry(",ALT", 45, output);
  add_String_Entry(encode_float(data.GPS_data[3]), 49, output);
  add_String_Entry(",GYR[", 53, output);
  add_String_Entry(encode_float(data.IMU_data[0]), 58, output);
  output[62] = ',';
  add_String_Entry(encode_float(data.IMU_data[1]), 63, output);
  add_String_Entry(",", 67, output);
  add_String_Entry(encode_float(data.IMU_data[2]), 68, output);
  add_String_Entry("],ACC[", 72, output);
  add_String_Entry(encode_float(data.IMU_data[3]), 78, output);
  add_String_Entry(",", 82, output);
  add_String_Entry(encode_float(data.IMU_data[4]), 83, output);
  add_String_Entry(",", 87, output);
  add_String_Entry(encode_float(data.IMU_data[5]), 88, output);
  add_String_Entry("],CMP[", 92, output);
  add_String_Entry(encode_float(data.IMU_data[6]), 98, output);
  add_String_Entry(",", 102, output);
  add_String_Entry(encode_float(data.IMU_data[7]), 103, output);
  add_String_Entry(",", 107, output);
  add_String_Entry(encode_float(data.IMU_data[8]), 108, output);
  add_String_Entry("],COM", 112, output);
  output[117] = data.command_data[0];
  output[118] = data.command_data[1];
  output[119] = data.command_data[2];
  output[120] = data.command_data[3];
  output[121] = data.command_data[4];
  output[122] = data.command_data[5];
  output[123] = data.command_data[6];
  output[124] = data.command_data[7];
  output[125] = ']';

  dataLog.writeDataEntry((byte*)output);
 // dataLog.writeDataEntry((byte*)output);

  SerialUSB.println("log entry");
  for(int i = 0; i < 147; i++){
    SerialUSB.print((char)output[i]);
  }
  SerialUSB.println(" ");
}

String sd_log :: read_entry(int entryIndex){
  byte buf[126];
  String output = "";
  dataLog.readDataEntry(entryIndex, buf);
  byte lNum[2] = {buf[3], buf[4]};
  logData.log_num = decode_int(lNum);
  logData.status_byte = buf[8];
  logData.power_data[0] = buf[12];
  logData.power_data[1] = buf[16];
  logData.power_data[2] = buf[20];
  byte dt[4] = {buf[25], buf[26], buf[27], buf[28]};
  logData.GPS_data[0] = decode_float(dt);
  byte lat[4] = {buf[33], buf[34], buf[35], buf[36]};
  logData.GPS_data[1] = decode_float(lat);
  byte lon[4] = {buf[41], buf[42], buf[43], buf[44]};
  logData.GPS_data[2] = decode_float(lon);
  byte alt[4] = {buf[49], buf[50], buf[51], buf[52]};
  logData.GPS_data[3] = decode_float(alt);
  byte gyrX[4] = {buf[58], buf[59], buf[60], buf[61]};
  logData.IMU_data[0] = decode_float(gyrX);
  byte gyrY[4] = {buf[63], buf[64], buf[65], buf[66]};  
  logData.IMU_data[1] = decode_float(gyrY);
  byte gyrZ[4] = {buf[68], buf[69], buf[70], buf[71]};
  logData.IMU_data[2] = decode_float(gyrZ);
  byte accX[4] = {buf[78], buf[79], buf[80], buf[81]};
  logData.IMU_data[3] = decode_float(accX);
  byte accY[4] = {buf[83], buf[84], buf[85], buf[86]};
  logData.IMU_data[4] = decode_float(accY);
  byte accZ[4] = {buf[88], buf[89], buf[90], buf[91]};
  logData.IMU_data[5] = decode_float(accZ);
  byte cmpX[4] = {buf[98], buf[99], buf[100], buf[101]};
  logData.IMU_data[6] = decode_float(cmpX); 
  byte cmpY[4] = {buf[103], buf[104], buf[105], buf[106]};
  logData.IMU_data[7] = decode_float(cmpY);
  byte cmpZ[4] = {buf[108], buf[109], buf[110], buf[111]};
  logData.IMU_data[8] = decode_float(cmpZ);
  logData.command_data[0] = buf[117];
  logData.command_data[1] = buf[118];
  logData.command_data[2] = buf[119];
  logData.command_data[3] = buf[120];
  logData.command_data[4] = buf[121];
  logData.command_data[5] = buf[122];
  logData.command_data[6] = buf[123];
  logData.command_data[7] = buf[124];

  SerialUSB.println("read result:");
  for(int i = 0; i < 147; i++){
    SerialUSB.print((char)buf[i]);
  }
  SerialUSB.println(" ");
  String converted = log_to_String();
  SerialUSB.println("converted ");
  SerialUSB.println(converted);
  return converted;
}

char* sd_log :: data_dump(int numEntries){

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
  output += logData.GPS_data[0];
  output += ",LAT";
  output+= logData.GPS_data[1];
  output+= ",LON";
  output += String(logData.GPS_data[2], 6);
  output += ",ALT";
  output+= String(logData.GPS_data[3], 6);
  output+= ",GYR[";
  output += String(logData.IMU_data[0], 6);
  output += ",";
  output += String(logData.IMU_data[1], 6);
  output += ",";
  output += String(logData.IMU_data[2], 6);
  output +=  "],ACC[";
  output+= String(logData.IMU_data[3], 6);
  output+= ",";
  output+= String(logData.IMU_data[4], 6);
  output+= ",";
  output+= String(logData.IMU_data[5], 6);
  output+= "],CMP[";
  output+= String(logData.IMU_data[6], 6);
  output+= ",";
  output+= String(logData.IMU_data[7], 6);
  output+=  ",";
  output+= String(logData.IMU_data[8], 6);
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




