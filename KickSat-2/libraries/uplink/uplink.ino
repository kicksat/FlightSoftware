#include "uplink.h"


void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB);
  delay(5000);
  configFile.init();
  configFile.setStandby();
}

void loop() {
  output_menu();
  char buf[50];
  uint8_t command = listenForUplink(buf, LISTENINGDURATION);
  SerialUSB.print("Command Index: ");
  SerialUSB.println(command);
  SerialUSB.print("Metadata: ");
  SerialUSB.print(buf[COMMAND_WIDTH]);
  SerialUSB.println();
  processUplink(buf, command);
  checkConfigStatus();
  SerialUSB.println();
  delay(5000);
}


/////////////////////////
////////////////////////////////////////////////////////////////////////////////
void output_menu() { // TODO: this is an artifact of serial, it gets replaced by the ground station receiving a beacon
  // Listen for Uplink:
  SerialUSB.println("Choose a command from the following menu: ");
  SerialUSB.println("dog) Downlink Data");
  SerialUSB.println("cat) Set Arming Mode");
  SerialUSB.println("pig) Reflash Sensor Config");
  SerialUSB.println("owl) Do Nothing");
  SerialUSB.println("brn) Burn Wires");

  
  SerialUSB.print("Choose a command: ");
}

void checkConfigStatus(){
  SerialUSB.println();
  SerialUSB.print("Status: ");
  SerialUSB.println((char)configFile.getStatus());
  SerialUSB.print("AB1 deployed: ");
  SerialUSB.print(configFile.getAB1status());
  SerialUSB.print(", AB2 deployed: ");
  SerialUSB.print(configFile.getAB2status());
  SerialUSB.print(", DB1 deployed: ");
  SerialUSB.print(configFile.getDB1status());
  SerialUSB.print(", DB2 deployed: ");
  SerialUSB.print(configFile.getDB2status());
  SerialUSB.print(", DB3 deployed: ");
  SerialUSB.println(configFile.getDB3status());
  SerialUSB.print("DB1 Flag: ");
  SerialUSB.print(configFile.getDB1FlagStatus());
  SerialUSB.print(", DB2 Flag: ");
  SerialUSB.print(configFile.getDB2FlagStatus());
  SerialUSB.print(", DB3 Flag: ");
  SerialUSB.println(configFile.getDB3FlagStatus());
  SerialUSB.print("Hold mode: ");
  SerialUSB.print(configFile.getHoldstatus());
  SerialUSB.print(", Standby mode: ");
  SerialUSB.print(configFile.getStandbyStatus());
  SerialUSB.print(", Armed mode: ");
  SerialUSB.print(configFile.getArmedStatus());
  SerialUSB.print(", Deployed mode: ");
  SerialUSB.println(configFile.getDeployedStatus());
  SerialUSB.print("Antenna Timer: ");
  SerialUSB.println(configFile.checkAntennaTimer());
  SerialUSB.println();
  delay(500);
}

