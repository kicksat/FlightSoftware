#include "KickSat_Sensor.h"
#include <SD.h>
#include <SPI.h>

//constructor, sets up this sensor object with the corresponding config file
KickSat_Sensor::KickSat_Sensor(int adc_cs, int adc_rst, int sd_cs, String cf_name) {
  _ADCchipSelect = adc_cs;
  _ADCreset = adc_rst;
  _SDchipSelect = sd_cs;
  _configFileName = cf_name;

  pinMode(_ADCchipSelect, OUTPUT);
  pinMode(_ADCreset, OUTPUT);
  digitalWrite(_ADCchipSelect, HIGH);
  digitalWrite(_ADCreset, HIGH);
}

//this is the main function for using the sensor. this function will execute commands on the sensor board's ADC
//based on the config file.
void KickSat_Sensor::operate(byte* dataOut) {
  //read commands from config file
  //File configFile = SD.open(_configFileName, FILE_READ);
  //TODO: read in data file
  //configFile.close();

  //  TEST COMMANDS  //
  //these commands are here just to test command parsing functionality
  //in the final version, these will be read from a config file
  //TODO: read these from a config file
  int numCommands = 10;
  String commandList[] = {
    "reset",
    "delay 500",
    "readout",
    "delay 500",
    "start",
    "delay 250",
    "mosfet 4",
    "delay 250",
    "mosfet 5",
    "delay 250",
    "stop"
  };

  
  int bufIndex = 0; //needed to ensure data in buf is not overwritten
  for (int i = 0; i < numCommands; i++) {
    //Serial.println(commandList[i]);
    handleCommand(commandList[i], dataOut, &bufIndex);
  }
}

//this function takes a command outputted by parseMessage, and executes that command
void KickSat_Sensor::handleCommand(String cmd, byte* buf, int* index) {
  String argv[6];
  parseMessage(cmd, argv);

  //Serial.println(argv[0]);
  
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

//    //save data
//    int i = *index;
//    buf[i] = inByte1;
//    buf[i+1] = inByte2;
//    buf[i+2] = inByte3;
//    *index += 3;
  } else if (argv[0] == "readout") {
    regReadout();
  } else if (argv[0] == "config") {
//    burstWriteRegs(argv[1], argv[2].toInt());
  } else if (argv[0] == "start") {
    startADC();
  } else if (argv[0] == "reset") {
    //Serial.println("resetting");
    resetADC();
  } else if (argv[0] == "mosfet") {
    mosfetV(argv[1].toInt());
    int i = *index;
    buf[i] = val1;
    buf[i+1] = val2;
    buf[i+2] = val3;
    *index += 3;
    Serial.println(-1*dataConvert(val1,val2,val3)); 
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
//      Serial.println(arg[wordIndex]);
//      Serial.println(arg[wordIndex]);
      wordStart = index + 1;
      wordIndex++;
    }
    index++;
  }
  arg[wordIndex] = msg.substring(wordStart, index);
}


//==================== Register Commands ====================//

//this function wirtes [len] registers to the values specified in [data]
//use to initialize/reset the ADC
void KickSat_Sensor::burstWriteRegs(byte* data, uint8_t len) {
  Serial.println("------Writing ADC Config------");
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
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
  Serial.println("------Starting ADC------");
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x0A); //send stop byte
  SPI.transfer(0x08); //send start byte
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
}

//brings the ADC from CONVERSION mode to STANDBY mode
void KickSat_Sensor::stopADC() {
  Serial.println("------Stopping ADC------");
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x0A); //send stop byte
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
}

//resets the ADC
void KickSat_Sensor::resetADC() {
  Serial.println("------Resetting ADC------");
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x06); //send reset byte
  digitalWrite(_ADCreset, LOW);
  delayMicroseconds(1);
  digitalWrite(_ADCreset, HIGH);
  delay(1);
  SPI.transfer(0x42);   //Send register START location
  SPI.transfer(0x07);   //how many registers to write to
  SPI.transfer(0xCC);   //0x42  INPMUX 
  SPI.transfer(0x08);   //0x43  PGA
  SPI.transfer(0x1D);   //0x44  DATARATE
  SPI.transfer(0x39);   //0x45  REF
  SPI.transfer(0x00);   //0x46  IDACMAG
  SPI.transfer(0xFF);   //0x47  IDACMUX
  SPI.transfer(0x00);   //0x48  VBIAS
  SPI.transfer(0x10);   //0x49  SYS
