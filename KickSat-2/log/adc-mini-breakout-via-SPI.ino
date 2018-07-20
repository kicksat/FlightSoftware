/* Code adapted from Max's ADC <--> SAMD Mini Breakout board 
 * Using Mainboard pin locations & pin redefinitions
 * Andrea + Charlie
*/
#include <SPI.h>
#include <SD.h>
#include "wiring_private.h" // pinPeripheral() function
#define chipSelectPin 3 // ADC CS
#define startSync 9     // ADC start
#define resetPin 8      // ADC reset
#define ledPin 4

//https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json
//https://adafruit.github.io/arduino-board-index/package_adafruit_index.json 
//use python script to save to file
//const int TX_LED = PIN_LED_TXL;  //SAMD21 green LED
//const int RX_LED = PIN_LED_RXL;  //SAMD21 yellow LED

// reconfigure I2C --> SPI pins 
SPIClass mySPI(&sercom3, 20, 21, 7, SPI_PAD_3_SCK_1, SERCOM_RX_PAD_0);

bool PGAen = false;
bool startUP = false;
bool resetMe = false;
bool printData = true;
byte address, thisValue, address2, address3 = 0;
byte readOut0, readOut1, readOut2, readOut3, readOut4, readOut5, readOut6, readOut7, readOut8, readOut9, readOut10, readOut11, readOut12, readOut13, readOut14, readOut15, readOut16, readOut17 = 0;
byte inByte1, inByte2, inByte3 = 0;
unsigned long timer = 0;

