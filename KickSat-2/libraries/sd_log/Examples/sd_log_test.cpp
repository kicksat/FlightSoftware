
#include "sd_log.h"
#include "SD_DataFile.h"
#include <SD.h>

void setup(){
  SerialUSB.begin(9600);
  delay(2000);
  SD.begin(10);


    sd_log mySd_log;
    mySd_log.logData = {1, (byte)'a', {(byte)'v', (byte)'v', (byte)'v'}, {3.14159, 5454.54, 1111.1}, {1.1, 1.2, 1.3, 2.1, 2.2, 2.3, 3.1, 3.2, 3.3}, {(byte)'c', (byte)'o', (byte)'m', (byte)'m', (byte)'a', (byte)'n', (byte)'d', (byte)'!'}};
    mySd_log.write_log(mySd_log.logData);
    mySd_log.logData = {2, (byte)'b', {(byte)'h', (byte)'i', (byte)'i'}, {32.14159, 4.5554, 1311.1}, {1.6, 1.7, 1.8, 2.4, 2.5, 2.6, 3.5, 3.6, 3.7}, {(byte)'x', (byte)'o', (byte)'m', (byte)'m', (byte)'a', (byte)'n', (byte)'d', (byte)'!'}};
    mySd_log.write_log(mySd_log.logData);

    mySd_log.read_entry(0);
    mySd_log.read_entry(1);

    byte buf[6];
    mySd_log.read_header(0, buf);
    SerialUSB.println("Header data from struct");
    SerialUSB.println(mySd_log.logData.log_num);
    SerialUSB.println(mySd_log.logData.status_byte);
    SerialUSB.println(mySd_log.logData.power_data[0]);
    SerialUSB.println(mySd_log.logData.power_data[1]);
    SerialUSB.println(mySd_log.logData.power_data[2]);

    mySd_log.read_header(1, buf);
    SerialUSB.println("Header data from struct");
    SerialUSB.println(mySd_log.logData.log_num);
    SerialUSB.println(mySd_log.logData.status_byte);
    SerialUSB.println(mySd_log.logData.power_data[0]);
    SerialUSB.println(mySd_log.logData.power_data[1]);
    SerialUSB.println(mySd_log.logData.power_data[2]);

    mySd_log.read_GPS(0);
    SerialUSB.println("GPS data from struct");
    SerialUSB.println(mySd_log.logData.GPS_data[0]);
    SerialUSB.println(mySd_log.logData.GPS_data[1]);
    SerialUSB.println(mySd_log.logData.GPS_data[2]);
    SerialUSB.println(mySd_log.logData.GPS_data[3]);

    mySd_log.read_GPS(1);
    SerialUSB.println("GPS data from struct");
    SerialUSB.println(mySd_log.logData.GPS_data[0]);
    SerialUSB.println(mySd_log.logData.GPS_data[1]);
    SerialUSB.println(mySd_log.logData.GPS_data[2]);
    SerialUSB.println(mySd_log.logData.GPS_data[3]);

    mySd_log.read_IMU(0);
    SerialUSB.println("IMU data from struct");
    SerialUSB.println(mySd_log.logData.IMU_data[0]);
    SerialUSB.println(mySd_log.logData.IMU_data[1]);
    SerialUSB.println(mySd_log.logData.IMU_data[2]);
    SerialUSB.println(mySd_log.logData.IMU_data[3]);
    SerialUSB.println(mySd_log.logData.IMU_data[4]);
    SerialUSB.println(mySd_log.logData.IMU_data[5]);
    SerialUSB.println(mySd_log.logData.IMU_data[6]);
    SerialUSB.println(mySd_log.logData.IMU_data[7]);
    SerialUSB.println(mySd_log.logData.IMU_data[8]);

    mySd_log.read_IMU(1);
    SerialUSB.println("IMU data from struct");
    SerialUSB.println(mySd_log.logData.IMU_data[0]);
    SerialUSB.println(mySd_log.logData.IMU_data[1]);
    SerialUSB.println(mySd_log.logData.IMU_data[2]);
    SerialUSB.println(mySd_log.logData.IMU_data[3]);
    SerialUSB.println(mySd_log.logData.IMU_data[4]);
    SerialUSB.println(mySd_log.logData.IMU_data[5]);
    SerialUSB.println(mySd_log.logData.IMU_data[6]);
    SerialUSB.println(mySd_log.logData.IMU_data[7]);
    SerialUSB.println(mySd_log.logData.IMU_data[8]);

}

