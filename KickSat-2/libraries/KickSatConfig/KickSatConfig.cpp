#include "KickSatConfig.h"

String configFilenames[NUM_FILES] = {"config0.txt", "config1.txt", "config2.txt"};

KickSatConfig::KickSatConfig(SdFat _sd) {
  SD = _sd;
}

int KickSatConfig :: checkAntennaTimer(){
  byte buf[NUM_FILES];
  if(readByteFromAll(buf, TIMER_LOC)){
    uint8_t result = buf[0];
    return (int)result;
  }else{
    errorCorrect();
    if(readByteFromAll(buf, TIMER_LOC)){
      return (int)buf[0];
    }
  }
  return antennaTimer;
}

void  KickSatConfig :: incrementAntennaTimer(){
  int timerNum = checkAntennaTimer();
  timerNum ++;
  byte newTimerNum = (byte)(uint8_t)timerNum;
  if(!writeByteToAll(newTimerNum, TIMER_LOC)){
    errorCorrect();
    writeByteToAll(newTimerNum, TIMER_LOC);
  }
  antennaTimer = timerNum;
}

bool KickSatConfig :: getHoldstatus(){
  byte buf[NUM_FILES];
  if(!readByteFromAll(buf, STATUS_LOC)){
    errorCorrect();
    if(!readByteFromAll(buf, STATUS_LOC)){
      buf[0] = statusByte;
    }
  }
  if(buf[0] == HOLD_BYTE){
      return true;
  }
  return false;
}

byte KickSatConfig :: getStatus(){
  byte buf[NUM_FILES];
  if(!readByteFromAll(buf, STATUS_LOC)){
    errorCorrect();
    if(!readByteFromAll(buf, STATUS_LOC)){
      buf[0] = statusByte;
    }
  }
  return buf[0];
}


bool KickSatConfig :: getAB1status(){
  byte buf[NUM_FILES];
  if(!readByteFromAll(buf, AB1_LOC)){
    errorCorrect();
    if(!readByteFromAll(buf, AB1_LOC)){
      buf[0] = AB1status;
    }
  }
  if(buf[0] == FLAG_TRUE){
      return true;
  }
  return false;
}

bool KickSatConfig :: getAB2status(){
 byte buf[NUM_FILES];
  if(!readByteFromAll(buf, AB2_LOC)){
    errorCorrect();
    if(!readByteFromAll(buf, AB2_LOC)){
      buf[0] = AB2status;
    }
  }
  if(buf[0] == FLAG_TRUE){
      return true;
  }
  return false;
}

bool KickSatConfig :: getDB1status(){
  byte buf[NUM_FILES];
   if(!readByteFromAll(buf, DB1_LOC)){
     errorCorrect();
     if(!readByteFromAll(buf, DB1_LOC)){
      buf[0] = DB1status;
    }
   }
   if(buf[0] == FLAG_TRUE){
       return true;
     }
   return false;
}

bool KickSatConfig :: getDB2status(){
  byte buf[NUM_FILES];
   if(!readByteFromAll(buf, DB2_LOC)){
     errorCorrect();
     if(!readByteFromAll(buf, DB2_LOC)){
      buf[0] = DB2status;
    }
   }
   if(buf[0] == FLAG_TRUE){
       return true;
     }
   return false;
}

bool KickSatConfig :: getDB3status(){
  byte buf[NUM_FILES];
   if(!readByteFromAll(buf, DB3_LOC)){
     errorCorrect();
     if(!readByteFromAll(buf, DB3_LOC)){
      buf[0] = DB3status;
    }
   }
   if(buf[0] == FLAG_TRUE){
       return true;
     }
   return false;

}

bool KickSatConfig :: getDB1FlagStatus(){
  byte buf[NUM_FILES];
   if(!readByteFromAll(buf, DB1_FLAG_LOC)){
     errorCorrect();
     if(!readByteFromAll(buf, DB1_FLAG_LOC)){
      buf[0] = DB1FlagStatus;
    }
   }
   if(buf[0] == FLAG_TRUE){
       return true;
     }
   return false;
}

bool KickSatConfig :: getDB2FlagStatus(){
  byte buf[NUM_FILES];
   if(!readByteFromAll(buf, DB2_FLAG_LOC)){
     errorCorrect();
     if(!readByteFromAll(buf, DB2_FLAG_LOC)){
      buf[0] = DB2FlagStatus;
    }
   }
   if(buf[0] == FLAG_TRUE){
       return true;
     }
   return false;
}

bool KickSatConfig :: getDB3FlagStatus(){
  byte buf[NUM_FILES];
   if(!readByteFromAll(buf, DB3_FLAG_LOC)){
     errorCorrect();
     if(!readByteFromAll(buf, DB3_FLAG_LOC)){
      buf[0] = DB3FlagStatus;
    }
   }
   if(buf[0] == FLAG_TRUE){
       return true;
     }
   return false;
}

