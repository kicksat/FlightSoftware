#include "KickSat_Sensor.h"
#include <SPI.h>
extern SdFat SD;
File configFile; 

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


// function to split a string into an array of commands
String getCommand(String data, char separator, int index)
{
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
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//this is the main function for using the sensor. this function will execute commands on the sensor board's ADC based on the config file.
void KickSat_Sensor::operate(float* dataOut, String board) {

  wakeADC();
  delay(500);
  resetADC();
  delay(100);
  startADC();
  delay(100);

  if (board== "xtb1"){
    Serial.println(board);
    dataOut[0] = readTemp(); 
    
    dataOut[1] = readPins(0x1C, 0xFF, 0x82, false, 200, 100, 0x03); //test func
    
    dataOut[2] = readPins(0x1C, 0xFF, 0x82, false, 200, 100, 0x03); //test func
    
    dataOut[3] = readPins(0x1C, 0xFF, 0x82, false, 200, 100, 0x03); //test func
    
    dataOut[4] = readPins(0x1C, 0xFF, 0x82, false, 200, 100, 0x03); //test func
    
    dataOut[5] = readPins(0x1C, 0xFF, 0x82, false, 200, 100, 0x03); //test func
    
    dataOut[6] = readPins(0x1C, 0xFF, 0x82, false, 200, 100, 0x03); //test func
    
    dataOut[7] = readPins(0x1C, 0xFF, 0x82, false, 200, 100, 0x03); //test func

    // readPins(0x7C, 0xF7, 0x80, false, 200, 100, 0x03, "P1");
    // readPins(0x4C, 0xF4, 0x80, false, 200, 100, 0x03, "P2");
    // readPins(0x3C, 0xF3, 0x80, false, 200, 100, 0x03, "N1");
    // readPins(0x2C, 0xF2, 0x80, false, 200, 100, 0x03, "N2");
    // readPins(0xCC, 0xFF, 0x80, false, 20, 1, 0x01, "");
    // GPIO(0x00, 0x01);
    // readPins(0x58, 0xF5, 0x80, false, 200, 100, 0x01, "R1A");
    // GPIO(0x00, 0x00);
    // delay(50);
    // GPIO(0x00, 0x02);
    // readPins(0x59, 0xF5, 0x80, false, 200, 100, 0x01, "R1B");
    // GPIO(0x00, 0x00);
    // delay(50);
    // GPIO(0x00, 0x04);
    // readPins(0x0A, 0xF0, 0x80, false, 200, 100, 0x01, "R2A");
    // GPIO(0x00, 0x00);
    // delay(50);
    // GPIO(0x00, 0x08);
    // readPins(0x0B, 0xF0, 0x80, false, 200, 100, 0x01, "R2B");
    // GPIO(0x00, 0x00);
  }

  else if (board=="xtb2"){
    // readPins(0x6C, 0xF6, 0x80, false, 200, 100, 0x03, "Z1");
    // readPins(0x3C, 0xF3, 0x80, false, 200, 100, 0x03, "Z2");
    // readPins(0x2C, 0xF6, 0x80, false, 200, 100, 0x03, "Z1");
    // GPIO(0x00, 0x04);
    // readPins(0x1A, 0xF1, 0x80, false, 200, 100, 0x03, "Z2");
    // GPIO(0x00, 0x00);
    // delay(50);
    // GPIO(0x00, 0x02);
    // readPins(0x49, 0xF4, 0x80, false, 200, 100, 0x01, "ZA");
    // GPIO(0x00, 0x00);
    // readPins(0x5C, 0xF5, 0x80, false, 200, 100, 0x01, "ZB");
    // GPIO(0x00, 0x01);
    // readPins(0x78, 0xF7, 0x80, false, 200, 100, 0x01, "ZA");
    // GPIO(0x00, 0x00);

  }
  else if (board=="xtb3"){
  
  }
  
  shutdownADC();
}

//this function takes a command from parseMessage and executes it
void KickSat_Sensor::handleCommand(String cmd, float* buf, int* index) {
  String argv[11];
  bool save = false; 
  parseMessage(cmd, argv);
  
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

void KickSat_Sensor::BatteryVoltage(bool save) {                //Measure the battery voltage at pin 9(A7)
  float measuredvbat;
  measuredvbat = analogRead(VBATPIN);   
  measuredvbat *= 2;                    // resistor divider by 2,
  measuredvbat *= 3.3;                  // Multiply by 3.3V
  measuredvbat /= 1024;                 // convert to voltage
  Serial.print("Battery Voltage: ");
  Serial.println(measuredvbat, 3);
  if (save){
    logfile.println("B,"+String(micros())+","+String(measuredvbat,3)); 
  }
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
  byte commands[8]{0xCC, 0x08, 0x1C, 0x39, 0x00, 0xFF, 0x00, 0x10};
  Serial.println("------Resetting ADC------");
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x06); //send reset byte
  digitalWrite(_ADCreset, LOW);
  delayMicroseconds(1);
  digitalWrite(_ADCreset, HIGH);
  burstWriteRegs(0x42, 8, commands);
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
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  byte mosDat1, mosDat2, mosDat3 = 0;
  byte IDACpin = pinNum | 0xF0;
//  byte mosfetVCOM[9]{pinNum, 0x08, 0x0C, 0x39, 0x04, IDACpin, 0x00, 0x0A, 0x08};
  pinNum |= 0xC0;
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);   
  SPI.transfer(0x42);   //Send register START location
  SPI.transfer(0x06);   //how many registers to write to
  SPI.transfer(pinNum); //0x42  INPMUX 
  SPI.transfer(0x08);   //0x43  PGA
  SPI.transfer(0x1C);   //0x44  DATARATE
  SPI.transfer(0x39);   //0x45  REF
  SPI.transfer(0x04);   //0x46  IDACMAG
  SPI.transfer(IDACpin);//0x47  IDACMUX
  SPI.transfer(0x00);   //0x48  VBIAS 
  delay(5);
  SPI.transfer(0x00);   //NOP to get rid of junk?
  SPI.transfer(0x12); //transfer read command  
  mosDat1 = SPI.transfer(0x00);
  mosDat2 = SPI.transfer(0x00);
  mosDat3 = SPI.transfer(0x00);
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
  mosData = -1*dataConvert(mosDat1,mosDat2,mosDat3);
  Serial.println(mosData);
}

