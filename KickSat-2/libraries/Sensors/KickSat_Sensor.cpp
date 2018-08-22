#include "KickSat_Sensor.h"
#include "ChecksumHandler.h"
#include <SPI.h>
#include <SdFat.h>
#define Serial SerialUSB

//constructor, sets up this sensor object with the corresponding config file
KickSat_Sensor::KickSat_Sensor(int adc_cs, int adc_rst, int sd_cs, String cf_name, SdFat _sd) {
  _ADCchipSelect = adc_cs;
  _ADCreset = adc_rst;
  _SDchipSelect = sd_cs;
  _configFileName = cf_name;
  SD = _sd;

  pinMode(_ADCchipSelect, OUTPUT);
  pinMode(_ADCreset, OUTPUT);
  digitalWrite(_ADCchipSelect, HIGH);
  digitalWrite(_ADCreset, HIGH);
}

//this is the main function for using the sensor. this function will execute commands on the sensor board's ADC
//based on the config file.
void KickSat_Sensor::operate(byte* dataOut) {
  //read commands from config file
  if (validateConfigFiles()) {
    SD.begin(_SDchipSelect);
    digitalWrite(_SDchipSelect, LOW);
    File _configFile = SD.open(_configFileName, FILE_READ);
    //extract commands from config file
    int numCommands = 0;
    String commandString = "";
    while (true) {
      char nextChar = _configFile.read();
      if (nextChar == 255) {
        break;
      } else if (nextChar == '\n') {
        numCommands++;
      }
      commandString += nextChar;
    }
    digitalWrite(_SDchipSelect, HIGH);
    
    //wake the ADC, talk to it, and then shut it down again
    wakeADC();
    int bufIndex = 0; //needed to ensure data in buf is not overwritten

    for (int i = 0; i < numCommands; i++) {
      handleCommand(getCommand(commandString, '\n', i), dataOut, &bufIndex);
    }
    shutdownADC();
  } else {
    //TODO: handle case in which error correction fails
    Serial.println("Warning: Config files corrupted beyond repair");
  }
  
}

//this function accepts a string of commands, a separator to delimit by, and an index to search for
//it then cuts the string into chunks, based on the specified separator, and returns the chunk
//that corresponds with the provided index
String KickSat_Sensor::getCommand(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1] - 1) : "";
}

//this function takes a command outputted by parseMessage, and executes that command
void KickSat_Sensor::handleCommand(String cmd, byte* buf, int* index) {
  String argv[6];
  parseMessage(cmd, argv);

  Serial.println(argv[0]);
  
  if (argv[0] == "read") {
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
  } else if (argv[0] == "delay") {
    delay(argv[1].toInt());
  } else if (argv[0] == "readout") {
    regReadout();
  } else if (argv[0] == "config") {
//    burstWriteRegs(argv[1], argv[2].toInt());
  } else if (argv[0] == "start") {
    startADC();
  } else if (argv[0] == "reset") {
    resetADC();
  } else if (argv[0] == "mosfet") {
    mosfetV(argv[1].toInt());
    int i = *index;
    buf[i] = val1;
    buf[i+1] = val2;
    buf[i+2] = val3;
    *index += 3;
    Serial.println(-1*dataConvert(val1,val2,val3)); 
  } else if (argv[0] == "stop") {
    stopADC();
  } else {
    Serial.println("NOP: No command performed");
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
      wordStart = index + 1;
      wordIndex++;
    }
    index++;
  }
  arg[wordIndex] = msg.substring(wordStart, index);
}


//==================== Config Commands ====================//

//this function accepts a buffer and a length,
//and will write this buffer and a checksum byte based on that buffer to the config files
//returns true if all the writes succeeded, false otherwise
bool KickSat_Sensor::rewriteConfigs(byte* buf, int len) {
  String backup1 = _configFileName.substring(0, _configFileName.length() - 4) + "1.txt";
  String backup2 = _configFileName.substring(0, _configFileName.length() - 4) + "2.txt";
  File cFile;

  SD.begin(_SDchipSelect); //TODO: figure out why this was necessary, and clean this fucking library based on that knowledge
  digitalWrite(_SDchipSelect, LOW);
  byte checksumByte = Checksum.calculateChecksum(buf, len);

  bool success = true;

  //O_CREAT makes it so that the file will be created again if it is missing
  //O_WRITE allows the file to be written to
  //O_TRUNC deletes the file if it already exists, so that a new, correct one can take its place
  cFile = SD.open(_configFileName, O_CREAT | O_WRITE | O_TRUNC);
  if (!cFile) {success = false;}
  cFile.write(buf, len);
  cFile.write(checksumByte);
  cFile.close();

  cFile = SD.open(backup1, O_CREAT | O_WRITE | O_TRUNC);
  if (!cFile) {success = false;}
  cFile.write(buf, len);
  cFile.write(checksumByte);
  cFile.close();

  cFile = SD.open(backup2, O_CREAT | O_WRITE | O_TRUNC);
  if (!cFile) {success = false;}
  cFile.write(buf, len);
  cFile.write(checksumByte);
  cFile.close();

  digitalWrite(_SDchipSelect, HIGH);

  return success;
}

