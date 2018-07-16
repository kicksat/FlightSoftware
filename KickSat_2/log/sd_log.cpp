//sd_log.c

#include "sd_log.h"

void sd_log :: write_log(){

  
}

char* sd_log :: read_entry(int entryIndex){
  
}

char* sd_log :: data_dump(int numEntries){
  
}

void sd_log::set_antenna_flag(bool flag){
  antenna_flag = flag;
}

void sd_log::set_bw1_flag(bool flag){
  bw1_flag = flag;
}

void sd_log::set_bw2_flag(bool flag){
  bw2_flag = flag; 
}

void sd_log::set_bw3_flag(bool flag){
  bw3_flag = flag;
}

void sd_log::set_early_mode(bool flag){
  mode_isEarly = flag;
}

void sd_log::set_standby_mode(bool flag){
  mode_isStandby = flag;
}

void sd_log::set_deploying_mode(bool flag){
  mode_isDeploying = flag;
}

void sd_log::set_deployed_flag(bool flag){
  didDeploy_flag = flag;
}

void sd_log::set_varChanged_flag(bool flag){
  varChanged_flag = flag;
}

// takes in a byte of information and counts the number of ones --> returns a bool
bool byte_to_bool(uint8_t inBool){
  uint8_t mask = 0b10000000; 
  short int cnt = 0;
  for(int i = 0; i < 7; i++){
    if((mask && inBool) == 0b10000000){
      cnt++;
    }
    inBool = inBool << 1;
  }
  if(cnt > 4){
    return true;
  } else {
    return false;
  }
}
