#include "KickSatConfig.h"
#include <KickSatLog.h>
SdFat SD;
KickSatConfig configFile(SD);

void setup() {
  SerialUSB.begin(9600);
  delay(1000);
  configFile.init();
  configFile.errorCorrectOpening();

  configFile.setHold();
  checkConfigStatus();
  configFile.writeByteToThree((byte)0, TIMER_LOC);
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

void loop(){

}
