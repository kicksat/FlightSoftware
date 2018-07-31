#include <SPI.h>
#include <SD.h>
#include "SD_DataFile.h"

#define CS_pin 10

SD_DataFile dataFile(126, "dipple.txt");

void setup() {
  SerialUSB.begin(115200);
  while (!Serial) {}
  

  SerialUSB.print("Initializing SD card...");
  

  if (!SD.begin(CS_pin)) {
    SerialUSB.println("initialization failed!");
    return;
  }
  SerialUSB.println("initialization done.");
  dataFile.refresh();
}

//helper function for handleCommand
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

//debugging function
//allows the user to push commands to the micro through the serial monitor
//in this case, we have 3 commands
//
//r <value> - reads the data entry specified by value
//w <data> - writes the bytes in data to the next entry of the file
//ra - reads out the entire file
void handleCommand() {
  String message = "";
  while (SerialUSB.available() > 0) {
    byte inByte = SerialUSB.read();
    message += (char)inByte;
    delay(1);
  }
  String argv[7];
  parseMessage(message, argv);

  if (argv[0] == "r") {
    SerialUSB.print(" === Reading line ");
    SerialUSB.println(argv[1]);
    byte out[dataFile._dataWidth];
    dataFile.readDataEntry(argv[1].toInt(), out);
    for (int i = 0; i < dataFile._dataWidth; i++) {
      SerialUSB.print((char)out[i]);
    }
    SerialUSB.println();
  } else if (argv[0] == "w") {
    SerialUSB.println(" === Writing data");
    byte data[dataFile._dataWidth];
    for (int i = 0; i < dataFile._dataWidth; i++) {
      data[i] = argv[1][i];
    }
    dataFile.writeDataEntry(data);
  } else if (argv[0] == "ra") {
    SerialUSB.println(" === Reading entire file");
    for (int n = 0; n < dataFile._numEntries; n++) {
      byte out[dataFile._dataWidth];
      dataFile.readDataEntry(n, out);
      SerialUSB.print(n);SerialUSB.print(": ");
      for (int i = 0; i < dataFile._dataWidth; i++) {
        SerialUSB.print((char)out[i]);
      }
      SerialUSB.println();
    }
  } else if (argv[0] == "rb") {
    SerialUSB.print(" === Reading bytes from line ");
    SerialUSB.println(argv[1]);
    byte out[dataFile._dataWidth];
    dataFile.readDataEntry(argv[1].toInt(), out);
    for (int i = 0; i < dataFile._dataWidth; i++) {
      SerialUSB.print(out[i]);
      SerialUSB.print(" ");
    }
    SerialUSB.println();
  } else if (argv[0] == "ri"){
    SerialUSB.print(" === Reading ");
    SerialUSB.print(argv[3].toInt());
    SerialUSB.print(" bytes from line ");
    SerialUSB.print(argv[1].toInt());
    SerialUSB.print(" at index ");
    SerialUSB.println(argv[2].toInt());
    byte out[argv[3].toInt()];
    dataFile.readLineIndex(argv[1].toInt(), argv[2].toInt(), argv[3].toInt(), out);
    for (int i = 0; i < argv[3].toInt(); i++) {
      SerialUSB.print((char)out[i]);
    }
    SerialUSB.println();
  }
}

void loop() {
  if (SerialUSB.available() > 0) {
    handleCommand();
  }
}