bool KickSatConfig :: getStandbyStatus(){
 byte buf[NUM_FILES];
  if(!readByteFromAll(buf, STATUS_LOC)){
    errorCorrect();
    if(!readByteFromAll(buf, STATUS_LOC)){
      buf[0] = statusByte;
    }
  }
  if(buf[0] == STANDBY_BYTE){
      return true;
    }
  return false;

}

bool KickSatConfig :: getArmedStatus(){
 byte buf[NUM_FILES];
  if(!readByteFromAll(buf, STATUS_LOC)){
    errorCorrect();
    if(!readByteFromAll(buf, STATUS_LOC)){
      buf[0] = statusByte;
    }
  }
  if(buf[0] == ARMED_BYTE){
      return true;
    }
  return false;
}

bool KickSatConfig :: getDeployedStatus(){
 byte buf[NUM_FILES];
  if(!readByteFromAll(buf, STATUS_LOC)){
    errorCorrect();
    if(!readByteFromAll(buf, STATUS_LOC)){
      buf[0] = statusByte;
    }
  }
  if(buf[0] == DEPLOYED_BYTE){
      return true;
  }
  return false;
}


void KickSatConfig :: setAB1Deployed(){
  if(!writeByteToAll(FLAG_TRUE, AB1_LOC)){
    errorCorrect();
    writeByteToAll(FLAG_TRUE, AB1_LOC);
  }
  AB1status = FLAG_TRUE;
}

void KickSatConfig :: setDB1Deployed(){
  if(!writeByteToAll(FLAG_TRUE, DB1_LOC)){
    errorCorrect();
    writeByteToAll(FLAG_TRUE, DB1_LOC);
  }
  DB1status = FLAG_TRUE;
}

void KickSatConfig :: setDB2Deployed(){
  if(!writeByteToAll(FLAG_TRUE, DB2_LOC)){
    errorCorrect();
    writeByteToAll(FLAG_TRUE, DB2_LOC);
  }
  DB2status = FLAG_TRUE;
}

void KickSatConfig :: setDB3Deployed(){
  if(!writeByteToAll(FLAG_TRUE, DB3_LOC)){
    errorCorrect();
    writeByteToAll(FLAG_TRUE, DB3_LOC);
  }
  DB3status = FLAG_TRUE;
}

void KickSatConfig :: setDB1Flag(){
  if(!writeByteToAll(FLAG_TRUE, DB1_FLAG_LOC)){
    errorCorrect();
    writeByteToAll(FLAG_TRUE, DB1_FLAG_LOC);
  }
  DB1FlagStatus = FLAG_TRUE;
}

void KickSatConfig :: setDB2Flag(){
  if(!writeByteToAll(FLAG_TRUE, DB2_FLAG_LOC)){
    errorCorrect();
    writeByteToAll(FLAG_TRUE, DB2_FLAG_LOC);
  }
  DB2FlagStatus = FLAG_TRUE;
}

void KickSatConfig :: setDB3Flag(){
  if(!writeByteToAll(FLAG_TRUE, DB3_FLAG_LOC)){
    errorCorrect();
    writeByteToAll(FLAG_TRUE, DB3_FLAG_LOC);
  }
  DB3FlagStatus = FLAG_TRUE;
}

void KickSatConfig :: setAB2Deployed(){
  if(!writeByteToAll(FLAG_TRUE, AB2_LOC)){
    errorCorrect();
    writeByteToAll(FLAG_TRUE, AB2_LOC);
  }
  AB2status = FLAG_TRUE;
}

void KickSatConfig :: setHold(){
  if(!writeByteToAll(HOLD_BYTE, STATUS_LOC)){
    errorCorrect();
    writeByteToAll(HOLD_BYTE, STATUS_LOC);
  }
  statusByte = HOLD_BYTE;
}

void KickSatConfig :: setStandby(){
  if(!writeByteToAll(STANDBY_BYTE, STATUS_LOC)){
    errorCorrect();
    writeByteToAll(STANDBY_BYTE, STATUS_LOC);
  }
  statusByte = STANDBY_BYTE;
}

void KickSatConfig :: setArmed(){
  if(!writeByteToAll(ARMED_BYTE, STATUS_LOC)){
    errorCorrect();
    writeByteToAll(ARMED_BYTE, STATUS_LOC);
  }
  statusByte = ARMED_BYTE;
}

void KickSatConfig :: setDeployed(){
  if(!writeByteToAll(DEPLOYED_BYTE, STATUS_LOC)){
    errorCorrect();
    writeByteToAll(DEPLOYED_BYTE, STATUS_LOC);
  }
  statusByte = DEPLOYED_BYTE;
}


