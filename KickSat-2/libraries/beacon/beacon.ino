#include "beacon.h"

SdFat SD;
SD_DataFile sensorLog1(SPI_CS_SD, DATA_WIDTH, "sl7.txt", SD);


void setup(){
  SerialUSB.begin(115200); // Restart SerialUSB
  while(!SerialUSB); // Wait for SerialUSB USB port to open
  SerialUSB.println("SerialUSB Initialized");
  delay(5000); 
  if(logfile.init()) { // Initialize log file
    SerialUSB.println("Log Card Initialized");
  } else {
    SerialUSB.println("Log Card Not Initialized");
  }
  sensorLog1.refresh();

  for(int i = 0; i < 10; i++){
    createRandomData();
    if(logfile.available()) {
      SerialUSB.println("**********CREATING LOG************");
      logfile.appendData();
    }
    delay(500);
  }
  SerialUSB.println("Sensor File:");
  char filler = 'A';
  for(int i = 0; i < 50; i++){
    sensorLog1.refresh();
    byte buf[DATA_WIDTH];
    buf[0] = (byte)filler;
    buf[1] = (byte)filler;
    buf[2] = (byte)filler;
    sensorLog1.writeDataEntry(buf);
    for(int j = 0; j < DATA_WIDTH; j++){
      SerialUSB.print((char)buf[0]);
    }
    SerialUSB.println();
    filler++;
  }
  SerialUSB.print("Logfile size: ");
  SerialUSB.println(sensorLog1.size());
  
    char beaconData[MAX_PACKET_SIZE];
    int len = beacon(beaconData, sensorLog1);
    SerialUSB.print("Beacon: ");
    for(int j = 0; j < len; j++){
      SerialUSB.print((char)beaconData[j]);
    }
    SerialUSB.println();
    SerialUSB.println();

    
   String output[BEACONS_PER_DOWNLINK];
   downLinkBeacon(sensorLog1, output);
   for(int i = 0; i < BEACONS_PER_DOWNLINK ; i++){
      SerialUSB.print("Beacon ");
      SerialUSB.print(i);
      SerialUSB.print(": ");
      SerialUSB.println(output[i]);
    }




}

void createRandomData() { // Temporary until we are     createRandomData(); // Temporary solution TODO: Complete data collection for all sensorsreading from each sensor
  data.status = random(0,10);
  for(uint8_t i = 0; i < 3; i++){
    data.powerData[i] = random(0,100)/13.87;
  }
  for(uint8_t i = 0; i < 2; i++){
    data.dateTime[i] = random(100000,999999);
  }
  for(uint8_t i = 0; i < 3; i++){
    data.gpsData[i] = random(0,200)/13.87;
  }
  for(uint8_t i = 0; i < 9; i++){
    data.imuData[i] = random(0,100)/9.123;
  }
}



void loop(){



}
