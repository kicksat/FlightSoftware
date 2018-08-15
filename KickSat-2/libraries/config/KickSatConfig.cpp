#include "KickSatConfig.h"

String filenames[3] = {"config0.txt", "config1.txt", "config2.txt"};

int KickSatConfig :: checkAntennaTimer(){
  byte buf[3];
  if(readByteFromThree(buf, 1)){
    uint8_t result = buf[0];
    //SerialUSB.print("Antenna Timer: ");
    //SerialUSB.println((int)result);
    return (int)result;
  }
  return -1;
}

void  KickSatConfig :: incrementAntennaTimer(){
  int timerNum = checkAntennaTimer();
  timerNum ++;
  byte newTimerNum = (byte)(uint8_t)timerNum;
  writeByteToThree(newTimerNum, 1);

}

bool KickSatConfig :: getAB1status(){
  byte buf[3];
  if(readByteFromThree(buf, 0)){
    if(buf[0] == AB1_BYTE){
      return true;
    }
  }
  return false;
}

bool KickSatConfig :: getAB2status(){
 byte buf[3];
  if(readByteFromThree(buf, 0)){
    if(buf[0] == AB2_BYTE){
      return true;
    }
  }
  return false;
}

bool KickSatConfig :: getStandbyStatus(){
 byte buf[3];
  if(readByteFromThree(buf, 0)){
    if(buf[0] == STANDBY_BYTE){
      return true;
    }
  }
  return false;

}

bool KickSatConfig :: getArmedStatus(){
 byte buf[3];
  if(readByteFromThree(buf, 0)){
    if(buf[0] == ARMED_BYTE){
      return true;
    }
  }
  return false;
}

bool KickSatConfig :: getDeployedStatus(){
 byte buf[3];
  if(readByteFromThree(buf, 0)){
    if(buf[0] == DEPLOYED_BYTE){
      return true;
    }
  }
  return false;
}


void KickSatConfig :: setAB1Deployed(){
  if(writeByteToThree(AB1_BYTE, 0)){

  }
}

void KickSatConfig :: setAB2Deployed(){
  if(writeByteToThree(AB2_BYTE, 0)){

  }
}

void KickSatConfig :: setStandby(){
  if(writeByteToThree(STANDBY_BYTE, 0)){

  }
}

void KickSatConfig :: setArmed(){
  if(writeByteToThree(ARMED_BYTE, 0)){

  }
}

void KickSatConfig :: setDeployed(){
  if(writeByteToThree(DEPLOYED_BYTE, 0)){

  }
}


// Initialize SD card
bool KickSatConfig :: init() {
  if (!SDStatus) { // If the SD card is not initialized
    pinMode(CS, OUTPUT); // Set pinmode for the SD card CS to output
    startSD(); // Starts communication with the SD card
    SDStatus = SD.begin(CS); // Record initialization of the SD card
  }
  if(SDStatus){
     SerialUSB.println("SD initialization failed");
  }else{
    SerialUSB.println("SD  config initialized");
  }
  for(int i = 0; i < 3; i++){
   if(!initFile(filenames[i])){
    SDStatus = false;
    SerialUSB.println("File initialization failed");
   }
  }
  endSD(); // Ends communication with the SD card
  return SDStatus; // Returns true if SD card initializes
}


// Open (or make, if it doesn't already exist) the log file, verifying that it is read/write capable
bool KickSatConfig::available(String filename) {
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  File configFileHandle = SD.open(filename, FILE_WRITE); // Open file (or create new file if it doesn't exist)
  if (configFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    configFileHandle.close(); // Close file
  }
  endSD(); // Ends communication with the SD card
  return fileStatus; // Returns file status
}


// Starts communication with the SD card
void KickSatConfig::startSD() {
  digitalWrite(CS, LOW); // Set chip select to LOW to start communication with SD card
}


// Ends communication with the SD card
void KickSatConfig::endSD() {
  digitalWrite(CS, HIGH); // Set chip select to HIGH to end communication with SD card
}



bool KickSatConfig::writeByteToThree(byte data, int location){
  bool result[3] = {false, false, false};
  for(int i = 0; i < 3; i++){
    result[i] = writeByte(filenames[i], data, location);
  }
  if(result[0] && result[1] && result[2]){
    return true;
  }
  return false;
}