// Initialize SD card
bool KickSatConfig :: init() {
  if (!SDStatus) { // If the SD card is not initialized (this is an exern variable in the kicksat log class)
    pinMode(CS, OUTPUT); // Set pinmode for the SD card CS to output
    startSD(); // Starts communication with the SD card
    SDStatus = SD.begin(CS); // Record initialization of the SD card
  }
  if(SDStatus){
     SerialUSB.println("SD initialized");
  }else{
    SerialUSB.println("SD initialization failed");
  }
  for(int i = 0; i < NUM_FILES; i++){  //inititalize all of the files, set default values
   if(!initFile(configFilenames[i])){ //if file cannot initialize
    SDStatus = false;                 //set SD status false TODO: add error correct opening
    SerialUSB.println("File initialization failed");
   }
  }
  endSD(); // Ends communication with the SD card

  AB1status = FLAG_FALSE;
  AB2status = FLAG_FALSE;
  DB1status = FLAG_FALSE;
  DB2status = FLAG_FALSE;
  DB3status = FLAG_FALSE;
  DB1FlagStatus = FLAG_FALSE;
  DB2FlagStatus = FLAG_FALSE;
  DB3FlagStatus = FLAG_FALSE;
  statusByte = '0';
  antennaTimer = 1;

  return SDStatus; // Returns true if SD card initializes
}