/*
  Read pin command
  in config file w/o quotes:
  "read [+ pin] [- pin] [IDAC pin] [VBias Pin] [Log Data t/f] [wait time (in ms)] [measurements to read] [IDAC mag] [data label text],"
  for example:
  "read 2 12 3 4 t 5,"
          measures a voltage between AIN2 and AINCOM (C hex = 12 DEC),
          drives a 100uA current on AIN3,
          applies a 1.6V bias on AIN2,
          logs the data to the SD card,
          and waits for 5 ms between measurements
*/
float KickSat_Sensor::readPins(byte pinNums, byte idacPin, byte vbPin, bool save, int wait, int bufflen, byte idacMag){
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  byte pinDat1, pinDat2, pinDat3 = 0;
  float pinData = 0; 
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1); 
  SPI.transfer(0x42);   //Send register START location
  SPI.transfer(0x06);   //how many registers to write to
  SPI.transfer(pinNums);//0x42  INPMUX 
  SPI.transfer(0x08);   //0x43  PGA
  SPI.transfer(0x1C);   //0x44  DATARATE
  SPI.transfer(0x39);   //0x45  REF
  SPI.transfer(idacMag);   //0x46  IDACMAG
  SPI.transfer(idacPin);   //0x47  IDACMUX
  SPI.transfer(vbPin);     //0x48  VBIAS -- BIASING AIN2
  delay(wait);
  for (uint8_t i = 0; i <= bufflen; i++) {    
    SPI.transfer(0x00);   //NOP to get rid of junk?
    SPI.transfer(0x00);   //NOP to get rid of junk?
    SPI.transfer(0x12);   //transfer read command  
    pinDat1 = SPI.transfer(0x00);
    pinDat2 = SPI.transfer(0x00);
    pinDat3 = SPI.transfer(0x00);    
    pinData += dataConvert(pinDat1,pinDat2,pinDat3);
    delay(2);
  }  
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
  pinData = pinData/bufflen;
  return pinData;
}

void KickSat_Sensor::GPIO(byte pins, byte state){
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1); 
  SPI.transfer(0x50);  
  SPI.transfer(0x01); 
  SPI.transfer(pins);
  SPI.transfer(state); 
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH); 
}

float KickSat_Sensor::readTemp() {
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  byte aa, bb, cc = 0;
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);   
  SPI.transfer(0x49);   //Send register START location
  SPI.transfer(0x00);   //how many registers to write to
  SPI.transfer(0x50);   //0x49  SYS  
  delay(5);
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
  if (temp < 129){
    dd = ((-1*(129.00-temp)*0.403)+25);
  }
  else {
    dd = ((-1*(129.00-temp)*0.403)+25);
  }
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);   
  SPI.transfer(0x49);   //Send register START location
  SPI.transfer(0x00);   //how many registers to write to
  SPI.transfer(0x10);   //0x49  SYS  
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
  return dd;
}