bool KickSatConfig::readByteFromThree(byte data[3], int location){
  for(int i = 0; i < 3; i++){
     data[i] = readByte(filenames[i], location);
  }
  if(data[0] == data[1]){
    if(data[1] == data[2]){ //all three are equal
       //SerialUSB.print("all bytes the same: ");
       //SerialUSB.println(data[0], HEX);
    }else{ // 0 and 1 equal 2 different
      data[2] = data[0];
      writeByte(filenames[2], data[2], location);
      SerialUSB.print("One was off, fixed now: ");
      SerialUSB.println((char)data[0]);
    }
  }else {
    if(data[0] == data[2]){ // 0 and 2 same 1 different
      data[1] = data[0];
      writeByte(filenames[1], data[1], location);
      SerialUSB.print("One was off, fixed now: ");
      SerialUSB.println((char)data[0]);
    }else if(data[1] == data[2]){// 1 and 2 same 0 different
      data[0] = data[1];
      writeByte(filenames[0], data[0], location);
      SerialUSB.print("One was off, fixed now: ");
      SerialUSB.println((char)data[0]);
    }else{                //all three different
        SerialUSB.println("uh oh all three bytes are different yikes!");
        return false;
    }
  }
  return true;


}

byte KickSatConfig::readByte(String filename, int location){
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  byte buf[1];
  File logFileHandle = SD.open(filename, FILE_READ); // Open file for reading
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    //goes to location
    bool validLocation = logFileHandle.seek(location);
    //ERROR CORRECTION HERE?
    if(!validLocation){
      SerialUSB.println("Error non valid location in file");
      logFileHandle.close();
      endSD();
      return false;
    }
    logFileHandle.read(buf, 1);
    //SerialUSB.print("0x");
    //SerialUSB.println((char)buf[0]); // Print to SerialUSB
    logFileHandle.close(); // Close file
  }
  endSD(); // Ends communication with the SD card
  return buf[0]; // Returns file status

}


bool KickSatConfig::writeByte(String filename, byte data, int location){
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  File logFileHandle = SD.open(filename, FILE_WRITE); // Open file for writing
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    //goes to location
    bool validLocation = logFileHandle.seek(location);
    //ERROR CORRECTION HERE?
    if(!validLocation){
      SerialUSB.println("Error non valid location in file");
      logFileHandle.close();
      endSD();
      return false;
    }
    // Writes data to file
    logFileHandle.print((char)data); // Print to config file on the SD card
    //SerialUSB.print("0x");
    //SerialUSB.println((char)data); // Print to SerialUSB
    logFileHandle.close(); // Close file
  }
  endSD(); // Ends communication with the SD card
  return fileStatus; // Returns file status

 }

bool KickSatConfig::initFile(String fileName){
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  File logFileHandle = SD.open(fileName, FILE_WRITE); // Open file for writing
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    char buf[3] = {'0', '0', '0'}; // Creates a default zer array to write to the file
    logFileHandle.seek(0);
    logFileHandle.print(buf); // Print to log file on the SD card
    SerialUSB.println(buf); // Print to SerialUSB
    logFileHandle.close(); // Close file
  }
  endSD(); // Ends communication with the SD card
  return fileStatus; // Returns file status
}



bool KickSatConfig::errorCorrectOpening(){
  bool canOpen[3] = {false, false, false};
  int numTrue = 0;
  for(int i = 0; i < 3; i++){
    canOpen[i] = available(filenames[0]);
    if(canOpen[i]){
      numTrue++;
    }
  }
  if(numTrue == 3){
    SerialUSB.println("All files can open");
    return true;
  }else if (numTrue ==2){
    //delete false file and make a new one, reinitialize then copy other files contents into it
    SerialUSB.println("One file can't open");
    return true;
  }else{
    SerialUSB.println("two files are corrupted and can't open.... lol you're screwed");
    return false;
  }
}

bool KickSatConfig::errorCorrectContents(){
  byte buf[3];
  if(readByteFromThree(buf, 0) && readByteFromThree(buf, 1)){
    return true;
  }
  return false;
}

KickSatConfig configFile; // Create config object
