/*
 * Basic FPT - goes through each of the components in KMB and checks whether they are all responsive
 * 
 * Components to test: WDT, SD Card, Radio, IMU, Battery readings
 * 
 * Last updated: 5-9-2018
 *           By: Andrea 
 */

//Includes
#include <BattHandler.h>
#include <IMUHandler.h>
#include <RH_RF22.h>  
#include <RTCCounter.h>
#include <SdFat.h>
#include <SPI.h> 

//Global objects
BattHandle power;
Counter watchdogTimer;
IMUHandle IMU;
SdFat SD;

//Radio 
RHHardwareSPI spi;
RH_RF22 radio(SPI_CS_RFM, RF_NIRQ, spi);
RH_RF22::ModemConfig FSK1k2 = {
  0x2B, //reg_1c
  0x03, //reg_1f
  0x41, //reg_20
  0x60, //reg_21
  0x27, //reg_22
  0x52, //reg_23
  0x00, //reg_24
  0x9F, //reg_25
  0x2C, //reg_2c - Only matters for OOK mode
  0x11, //reg_2d - Only matters for OOK mode
  0x2A, //reg_2e - Only matters for OOK mode
  0x80, //reg_58
  0x60, //reg_69
  0x09, //reg_6e
  0xD5, //reg_6f
  0x24, //reg_70
  0x22, //reg_71
  0x01  //reg_72
};
                   
uint8_t packet[] = {
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01110101, 0b10010110, 0b01010100, 0b10101011, 0b01010100, 0b10101011, 0b01010111, 0b01110010,
  0b01011010, 0b01001000, 0b10110101, 0b10101101, 0b10001010, 0b00101000, 0b11101010, 0b10101111,
  0b10110011, 0b00110111, 0b00010000, 0b10110000, 0b11011000, 0b10000111, 0b00001000, 0b10101001,
  0b00101110, 0b10010001, 0b00010001, 0b00000011, 0b01111101, 0b11000000, 0b01111111
};

//Global variables
int SDWrites = 0;
unsigned int radioPackets = 1;

//Flags
bool LEDSTATE = false; // to toggle LED, helps us test whether the board is alive

void setup() {
  // Define pin modes
  pinMode(LED_BUILTIN, OUTPUT); // Defines builtin LED pin mode to output
  pinMode(WDT_WDI, OUTPUT); // Set watchdog pin mode to output
  pinMode(RF_SDN, OUTPUT);
  pinMode(SPI_CS_SD, OUTPUT);
  digitalWrite(WDT_WDI, LOW);

  // Begin timers
  watchdogTimer.init(1000, watchdog); // timer delay, milliseconds

  // Initialize Serial
  SerialUSB.begin(115200); // Restart SerialUSB
  //while(!SerialUSB); // Wait for SerialUSB USB port to open
  SerialUSB.println("SerialUSB Initialized");
  delay(2500); // Provides user with time to open SerialUSB Monitor or upload before sleep

  //Need a delay before turning on radio
  //so that power supply can stabilize
  digitalWrite(RF_SDN, HIGH);
  delay(2000);
  digitalWrite(RF_SDN, LOW);
  delay(500);

  BattHandle(); //initializes battery pins

  if(IMU.begin()){ // Initialize IMU
    SerialUSB.println("IMU Intialized");
  } else {
    SerialUSB.println("IMU Could Not Be Intialized");
  }

  if(SD.begin(SPI_CS_SD)) { //Initialize SD Card
    SerialUSB.println("SD initialized");
  } else {
    SerialUSB.println("SD not initialized");
  }

  if(!radio.init()) { //Setup radio
    SerialUSB.println("Radio couldnt initialize");
    SerialUSB.println(radio.statusRead(), HEX);
  } else {
    SerialUSB.println("Radio initialized");
  }
  radio.setFrequency(437.505, 0.1);
  radio.setModemRegisters(&FSK1k2);
  radio.setTxPower(RH_RF22_RF23BP_TXPOW_30DBM);

}

void loop() { //check all parts of the board

  //Check battery readings
  checkBattHandler();

  //Check IMU
  checkIMUHandler();

  //Check SD card 
  checkSDCard();

  //Check radio chriping
  checkRadio();

  delay(5000);
}

void checkBattHandler() { //read battery current draw, voltage, and charging current
  SerialUSB.println("******Testing the Power Readings******");
  float battCurr = power.readBattCurrent();
  SerialUSB.print("Current Draw:\t\t");
  SerialUSB.print(battCurr);
  SerialUSB.println(" mA");
  delay(500);
  float battery = power.readBattVoltage();
  SerialUSB.print("Battery Voltage:\t");
  SerialUSB.print(battery,3);
  SerialUSB.println(" V");
  delay(500);
  float battChrg = power.readBattChargeCurrent();
  SerialUSB.print("Charging Current:\t");
  SerialUSB.print(battChrg);
  SerialUSB.println(" mA");
  SerialUSB.println("");
}

void checkIMUHandler() {
  SerialUSB.println("******Testing the IMU******");
  float imuData[9];
  IMU.read(imuData); // Read IMU data
  for(int i = 0; i < 9; i++) { // Print to Serial IMU data
    SerialUSB.print(i);SerialUSB.print(": ");SerialUSB.print(imuData[i]);SerialUSB.print("\t");
  }
  SerialUSB.println("\n");
}

void checkSDCard() { //read and writes to the SD Card
  SerialUSB.println("******Testing the SD Card******");
  File filetest = SD.open("TestSD.txt", FILE_WRITE); //first open the file to write
  if(filetest) { //if the file exists, print one new line on the file
    SerialUSB.print("Writing to SD card...");
    filetest.print(SDWrites);
    filetest.println(": Hi KickSat Team! The SD Card is working correctly.");
    filetest.close();
    SDWrites++;
    SerialUSB.println("done");
  } else {
    SerialUSB.println("Couldnt open TestSD.txt");
  }

  filetest = SD.open("TestSD.txt", FILE_READ); //reopen the file to read
  if(filetest) { //if the file opened, read all its contents
    SerialUSB.println("SDFile contents: ");
    while(filetest.available()) {
      SerialUSB.print((char)filetest.read());
    }
    SerialUSB.println("");
    filetest.close();
  } else {
    SerialUSB.println("Couldnt open TestSD.txt");
  }
}

void checkRadio() { //sends a radio packet 
  SerialUSB.println("******Testing the Radio******");
  spi.begin();
  SerialUSB.print("Status byte: 0x");
  SerialUSB.print(radio.statusRead(), HEX);
  SerialUSB.print("\t Packet ");
  radio.send(packet, 95);
  radio.waitPacketSent(500);
  SerialUSB.println(radioPackets++);
  SerialUSB.println("");
}

void watchdog() { // Function that runs every time watchdog timer triggers
  digitalWrite(WDT_WDI, HIGH);
  delayMicroseconds(2);
  digitalWrite(WDT_WDI, LOW);
  if(LEDSTATE) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  LEDSTATE = !LEDSTATE;
  SerialUSB.println("Toggled WDT");
}