void KickSat_Sensor::CreateFile() {
  strcpy(filename, "DATA0000.CSV");                       // Template for file name, characters 6 & 7 get set automatically later   
  if (!SD.begin(SD_CS)) {                 // see if the card is present and can be initialized:
    pinMode(RED, OUTPUT);
    digitalWrite(RED,HIGH);                // ERROR card not present- light up RED LED
    while (1){};                           // stay in this while loop to prevent data loss
  }
  
  for (uint8_t i = 0; i < 100; i++) {     //ensure the filename is unique  
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;    
    if (! SD.exists(filename)) {          // create if does not exist, do not open existing, write, sync after write      
      break;
    }
  }  
  logfile = SD.open(filename, FILE_WRITE);
  writeable = true;
  if( ! logfile ) {
    pinMode(RED, OUTPUT);
    digitalWrite(RED,HIGH);                // ERROR unable to write SD file
    while (1){};                           // stay in this while loop to prevent data loss
  }
}

void KickSat_Sensor::writeHeader() {
  logfile.println("Data Label, Time (micro sec), var[1], var[2], var[3], etc...");
  logfile.println("#,XTB_Arduino,v3a,2018-09-30,M.Holliday");
}


/* ------- HALL SPIN DEVICE C ------- */
/* ------- PINS: 5 | 9 | 8 | 4 ------ */
void KickSat_Sensor::hallSpinC(byte idacmag) {
  String label = "H2";
  
  GPIO(0x00,0x02);
  float phase1 = hallGen(8, 4, idacmag, 5, 9, 50, label);
  GPIO(0x00,0x00);
  float phase2 = hallGen(9, 5, idacmag, 8, 4, 50, label);
  float phase7 = hallGen(8, 4, idacmag, 9, 5, 50, label);
  GPIO(0x00,0x01);
  float phase8 = hallGen(9, 5, idacmag, 4, 8, 50, label); 
  GPIO(0x00,0x00);
  
  float dataSpin = (phase1+phase2+(-1*phase7)+(-1*phase8))/4;
  float vApplied = (voltageApplied/4);
  voltageApplied = 0; 
}

/* ------- HALL SPIN DEVICE D ------- */
/* ------ PINS: 3 | 1 | 0 | 2 ----- */
void KickSat_Sensor::hallSpinD(byte idacmag) {
  String label = "H1";
  
  float phase1 = hallGen(0, 2, idacmag, 3, 1, 50, label);
  float phase2 = hallGen(1, 3, idacmag, 0, 2, 50, label);
  float phase7 = hallGen(0, 2, idacmag, 1, 3, 50, label);
  float phase8 = hallGen(1, 3, idacmag, 2, 0, 50, label); 
  
  float dataSpin = (phase1+phase2+(-1*phase7)+(-1*phase8))/4;
  float vApplied = (voltageApplied/4);
  voltageApplied = 0; 
}

void KickSat_Sensor::SFOCAL() {
  Serial.println("Self Calibration");
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x19); //send self offset command
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
  delay(100);
}


float KickSat_Sensor::hallGen(int inp, int inn, byte idacMag, int idacMux, int vb, int delayT, String label) {
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
  byte inByteA, inByteB, inByteC, vbPin, inByteD, inByteE, inByteF = 0;
  byte inpMux = ((inp << 4) | inn);
  byte vapp = (idacMux <<4) | vb;
  if (vb <= 1){
    vbPin = (0x80 | (vb+1));
  } else if (vb == 2){
    vbPin = 0x84;
  } else if (vb == 3){
    vbPin = 0x88;
  } else if (vb == 4){
    vbPin = 0x90;
  } else if (vb == 5){
    vbPin = 0xA0;
  } else if (vb > 5){
    vbPin = 0x80;
  }  
  digitalWrite(_ADCchipSelect, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x42);     //Send register START location
  SPI.transfer(0x06);     //how many registers to write to
  SPI.transfer(inpMux);   //0x42  INPMUX 
  SPI.transfer(0x08);     //0x43  PGA
  SPI.transfer(0x0C);     //0x44  DATARATE
  SPI.transfer(0x39);     //0x45  REF
  SPI.transfer(idacMag);           //0x46  IDACMAG
  SPI.transfer((0xF0 | idacMux));  //0x47  IDACMUX
  SPI.transfer(vbPin);             //0x48  VBIAS
  delay(delayT);
  SPI.transfer(0x00);
  SPI.transfer(0x12);     //transfer read command  
  inByteA = SPI.transfer(0x00);
  inByteB = SPI.transfer(0x00);
  inByteC = SPI.transfer(0x00);
  SPI.transfer(0x42);   //Send register START location
  SPI.transfer(0x00);   //0x42  INPMUX 
  SPI.transfer(vapp);   //how many registers to write to
  delay(10);
  SPI.transfer(0x00);
  SPI.transfer(0x12);     //transfer read command  
  inByteD = SPI.transfer(0x00);
  inByteE = SPI.transfer(0x00);
  inByteF = SPI.transfer(0x00);
  delay(1);
  digitalWrite(_ADCchipSelect, HIGH);
  float reading = dataConvert(inByteA,inByteB,inByteC);
  float voltageApp = dataConvert(inByteD,inByteE,inByteF);
  voltageApplied += voltageApp;  
  return reading;
}
