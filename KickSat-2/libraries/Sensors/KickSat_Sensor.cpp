#include "KickSat_Sensor.h"
#include <SD.h>

KickSat_Sensor::KickSat_Sensor(int adc_cs, int sd_cs, String cf_name) {
  _ADCchipSelect = adc_cs;
  _SDchipSelect = sd_cs;
  _configFileName = cf_name;
  //_dataFileName = df_name;
  _dataFile = df;
}

void KickSat_Sensor::operate(byte* dataOut) {
  //read commands from config file
  SD.begin(_SDchipSelect);
  File configFile = SD.open(_configFileName, FILE_READ);
  //TODO: read in data file

  //byte buf[100]; //holds data from readData, after handling all commands, use it as you please
  int bufIndex = 0; //needed to ensure data in buf is not overwritten
  for (int i = 0; i < numCommands; i++) {
    handleCommand(cmd, dataOut, &bufIndex);
  }
}

void KickSat_Sensor::handleCommand(String cmd, byte* buf, int* index) {
  String argv[6];
  parseMessage(cmd, argv);

  if (argv[0] == "delay") {
    delay(argv[1].toInt());
  } else if (argv[0] == "read") {
    //read data
    digitalWrite(adc_cs, LOW);  
    SPI.transfer(0x12); //transfer read command  
    inByte1 = SPI.transfer(0x00);
    inByte2 = SPI.transfer(0x00);
    inByte3 = SPI.transfer(0x00);
    delay(1);
    digitalWrite(adc_cs, HIGH);

    //save data
    int i = *index;
    buf[i] = inByte1;
    but[i+1] = inbyte2;
    buf[i+2] = inbyte3;
    *index += 3;
  } else if (argv[0] == "setInputPins") {
    setInputPins(argv[1].toInt(), argv[2].toInt());
  }
  
}

void parseMessage(String msg, String arg[]) {
  int index = 0;

  int wordIndex = 0;
  int wordStart = 0;
  while (index < msg.length()) {
    if (msg.charAt(index) == ' ') {
      arg[wordIndex] = msg.substring(wordStart, index);
      //SerialUSB.println(arg[wordIndex]);
      //SerialUSB.println(arg[wordIndex]);
      wordStart = index + 1;
      wordIndex++;
    }
    index++;
  }
  arg[wordIndex] = msg.substring(wordStart, index);
}