void loop(){

}


//////////////////////////////////////////////////////////////////////////////////////////
//
//
//#include <SPI.h>
//#include <SD.h>
//#include "SD_DataFile.h"
//
//#define CS_pin 10
//
//SD_DataFile dataFile(126, "data.txt");
//
//void setup() {
//  SerialUSB.begin(115200);
//  while (!SerialUSB) {}
//
//  SerialUSB.print("Initializing SD card...");
//
//
//  if (!SD.begin(CS_pin)) {
//    SerialUSB.println("initialization failed!");
//    return;
//  }
//  SerialUSB.println("initialization done.");
//  dataFile.refresh();
//}
//
////helper function for handleCommand
//void parseMessage(String msg, String arg[]) {
//  int index = 0;
//
//  int wordIndex = 0;
//  int wordStart = 0;
//  while (index < msg.length()) {
//    if (msg.charAt(index) == ' ') {
//      arg[wordIndex] = msg.substring(wordStart, index);
//      //SerialUSB.println(arg[wordIndex]);
//      //SerialUSB.println(arg[wordIndex]);
//      wordStart = index + 1;
//      wordIndex++;
//    }
//    index++;
//  }
//  arg[wordIndex] = msg.substring(wordStart, index);
//}
//
////debugging function
////allows the user to push commands to the micro through the SerialUSB monitor
////in this case, we have 3 commands
////
////r <value> - reads the data entry specified by value
////ri <lineNum index len> - reads at an index in a line
////w <data> - writes the bytes in data to the next entry of the file
////ra - reads out the entire file
//void handleCommand() {
//  String message = "";
//  while (SerialUSB.available() > 0) {
//    byte inByte = SerialUSB.read();
//    message += (char)inByte;
//    delay(1);
//  }
//  String argv[7];
//  parseMessage(message, argv);
//
//  if (argv[0] == "r") {
//    SerialUSB.print(" === Reading line ");
//    SerialUSB.println(argv[1]);
//    byte out[dataFile._dataWidth];
//    dataFile.readDataEntry(argv[1].toInt(), out);
//    for (int i = 0; i < dataFile._dataWidth; i++) {
//      SerialUSB.print((char)out[i]);
//    }
//    SerialUSB.println();
//  } else if (argv[0] == "w") {
//    SerialUSB.println(" === Writing data");
//    byte data[dataFile._dataWidth];
//    for (int i = 0; i < dataFile._dataWidth; i++) {
//      data[i] = argv[1][i];
//    }
//    dataFile.writeDataEntry(data);
//  } else if (argv[0] == "ra") {
//    SerialUSB.println(" === Reading entire file");
//    for (int n = 0; n < dataFile._numEntries; n++) {
//      byte out[dataFile._dataWidth];
//      dataFile.readDataEntry(n, out);
//      SerialUSB.print(n);SerialUSB.print(": ");
//      for (int i = 0; i < dataFile._dataWidth; i++) {
//        SerialUSB.print((char)out[i]);
//      }
//      SerialUSB.println();
//    }
//  } else if (argv[0] == "rb") {
//    SerialUSB.print(" === Reading bytes from line ");
//    SerialUSB.println(argv[1]);
//    byte out[dataFile._dataWidth];
//    dataFile.readDataEntry(argv[1].toInt(), out);
//    for (int i = 0; i < dataFile._dataWidth; i++) {
//      SerialUSB.print(out[i]);
//      SerialUSB.print(" ");
//    }
//    SerialUSB.println();
//  }else if (argv[0] == "ri"){
//    SerialUSB.print("reading index ");
//    byte out[argv[3].toInt()];
//    dataFile.readLineIndex(argv[1].toInt(), argv[2].toInt(), argv[3].toInt(), out);
//    for (int i = 0; i < argv[3].toInt(); i++) {
//      SerialUSB.print((char)out[i]);
//      SerialUSB.print(" ");
//    }
//    SerialUSB.println();
//  }
//}
//
//void loop() {
//  if (SerialUSB.available() > 0) {
//    handleCommand();
//  }
//}
//
