#include "KickSat_Sensor.h"
#include <SD.h>
#include <SPI.h>

//constructor, sets up this sensor object with the corresponding config file
KickSat_Sensor::KickSat_Sensor(int adc_cs, int sd_cs, String cf_name) {
  _ADCchipSelect = adc_cs;
  _SDchipSelect = sd_cs;
  _configFileName = cf_name;
  //_dataFileName = df_name;
}

//this is the main function for using the sensor. this function will execute commands on the sensor board's ADC
//based on the config file.
void KickSat_Sensor::operate(byte* dataOut) {
  //read commands from config file
  SD.begin(_SDchipSelect);
  File configFile = SD.open(_configFileName, FILE_READ);
  //TODO: read in data file

  //byte buf[100]; //holds data from readData, after handling all commands, use it as you please
//  int bufIndex = 0; //needed to ensure data in buf is not overwritten
//  for (int i = 0; i < numCommands; i++) {
//    handleCommand(cmd, dataOut, &bufIndex);
//  }
}

//this function takes a command outputted by parseMessage, and executes that command
void KickSat_Sensor::handleCommand(String cmd, byte* buf, int* index) {
  String argv[6];
  parseMessage(cmd, argv);

  if (argv[0] == "delay") {
    delay(argv[1].toInt());
  } else if (argv[0] == "read") {
    //read data
    digitalWrite(_ADCchipSelect, LOW);  
    SPI.transfer(0x12); //transfer read command  
    byte inByte1 = SPI.transfer(0x00);
    byte inByte2 = SPI.transfer(0x00);
    byte inByte3 = SPI.transfer(0x00);
    delay(1);
    digitalWrite(_ADCchipSelect, HIGH);

    //save data
    int i = *index;
    buf[i] = inByte1;
    buf[i+1] = inByte2;
    buf[i+2] = inByte3;
    *index += 3;
  } else if (argv[0] == "setInputPins") {
    //setInputPins(argv[1].toInt(), argv[2].toInt());
  }
  
}

//this function takes one line from the config file
//and converts it into an executable command
void KickSat_Sensor::parseMessage(String msg, String arg[]) {
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

//================= Register Commands ====================//

//this function wirtes [len] registers to the values specified in [data]
//use to initialize/reset the ADC
void KickSat_Sensor::burstWriteRegs(byte* data, uint8_t len) {
  digitalWrite(_SDchipSelect, LOW);
  SPI.transfer(0x42);   //Send register START location
  SPI.transfer(len);   //how many registers to write to
  for (int i = 0; i < len; i++) {
    SPI.transfer(data[i]);
  }
  delay(1);
  digitalWrite(_SDchipSelect, HIGH);
}

//brings the ADC from STANDBY mode into CONVERSION mode
void KickSat_Sensor::startADC() {
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x0A); //send stop byte
  SPI.transfer(0x08); //send start byte
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
}

//brings the ADC from CONVERSION mode to STANDBY mode
void KickSat_Sensor::stopADC() {
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x0A); //send stop byte
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
}

//resets the ADC
void KickSat_Sensor::resetADC() {
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x06); //send reset byte
  digitalWrite(resetPin, LOW);
  delayMicroseconds(1);
  digitalWrite(resetPin, HIGH);
  digitalWrite(_ADCchipSelect, HIGH);
}

//brings the ADC from CONVERSION or STANDBY mode into SHUTDOWN mode
void KickSat_Sensor::shutdownADC() {
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x04); //send shutdown byte
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
}

//brings the ADC into STANDBY mode from SHUTDOWN mode
void KickSat_Sensor::wakeADC()  {
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x02); //send wakeup byte
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
}