//  SPI.transfer(0x19);   //system calibration
  SPI.transfer(0x0A);   //send stop byte
  SPI.transfer(0x08);   //send start byte
  delay(20);
  digitalWrite(_ADCchipSelect, HIGH);
}

//brings the ADC from CONVERSION or STANDBY mode into SHUTDOWN mode
void KickSat_Sensor::shutdownADC() {
  Serial.println("------Shutting Down ADC------");
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x04); //send shutdown byte
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
}

//brings the ADC into STANDBY mode from SHUTDOWN mode
void KickSat_Sensor::wakeADC()  {
  Serial.println("------Waking ADC------");
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x02); //send wakeup byte
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
}

void KickSat_Sensor::regReadout(){
  Serial.println("------Register Readout------");
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x20);   //Send register START location
  SPI.transfer(0x12);   //how many registers we want to read (0x12 = all 18)
  byte readOut0 = SPI.transfer(0x00);
  byte readOut1 = SPI.transfer(0x00);
  byte readOut2 = SPI.transfer(0x00);
  byte readOut3 = SPI.transfer(0x00);
  byte readOut4 = SPI.transfer(0x00);
  byte readOut5 = SPI.transfer(0x00);
  byte readOut6 = SPI.transfer(0x00);
  byte readOut7 = SPI.transfer(0x00);
  byte readOut8 = SPI.transfer(0x00);
  byte readOut9 = SPI.transfer(0x00);
  byte readOut10 = SPI.transfer(0x00);
  byte readOut11 = SPI.transfer(0x00);
  byte readOut12 = SPI.transfer(0x00);
  byte readOut13 = SPI.transfer(0x00);
  byte readOut14 = SPI.transfer(0x00);
  byte readOut15 = SPI.transfer(0x00);
  byte readOut16 = SPI.transfer(0x00);
  byte readOut17 = SPI.transfer(0x00);  
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
  Serial.print("Register 0x00 (ID):        "), Serial.println(readOut0, HEX);
  Serial.print("Register 0x01 (STATUS):    "), Serial.println(readOut1, HEX);
  Serial.print("Register 0x02 (INPMUX):    "), Serial.println(readOut2, HEX);
  Serial.print("Register 0x03 (PGA):       "), Serial.println(readOut3, HEX);
  Serial.print("Register 0x04 (DATARATE):  "), Serial.println(readOut4, HEX);
  Serial.print("Register 0x05 (REF):       "), Serial.println(readOut5, HEX);
  Serial.print("Register 0x06 (IDACMAG):   "), Serial.println(readOut6, HEX);
  Serial.print("Register 0x07 (IDACMUX):   "), Serial.println(readOut7, HEX);
  Serial.print("Register 0x08 (VBIAS):     "), Serial.println(readOut8, HEX);
  Serial.print("Register 0x09 (SYS):       "), Serial.println(readOut9, HEX);
  Serial.print("Register 0x0A (OFCAL0):    "), Serial.println(readOut10, HEX);
  Serial.print("Register 0x0B (OFCAL1):    "), Serial.println(readOut11, HEX);
  Serial.print("Register 0x0C (OFCAL2):    "), Serial.println(readOut12, HEX);
  Serial.print("Register 0x0D (FSCAL0):    "), Serial.println(readOut13, HEX);
  Serial.print("Register 0x0E (FSCAL1):    "), Serial.println(readOut14, HEX);
  Serial.print("Register 0x0F (FSCAL2):    "), Serial.println(readOut15, HEX);
  Serial.print("Register 0x10 (GPIODAT):   "), Serial.println(readOut16, HEX);
  Serial.print("Register 0x11 (GPIOCON):   "), Serial.println(readOut17, HEX);
  Serial.println("-----------------------------");
}

