#include <SPI.h>
#include <SD.h>
#include "SD_DataFile.h"

#define CS_pin 10

SD_DataFile dataFile(6, "data.txt");

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial.print("Initializing SD card...");
  

  if (!SD.begin(CS_pin)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
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
      //Serial.println(arg[wordIndex]);
      //Serial.println(arg[wordIndex]);
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
  while (Serial.available() > 0) {
    byte inByte = Serial.read();
    message += (char)inByte;
    delay(1);
  }
  String argv[7];
  parseMessage(message, argv);

  if (argv[0] == "r") {
    Serial.print(" === Reading line ");
    Serial.println(argv[1]);
    byte out[dataFile._dataWidth];
    dataFile.readDataEntry(argv[1].toInt(), out);
    for (int i = 0; i < dataFile._dataWidth; i++) {
      Serial.print((char)out[i]);
    }
    Serial.println();
  } else if (argv[0] == "w") {
    Serial.println(" === Writing data");
    byte data[dataFile._dataWidth];
    for (int i = 0; i < dataFile._dataWidth; i++) {
      data[i] = argv[1][i];
    }
    dataFile.writeDataEntry(data);
  } else if (argv[0] == "ra") {
    Serial.println(" === Reading entire file");
    for (int n = 0; n < dataFile._numEntries; n++) {
      byte out[dataFile._dataWidth];
      dataFile.readDataEntry(n, out);
      Serial.print(n);Serial.print(": ");
      for (int i = 0; i < dataFile._dataWidth; i++) {
        Serial.print((char)out[i]);
      }
      Serial.println();
    }
  } else if (argv[0] == "rb") {
    Serial.print(" === Reading bytes from line ");
    Serial.println(argv[1]);
    byte out[dataFile._dataWidth];
    dataFile.readDataEntry(argv[1].toInt(), out);
    for (int i = 0; i < dataFile._dataWidth; i++) {
      Serial.print(out[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void loop() {
  if (Serial.available() > 0) {
    handleCommand();
  }
}
