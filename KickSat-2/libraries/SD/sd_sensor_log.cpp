 #include "sd_sensor_log.h"
 
 
 sd_sensor_log :: sd_sensor_log(){
  
 }
 
 bool sd_sensor_log :: write_sensor(byte data[], int sensor_num){
   bool result = false;
    switch(sensor_num){
      case 0: {
         SD_DataFile sensorLog0 = SD_DataFile(DATA_LEN, "S0");
         sensorLog0.refresh();
         result = sensorLog0.writeDataEntry(data);
      }break;
      case 1:{
         SD_DataFile sensorLog1 = SD_DataFile(DATA_LEN, "S1");
         sensorLog1.refresh();
         result = sensorLog1.writeDataEntry(data);
      }break;      
      case 2:{
         SD_DataFile sensorLog2 = SD_DataFile(DATA_LEN, "S2");
         sensorLog2.refresh();
         result = sensorLog2.writeDataEntry(data);
      }break;     
      case 3:{
         SD_DataFile sensorLog3 = SD_DataFile(DATA_LEN, "S3");
         sensorLog3.refresh();
         result = sensorLog3.writeDataEntry(data);
      }break;     
      case 4:{
         SD_DataFile sensorLog4 = SD_DataFile(DATA_LEN, "S4");
         sensorLog4.refresh();
         result = sensorLog4.writeDataEntry(data);
      }break;        
    }
    return result;
  
 }


 
 bool sd_sensor_log :: read_sensor(byte buf[], int sensor_num, int line_num){
  bool result = false;
    switch(sensor_num){
      case 0: {
         SD_DataFile sensorLog0 = SD_DataFile(DATA_LEN, "S0");
         sensorLog0.refresh();
         result = sensorLog0.readDataEntry(line_num, buf);
      }break;
      case 1:{
         SD_DataFile sensorLog1 = SD_DataFile(DATA_LEN, "S1");
         sensorLog1.refresh();
         result = sensorLog1.readDataEntry(line_num, buf);
      }break;      
      case 2:{
         SD_DataFile sensorLog2 = SD_DataFile(DATA_LEN, "S2");
         sensorLog2.refresh();
         result = sensorLog2.readDataEntry(line_num, buf);
      }break;     
      case 3:{
         SD_DataFile sensorLog3 = SD_DataFile(DATA_LEN, "S3");
         sensorLog3.refresh();
         result = sensorLog3.readDataEntry(line_num, buf);
      }break;     
      case 4:{
         SD_DataFile sensorLog4 = SD_DataFile(DATA_LEN, "S4");
         sensorLog4.refresh();
         result = sensorLog4.readDataEntry(line_num, buf);
      }break;        
    }
    return result;
  
 }


bool sd_sensor_log:: write_delimeter(int sensor_num){
  write_sensor(delimeter, sensor_num);
  
}
 
 bool sd_sensor_log :: data_dump_sensor(byte* data[], int sensor_num, int start_line, int num_entries){
    for(int i = 0; i < num_entries; i++){
      byte buf[DATA_LEN];
      read_sensor(buf, sensor_num, start_line+ i);
      data[i] = buf;
    }
  
  
 }