//----- MAX'S CRAP FUNCTIONS -----
float KickSat_Sensor::dataConvert( byte a, byte b, byte c){
  int rawDataIN = 0; //create an empty 24 bit integer for the data
  float dataOUT = 0;
  rawDataIN |= a; //shift the data in one byte at a time
  rawDataIN = (rawDataIN << 8) | b;
  rawDataIN = (rawDataIN << 8) | c;
  if (((1 << 23) & rawDataIN) != 0){ //If the rawData has bit 24 on, then it's a negative value
    int maskIN = (1 << 24) - 1;
    rawDataIN = ((~rawDataIN) & maskIN) + 1;
    dataOUT = rawDataIN * LSBsize * -1;}
  else{ //if it's not negative
    dataOUT = float(rawDataIN)*LSBsize;} //then just multiply by LSBsize
  return dataOUT;
}

void KickSat_Sensor::mosfetV(byte pinNum){
  byte mosDat1, mosDat2, mosDat3 = 0;
  byte IDACpin = pinNum | 0xF0;
  pinNum |= 0xC0;
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);   
  SPI.transfer(0x42);   //Send register START location
  SPI.transfer(0x06);   //how many registers to write to
  SPI.transfer(pinNum); //0x42  INPMUX 
  SPI.transfer(0x08);   //0x43  PGA
  SPI.transfer(0x0C);   //0x44  DATARATE
  SPI.transfer(0x39);   //0x45  REF
  SPI.transfer(0x04);   //0x46  IDACMAG
  SPI.transfer(IDACpin);//0x47  IDACMUX
  SPI.transfer(0x00);   //0x48  VBIAS
  SPI.transfer(0x0A);   //send stop byte
  SPI.transfer(0x08);   //send start byte
  delay(5);
  SPI.transfer(0x00);   //NOP to get rid of junk?
  SPI.transfer(0x12); //transfer read command  
  mosDat1 = SPI.transfer(0x00);
  mosDat2 = SPI.transfer(0x00);
  mosDat3 = SPI.transfer(0x00);
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
//  float mosData = -1*dataConvert(mosDat1,mosDat2,mosDat3);
  val1 = mosDat1;
  val2 = mosDat2;
  val3 = mosDat3;
}

void KickSat_Sensor::readTemp() {
  byte aa, bb, cc = 0;
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);   
  SPI.transfer(0x42);   //Send register START location
  SPI.transfer(0x07);   //how many registers to write to
  SPI.transfer(0xCC);   //0x42  INPMUX 
  SPI.transfer(0x08);   //0x43  PGA
  SPI.transfer(0x15);   //0x44  DATARATE
  SPI.transfer(0x39);   //0x45  REF
  SPI.transfer(0x00);   //0x46  IDACMAG
  SPI.transfer(0xFF);   //0x47  IDACMUX
  SPI.transfer(0x00);   //0x48  VBIAS
  SPI.transfer(0x50);   //0x49  SYS
  SPI.transfer(0x0A);   //send stop byte
  SPI.transfer(0x08);   //send start byte
  delay(50);
  SPI.transfer(0x00);   //send NOPS
  SPI.transfer(0x00);
  SPI.transfer(0x12); //transfer read command  
  aa = SPI.transfer(0x00);
  bb = SPI.transfer(0x00);
  cc = SPI.transfer(0x00);
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);  
  float temp = dataConvert(aa, bb, cc)*1000.0;
  float dd = 0;
//  Serial.print("Temperature: ");
  if (temp < 129){
//    Serial.print(temp,DEC),Serial.print("  ");
    dd = ((-1*(129.00-temp)*0.403)+25);
//    Serial.print(dd, 2), Serial.println(" degrees C");
  }
  else {
//    Serial.print(temp,DEC),Serial.print("  ");
    dd = ((-1*(129.00-temp)*0.403)+25);
//    Serial.print(dd, 2), Serial.println(" degrees C");
}
  delay(1);  
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);   
  SPI.transfer(0x42);   //Send register START location
  SPI.transfer(0x07);   //how many registers to write to
  SPI.transfer(0xCC);   //0x42  INPMUX 
  SPI.transfer(0x08);   //0x43  PGA
  SPI.transfer(0x0C);   //0x44  DATARATE
  SPI.transfer(0x39);   //0x45  REF
  SPI.transfer(0x00);   //0x46  IDACMAG
  SPI.transfer(0xFF);   //0x47  IDACMUX
  SPI.transfer(0x00);   //0x48  VBIAS
  SPI.transfer(0x00);   //0x49  SYS
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
  delay(1);
  Serial.println(dd, 2);
}