/*------------------------------------------------*/
/*---------- varriables you need to set! ---------*/ 
const float refV = 2.5; //reference voltage for the ADC. Usually use internal 2.5V by setting the registers below
const float pgaGain = 1;
const float FSR = (refV*2)/pgaGain;
const float LSBsize = FSR/pow(2,24);
bool showHex = true;
long loopTime = 10;   // microseconds
/*------------------------------------------------*/
/*------------------------------------------------*/
void setup() {
  SerialUSB.begin(115200);
  delay(3);
//  pinMode(RX_LED, OUTPUT);
//  pinMode(TX_LED, OUTPUT);
  SerialUSB.println("Serial Connected");
  pinMode(resetPin,OUTPUT);
  pinMode(chipSelectPin,OUTPUT); 
  pinMode(startSync,OUTPUT); 
  pinMode(ledPin, OUTPUT);
  digitalWrite(startSync, HIGH);
  digitalWrite(resetPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(1);
  delay(500);
  mySPI.begin();

  // Assign pins 7, 20, 21 to SERCOM functionality
  pinPeripheral(7, PIO_SERCOM_ALT); // Note alternate!!
  pinPeripheral(21, PIO_SERCOM);
  pinPeripheral(20, PIO_SERCOM);
  
  mySPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
  
  
  /* inital startup routine (including reset)*/ 
  delay(100);
  resetADC();
  delay(1000); 
  /* register configuration - use excel calculator!*/
  digitalWrite(chipSelectPin, LOW);
  mySPI.transfer(0x42);   //Send register START location
  mySPI.transfer(0x07);   //how many registers to write to
  mySPI.transfer(0x8A);   //0x42  INPMUX  // was C5
  mySPI.transfer(0x08);   //0x43  PGA
  mySPI.transfer(0x18);   //0x44  DATARATE chop w/ 200 SPS
  mySPI.transfer(0x39);   //0x45  REF
  mySPI.transfer(0x03);   //0x46  IDACMAG
  mySPI.transfer(0xF5);   //0x47  IDACMUX
  mySPI.transfer(0x00);   //0x48  VBIAS
  mySPI.transfer(0x10);   //0x49  SYS
  delay(1);
  digitalWrite(chipSelectPin, HIGH);
  delay(3000);
  
  startADC();
  delay(100);
  /*Read all the register values*/ 
//  IDAC(true, 0x02, 0xF8); 
  delay(3000);
  delay(50);
//  SFOCAL();
//  delay(5);
  
  timer = micros();
//  SerialUSB.print("LSB size ");SerialUSB.println(LSBsize,DEC);

  digitalWrite(ledPin, LOW);
}
/*------------------------------------------------*/
/*------------------------------------------------*/

void loop() 
{
//  timeSync(loopTime);
//
//  /* HALL SPIN -- A --*/
//  float rDataA = 0;
//  writeReg(0x42, 0x60); 
//  delay(5);
//  writeReg(0x48, 0x08); 
//  delay(5);
//  IDAC(true, 0x04, 0xF2); 
////  delay(50);
////  SFOCAL();
//  delay(20);
//  rDataA = readData1(showHex = false, 1, printData = false);
// 
// /* HALL SPIN -- B --*/
//  float rDataB = 0;
//  writeReg(0x42, 0x23); 
//  delay(5);
//  writeReg(0x48, 0x01); 
//  delay(20);
//  IDAC(true, 0x04, 0xF6); 
////  delay(50);
////  SFOCAL();
//  delay(5);
//  rDataB = readData1(showHex = false, 1, printData = false);
//  float dataSpin = (rDataA-rDataB)*1;
//  SerialUSB.print(micros());
//  SerialUSB.print(",");
//  SerialUSB.println(dataSpin, DEC);

  digitalWrite(ledPin, HIGH);
  for (int j=1; j<=20; j++)
  {
    readData1(showHex = false, 1000, printData = true);
    delay(300);
  }
  digitalWrite(ledPin, LOW);
  regReadout();
  delay(1000);
}
/*------------------------------------------------*/
void sendToPC(byte* data)
{
  byte* byteData = (byte*)(data);
  SerialUSB.write(byteData, 4);
}

void timeSync(unsigned long deltaT)
{
  unsigned long currTime = micros();
  long timeToDelay = deltaT - (currTime - timer);
  if (timeToDelay > 5000)
  {
    delay(timeToDelay / 1000);
    delayMicroseconds(timeToDelay % 1000);
  }
  else if (timeToDelay > 0)
  {
    delayMicroseconds(timeToDelay);
  }
  else
  {
      // timeToDelay is negative so we start immediately
  }
  timer = currTime + timeToDelay;
}
/*Start ADC with command + SYNC pin --- WORKING ---*/
void startADC() {
  SerialUSB.println("---Starting ADC---");
  digitalWrite(chipSelectPin, LOW);
  mySPI.transfer(0x08); //send start byte
  digitalWrite(startSync, LOW);
  delay(4*1/10000000);
  digitalWrite(startSync, HIGH);
  delay(20);
  digitalWrite(chipSelectPin, HIGH);
}
/*Stop ADC with command + SYNC pin --- UNKNOWN ---*/
void stopADC() {
  SerialUSB.println("---Stopping ADC---");
  digitalWrite(chipSelectPin, LOW);
  mySPI.transfer(0x0A); //send start byte
  digitalWrite(startSync, LOW);
  delay(20);
  digitalWrite(chipSelectPin, HIGH);
}
/*Configure current excitation source --- WORKING ---*/
void IDAC(bool setIDAC, byte valIDAC, byte pinIDAC){
  if(setIDAC == 1){
    digitalWrite(chipSelectPin, LOW);
    mySPI.transfer(0x46);   //Send register START location
    mySPI.transfer(0x01);   //how many registers to write to
    mySPI.transfer(valIDAC);   //set 0x46 register to ENABLE @ 100uA
    mySPI.transfer(pinIDAC);   //set register to specified value
    delay(1);
    digitalWrite(chipSelectPin, HIGH); 
    }
  else{
    digitalWrite(chipSelectPin, LOW);
    mySPI.transfer(0x46);   //Send register START location
    mySPI.transfer(0x01);   //how many registers to write to
    mySPI.transfer(0x00);   //set 0x46 register to DISABLE
    mySPI.transfer(0xFF);   //set 0x47 register to DISCONNECT all IDAC pins
    delay(1);
    digitalWrite(chipSelectPin, HIGH);
    }
}
/*Reset ADC with command + reset pin --- WORKING ---*/
void resetADC() {
  SerialUSB.println("---Resetting ADC---");
  digitalWrite(chipSelectPin, LOW);
  mySPI.transfer(0x06); //send reset byte
  digitalWrite(resetPin, LOW);
  delay(4*1/10000000);
  digitalWrite(resetPin, HIGH);
  delay(4096*1/10000000);
  digitalWrite(chipSelectPin, HIGH); 
}
/*Read 24 bit data from ADC --- WORKING ---*/
float readData1(bool showHex, int scalar, bool printData) { //read the ADC data when STATUS and CRC bits are NOT enabled
//  SerialUSB.print("Data Read: ");
  float decVal = 0;
  /*Read the three bytes of 2's complement data from the ADC*/ 
  digitalWrite(chipSelectPin, LOW);  
  mySPI.transfer(0x12); //transfer read command  
  inByte1 = mySPI.transfer(0x00);
  inByte2 = mySPI.transfer(0x00);
  inByte3 = mySPI.transfer(0x00);
  delay(1);
  digitalWrite(chipSelectPin, HIGH);
  
  /*Convert the three bytes into readable data*/
  int rawData = 0; //create an empty 24 bit integer for the data
  rawData = (rawData << 8) | inByte1; //shift the data in one byte at a time
  rawData = (rawData << 8) | inByte2;
  rawData = (rawData << 8) | inByte3;
  
  /*Print the HEX value (if showHex = true)*/
  if (showHex == 1){
    SerialUSB.print("HEX Value: ");
    SerialUSB.println(rawData,HEX);
  }
  if (((1 << 23) & rawData) != 0){ //check if the value is negative    
    int mask = (1 << 24) - 1;
    int result = ((~rawData) & mask) + 1;
    decVal = float(result) * scalar * LSBsize * -1;
  }
  else{ //if it's not negative
    decVal = float(rawData)*LSBsize*scalar; //then just multiply by LSBsize
  }
//  SerialUSB.println(decVal, DEC);
  if (printData == 1){SerialUSB.println(decVal,DEC);}
  return decVal;
}
/*Read internal temperature --- NOT WORKING ---*/
void readTemp() {
  writeReg(0x43, 0x0A); //set PGA gain to 4 
  delay(20);
  writeReg(0x49, 0x50); //enable internal temp monitor
  delay(500);
  float a = readData1(showHex = false, 1000, printData = false);
  SerialUSB.print(0.1*a*(1/0.403)), SerialUSB.println(" degrees C");
  delay(5);
  writeReg(0x49, 0x18); //disable internal temp monitor
  delay(500);  
}
/*Write register value --- WORKING ---*/
void writeReg(byte address, byte thisValue) {
  digitalWrite(chipSelectPin, LOW);
  mySPI.transfer(address); //Send register location
  mySPI.transfer(0x00);
  mySPI.transfer(thisValue);  //Send value to write 
  delay(1);
  digitalWrite(chipSelectPin, HIGH);
  address = 0;
  thisValue = 0;
}
/*Read register value --- UNKNOWN ---*/
void readReg(byte address2) {  
  byte inByte = 0x00;
  byte result = 0x00;
  byte rawRegVal = 0x00;
  SerialUSB.print(address2, HEX);
  SerialUSB.print("\t");
  digitalWrite(chipSelectPin, LOW); 
  mySPI.transfer(address2);
  inByte = mySPI.transfer(0x00);
  result = mySPI.transfer(0x00);
  delay(1);
  digitalWrite(chipSelectPin, HIGH);
  rawRegVal = (rawRegVal << 8 | result);
  SerialUSB.print(rawRegVal, HEX);
  SerialUSB.print("\t");
  SerialUSB.println(result, HEX);
}
/*Read ALL of the registers in a row --- WORKING ---*/
void regReadout(){
  SerialUSB.println("------Register Readout-------");
  digitalWrite(chipSelectPin, LOW);
  mySPI.transfer(0x20);   //Send register START location
  mySPI.transfer(0x12);   //how many registers we want to read (0x12 = all 18)
  readOut0 = mySPI.transfer(0x00);
  readOut1 = mySPI.transfer(0x00);
  readOut2 = mySPI.transfer(0x00);
  readOut3 = mySPI.transfer(0x00);
  readOut4 = mySPI.transfer(0x00);
  readOut5 = mySPI.transfer(0x00);
  readOut6 = mySPI.transfer(0x00);
  readOut7 = mySPI.transfer(0x00);
  readOut8 = mySPI.transfer(0x00);
  readOut9 = mySPI.transfer(0x00);
  readOut10 = mySPI.transfer(0x00);
  readOut11 = mySPI.transfer(0x00);
  readOut12 = mySPI.transfer(0x00);
  readOut13 = mySPI.transfer(0x00);
  readOut14 = mySPI.transfer(0x00);
  readOut15 = mySPI.transfer(0x00);
  readOut16 = mySPI.transfer(0x00);
  readOut17 = mySPI.transfer(0x00);  
  delay(1);
  digitalWrite(chipSelectPin, HIGH);
  SerialUSB.print("Register 0x00 (ID):        "), SerialUSB.println(readOut0, HEX);
  SerialUSB.print("Register 0x01 (STATUS):    "), SerialUSB.println(readOut1, HEX);
  SerialUSB.print("Register 0x02 (INPMUX):    "), SerialUSB.println(readOut2, HEX);
  SerialUSB.print("Register 0x03 (PGA):       "), SerialUSB.println(readOut3, HEX);
  SerialUSB.print("Register 0x04 (DATARATE):  "), SerialUSB.println(readOut4, HEX);
  SerialUSB.print("Register 0x05 (REF):       "), SerialUSB.println(readOut5, HEX);
  SerialUSB.print("Register 0x06 (IDACMAG):   "), SerialUSB.println(readOut6, HEX);
  SerialUSB.print("Register 0x07 (IDACMUX):   "), SerialUSB.println(readOut7, HEX);
  SerialUSB.print("Register 0x08 (VBIAS):     "), SerialUSB.println(readOut8, HEX);
  SerialUSB.print("Register 0x09 (SYS):       "), SerialUSB.println(readOut9, HEX);
  SerialUSB.print("Register 0x0A (OFCAL0):    "), SerialUSB.println(readOut10, HEX);
  SerialUSB.print("Register 0x0B (OFCAL1):    "), SerialUSB.println(readOut11, HEX);
  SerialUSB.print("Register 0x0C (OFCAL2):    "), SerialUSB.println(readOut12, HEX);
  SerialUSB.print("Register 0x0D (FSCAL0):    "), SerialUSB.println(readOut13, HEX);
  SerialUSB.print("Register 0x0E (FSCAL1):    "), SerialUSB.println(readOut14, HEX);
  SerialUSB.print("Register 0x0F (FSCAL2):    "), SerialUSB.println(readOut15, HEX);
  SerialUSB.print("Register 0x10 (GPIODAT):   "), SerialUSB.println(readOut16, HEX);
  SerialUSB.print("Register 0x11 (GPIOCON):   "), SerialUSB.println(readOut17, HEX);
  SerialUSB.println("-----------------------------");
}

/*Initiate Self Calibration --- UNKNOWN ---*/
void SFOCAL() {
//  SerialUSB.println("Self Calibration");
  digitalWrite(chipSelectPin, LOW);
  mySPI.transfer(0x19); //send self offset command
  delay(1);
  digitalWrite(chipSelectPin, HIGH);
  delay(100);
}

/*Hall Switching --- UNKNOWN ---*/
//void hallSwitch() {
//  
//}