//this function reads the main config file and its two backups, and validates their checksums
//if any file fails the checksum, it is overwritten by one of the files which passed the checksum
//if all three files are corrupted, returns false
//otherwise returns true
bool KickSat_Sensor::validateConfigFiles() {
  //set up initial variables
  bool valid_0 = true;
  bool valid_1 = true;
  bool valid_2 = true;
  String backup1 = _configFileName.substring(0, _configFileName.length() - 4) + "1.txt";
  String backup2 = _configFileName.substring(0, _configFileName.length() - 4) + "2.txt";
  File correctFile;
  File _configFile;
  int sz0 = 0;
  int sz1 = 0;
  int sz2 = 0;

  SD.begin(_SDchipSelect);
  digitalWrite(_SDchipSelect, LOW);
  
  //make sure the files can even open, and initialize buffer and sz variables
  _configFile = SD.open(_configFileName, FILE_READ);
  if (!_configFile) {
    //Serial.println("cannot open config 0");
    valid_0 = false;
  } else {
    sz0 = _configFile.size();
  }
  _configFile.close();
  _configFile = SD.open(backup1, FILE_READ);
  if (!_configFile) {
    //Serial.println("cannot open config 1");
    valid_1 = false;
  } else {
    sz1 = _configFile.size();
  }
  _configFile.close();
  _configFile = SD.open(backup2, FILE_READ);
  if (!_configFile) {
    //Serial.println("cannot open config 2");
    valid_2 = false;
  } else {
    sz2 = _configFile.size();
  }
  _configFile.close();
  int maxsz = sz0;
  if (sz1 > maxsz) {
    maxsz = sz1;
  }
  if (sz2 > maxsz) {
    maxsz = sz2;
  }
  //open a buffer large enough to hold any of the three files
  byte buf[maxsz];
  
  //check base config file
  if (valid_0) {
    _configFile = SD.open(_configFileName, FILE_READ);
    _configFile.read(buf, sz0);
    valid_0 = Checksum.evaluateChecksum(buf, sz0);
    _configFile.close();
  }
  
  //check backup 1
  if (valid_1) {
    _configFile = SD.open(backup1, FILE_READ);
    _configFile.read(buf, sz1);
    valid_1 = Checksum.evaluateChecksum(buf, sz1);
    _configFile.close();
  }
  
  //check backup 2
  if (valid_2) {
    _configFile = SD.open(backup2, FILE_READ);
    _configFile.read(buf, sz2);
    valid_2 = Checksum.evaluateChecksum(buf, sz2);
    _configFile.close();
  }
  
  //identify a usable correct file
  if (valid_0) {
    correctFile = SD.open(_configFileName, FILE_READ);
  } else if (valid_1) {
    correctFile = SD.open(backup1, FILE_READ);
  } else if (valid_2) {
    correctFile = SD.open(backup2, FILE_READ);
  } else {
    //Serial.println("no correct file available");
    return false;
  }
  
  //rewrite any corrupted files
  int correctsz = correctFile.size();
  correctFile.read(buf, correctsz);
  correctFile.close();
  if (!valid_0) {
    //O_CREAT makes it so that the file will be created again if it is missing
    //O_WRITE allows the file to be written to
    //O_TRUNC deletes the file if it already exists, so that a new, correct one can take its place
    _configFile = SD.open(_configFileName, O_CREAT | O_WRITE | O_TRUNC);
    _configFile.write(buf, correctsz);
    _configFile.close();
  }
  if (!valid_1) {
    _configFile = SD.open(backup1, O_CREAT | O_WRITE | O_TRUNC);
    _configFile.write(buf, correctsz);
    _configFile.close();
  }
  if (!valid_2) {
    _configFile = SD.open(backup2, O_CREAT | O_WRITE | O_TRUNC);
    _configFile.write(buf, correctsz);
    _configFile.close();
  }

  digitalWrite(_SDchipSelect, HIGH);
  
  return true;
}


//==================== Register Commands ====================//

//this function wirtes [len] registers to the values specified in [data]
//use to initialize/reset the ADC
void KickSat_Sensor::burstWriteRegs(byte start, uint8_t len, byte* data) {
  Serial.println("------Writing ADC Config------");
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_SDchipSelect, LOW);
  SPI.transfer(start);   //Send register START location
  SPI.transfer(len - 1);   //how many registers to write to (must be len-1 as the ADC considers 0x00 to be 1, 0x01 is 2, ect)
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
  //Serial.println("------Shutting Down ADC------");
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x04); //send shutdown byte
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
}

//brings the ADC into STANDBY mode from SHUTDOWN mode
void KickSat_Sensor::wakeADC()  {
  //Serial.println("------Waking ADC------");
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