// Open (or make, if it doesn't already exist) a file, verifying that it is read/write capable
bool KickSatConfig::available(String filename) {
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  File configFileHandle = SD.open(filename, FILE_WRITE); // Open file (or create new file if it doesn't exist)
  if (configFileHandle != 0) { // If the file can be opened
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



bool KickSatConfig::writeByteToAll(byte data, int location){
  bool writeSuccessful[NUM_FILES];
  for(int i = 0; i < NUM_FILES; i++){ //initialize to false
    writeSuccessful[i] = false;
  }
  for(int i = 0; i < NUM_FILES; i++){ //write data to file and record if write successful
    writeSuccessful[i] = writeByte(configFilenames[i], data, location);
  }
  bool result = false;
  for(int i = 0; i < NUM_FILES; i++){ //checks that all writes were successful
    result = result && writeSuccessful[i];
  }
  return result;
}

/*reads a byte from the all of the files and then uses a voting system to
  determine which byte is correct... more than half of the bytes must be the
  same in order to count as a correct vote. The function then returns an
  array of data- if more than half of the bytes were the same, every entry in
  the array will be the correct byte. If the read failed it will return the
  data it read- several different bytes
*/
bool KickSatConfig::readByteFromAll(byte* data, int location){
  if(location > NUM_ENTRIES){ //can't read beyond num_entries because all values must be initialized when using sd seek
    return false;
  }
  for(int i = 0; i < NUM_FILES; i++){
     byte buf[1];
     if(!readByte(configFilenames[i], location, buf)){ //may be due to improper initialization- reinitialize and try again
       initFile(configFilenames[i]);
       if(!readByte(configFilenames[i], location, buf)){
        return false;
       }
     }
     data[i] = buf[0];
  }
  int votes[NUM_FILES]; //represents the number of times each byte is represented
  byte byteOptions[NUM_FILES]; //represents all the unique bytes in data

  for(int i = 0; i < NUM_FILES; i++){ //initialize votes to zero
    votes[i] = 0;
  }

  byteOptions[0] = data[0]; //set the first byte option to the first output of data
  for(int i = 0; i < NUM_FILES; i++){ //go through every data entry
    for(int j = 0; j < NUM_FILES; j++){ //compare to all byte options
      if(data[i] == byteOptions[j]){ //if data entry equal to an existing byte option
        votes[j]++;                  //increase the number of votes
        break;
      }else if(votes[j] == 0){  //if there are no votes, this is a byte we haven't seen before
        byteOptions[j] = data[i]; //set the byte option to the current data entry
        votes[j]++;               // add a vote to indicate that one of this byte has been found
        break;
      }
    }
  }

  //determine which byte got the most votes
  byte correctByte;
  int numIdentical = 0;
  for(int i = 0; i < NUM_FILES; i++){
//    SerialUSB.print("Index ");
//    SerialUSB.print(i);
//    SerialUSB.print(") Votes: ");
//    SerialUSB.print(votes[i]);
//    SerialUSB.print(", byteOption: ");
//    SerialUSB.println((char)byteOptions[i]);
    if(votes[i] > numIdentical){
      numIdentical = votes[i];
      correctByte = byteOptions[i];
    }
  }

  //decide what to do with results
  if(numIdentical == NUM_FILES){ //all bytes are the same return true
    return true;
  }else if(numIdentical > (NUM_FILES/2)){//more than half of the bytes are the same
    writeByteToAll(correctByte, location); //sets all files to the correct byte
    for(int i = 0; i < NUM_FILES; i++){ //sets all bytes of data to be the correct byte
      data[i] = correctByte;
    }
    return true;
  }
  return false; //if it gets here less than half of the bytes are the same- cannot error correct
}

//reads a single byte from a file at a specified location
bool KickSatConfig::readByte(String filename, int location, byte* buf){
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  //byte buf[1]; //byte array to
  File logFileHandle = SD.open(filename, FILE_READ); // Open file for reading
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    //goes to location
    bool validLocation = logFileHandle.seek(location);
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
  return fileStatus; // Returns byte that was read from file

}

//writes a single byte to a file
bool KickSatConfig::writeByte(String filename, byte data, int location){
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  File logFileHandle = SD.open(filename, FILE_WRITE); // Open file for writing
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    //goes to location
    bool validLocation = logFileHandle.seek(location);
    if(!validLocation){
      SerialUSB.println("Error non valid location in file");
      logFileHandle.close();
      endSD();
      return false;
    }
    // Writes data to file
    fileStatus = logFileHandle.print((char)data); // Print to config file on the SD card
    //SerialUSB.print("Wrote: ");
    //SerialUSB.println((char)data); // Print to SerialUSB
    logFileHandle.close(); // Close file
  }
  endSD(); // Ends communication with the SD card
  return fileStatus; // Returns file status

 }

//Initializes a single file with default values- this allows you to skip to the nth line in the file right away,
//if you don't set default values it will give you an error when trying to read from a location in the file that doesn't exist yet
bool KickSatConfig::initFile(String fileName){
  bool fileStatus = false; // Flag to record status of file, returns true if the file can be opened
  startSD(); // Starts communication with the SD card
  File logFileHandle = SD.open(fileName, FILE_WRITE); // Open file for writing
  if (logFileHandle) { // If the file can be opened
    fileStatus = true; // Update flag for file status
    char buf[NUM_ENTRIES]; // Creates a default zero array to write to the file
    for(int i = 0; i < NUM_ENTRIES; i++){
      buf[i] = FLAG_FALSE;  //defaults everything to false- including the status byte
    }
    buf[TIMER_LOC] = (byte) 1; //sets timer to 1 so it can count up from there- Don't set to zero because weird error with a null character
    logFileHandle.seek(0);
    logFileHandle.print(buf); // Print buffer to file on the SD card
    SerialUSB.println(buf); // Print to SerialUSB
    logFileHandle.close(); // Close file
  }
  endSD(); // Ends communication with the SD card
  return fileStatus; // Returns file status
}



//checks whether the files can open and that contents are the same
//creates new file if problem opening old one and populates
bool KickSatConfig::errorCorrect(){
  bool canOpen[NUM_FILES];
  for(int i = 0; i < NUM_FILES; i++){ //initialize to false
    canOpen[i] = false;
  }
  int numTrue = 0;
  for(int i = 0; i < NUM_FILES; i++){
    canOpen[i] = available(configFilenames[i]); //record which files are availile/can open
    if(canOpen[i]){
      numTrue++;
    }else{
      //SerialUSB.println("NOT AVAILABLE");
    }
  }
  if(numTrue == NUM_FILES){
    SerialUSB.print("all files fine ");
    bool contentsFine = errorCorrectContents(); //all files open returns true if contents are the same for all
    SerialUSB.println(contentsFine);
    return contentsFine;
  }else if (numTrue >= (NUM_FILES/2)){ //more than half of the files open but some don't ->  make a new file, reinitialize then copy other files contents into it
    SerialUSB.println("fixable");
    for(int i = 0; i < NUM_FILES; i++){
      if(!canOpen[i]){                     //if file couldn't open
        int dashIndex = configFilenames[i].indexOf('-');
        String newName = configFilenames[i].substring(0, 7); //rename config file
        if(dashIndex == -1){
          newName += '-';
          newName += '1';
        }else{
          int dotIndex = configFilenames[i].indexOf('.');
          int num = configFilenames[i].substring(dashIndex + 1, dotIndex).toInt();
          num++;
          newName += '-';
          newName += num;
        }
        newName += ".txt";
        configFilenames[i] = newName;
        initFile(configFilenames[i]); //reinitialize
      }
    }
    return errorCorrectContents(); //correct contents
  }else{
    //SerialUSB.println("can't open more than half");
    return false;                      //problem opening more than half of the files- Yikes!
  }
}

//checks that the contents of all bytes are the same
bool KickSatConfig::errorCorrectContents(){
  byte buf[NUM_FILES];
  bool result;
  for(int i = 0; i < NUM_ENTRIES; i++){
    result = result && readByteFromAll(buf, i);
  }

  return result;
}
