#include <KickSatConfig.h>
#include <KickSatLog.h>
SdFat SD;
KickSatConfig configFile(SD);

void setup() {
  SerialUSB.begin(9600);
  delay(1500);
  SerialUSB.println("serial initialized");
  configFile.init();
  //configFile.writeByteToAll('1', 2);
  configFile.errorCorrect();


//  for(int i = 60; i > 0; i--){
//    SerialUSB.println(i);
//    delay(1000);
//  }
//  
//  SD.begin(CS);
//  configFile.errorCorrectOpening();

//To use comment out private: in KickSatConfig.h

//   configFile.writeByte("config1.txt", (byte)'1', 0);
//   //configFile.writeByte("config3.txt", (byte)'2', 0);
//   //configFile.writeByte("config4.txt", (byte)'1', 0);
//   byte buf[NUM_FILES];
//   if(configFile.readByteFromAll(buf, 0)){
//     SerialUSB.println("read successful!!!");
//   }else{
//     SerialUSB.println("read failed :( ");
//   }
//   SerialUSB.println("Bytes:");
//   for(int i = 0; i < NUM_FILES; i++){
//     SerialUSB.println((char)buf[i]);
//   }
//   if(configFile.readByteFromAll(buf, 0)){
//     SerialUSB.println("read successful!!!");
//   }else{
//     SerialUSB.println("read failed :( ");
//   }
//   SerialUSB.println("Bytes:");
//   for(int i = 0; i < NUM_FILES; i++){
//     SerialUSB.println((char)buf[i]);
//   }
}



void loop(){
   configFile.setHold();
   checkConfigStatus();
    
   for(int i = 0; i < 10; i++){
     configFile.incrementAntennaTimer();
     checkConfigStatus();
   }
  
   configFile.setAB1Deployed();
   checkConfigStatus();
  
   configFile.setAB2Deployed();
   checkConfigStatus();
  
   configFile.setStandby();
   checkConfigStatus();
  
   configFile.setArmed();
   checkConfigStatus();
  
   configFile.setDB1Flag();
   checkConfigStatus();
  
   configFile.setDB2Flag();
   checkConfigStatus();
  
   configFile.setDB3Flag();
   checkConfigStatus();
  
   configFile.setDB1Deployed();
   checkConfigStatus();
  
   configFile.setDB2Deployed();
   checkConfigStatus();
  
   configFile.setDB3Deployed();
   checkConfigStatus();
  
   configFile.setDeployed();
   checkConfigStatus();
  
   delay(10000);

   configFile.init();

}

void checkConfigStatus(){
  SerialUSB.println();
  SerialUSB.print("Status: ");
  SerialUSB.println((char)configFile.getStatus());
  SerialUSB.print("AB1 deployed: ");
  SerialUSB.println(configFile.getAB1status());
  SerialUSB.print("AB2 deployed: ");
  SerialUSB.println(configFile.getAB2status());
  SerialUSB.print("DB1 flag: ");
  SerialUSB.println(configFile.getDB1FlagStatus());
  SerialUSB.print("DB2 flag: ");
  SerialUSB.println(configFile.getDB2FlagStatus());
  SerialUSB.print("DB3 flag: ");
  SerialUSB.println(configFile.getDB3FlagStatus());
  SerialUSB.print("DB1 deployed: ");
  SerialUSB.println(configFile.getDB1status());
  SerialUSB.print("DB2 deployed: ");
  SerialUSB.println(configFile.getDB2status());
  SerialUSB.print("DB3 deployed: ");
  SerialUSB.println(configFile.getDB3status());
  SerialUSB.print("Hold mode: ");
  SerialUSB.println(configFile.getHoldstatus());
  SerialUSB.print("Standby mode: ");
  SerialUSB.println(configFile.getStandbyStatus());
  SerialUSB.print("Armed mode: ");
  SerialUSB.println(configFile.getArmedStatus());
  SerialUSB.print("Deployed mode: ");
  SerialUSB.println(configFile.getDeployedStatus());
  SerialUSB.print("Antenna Timer: ");
  SerialUSB.println(configFile.checkAntennaTimer());
  SerialUSB.println();
  delay(500);
}
