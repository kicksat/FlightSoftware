/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library.

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** MOSI - pin 7
 ** MISO - pin 20
 ** CLK - pin 21
 ** CS - pin 5

*/

#include <SPI.h>
#include <SD.h>
#include "wiring_private.h" // pinPeripheral() function

File root;
const int chipSelect = 5;

void setup() {
  // Open serial communications and wait for port to open:
  SerialUSB.begin(9600);
  delay(5000);

  pinPeripheral(7, PIO_SERCOM_ALT);
  
  SerialUSB.print("Initializing SD card... ");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    SerialUSB.println("Card failed, or not present");
    while (1);
  }
  SerialUSB.println("Card initialized.");

  root = SD.open("/");

  printDirectory(root, 0);

  SerialUSB.println("done!");

  root.close();
  
}

void loop() {

}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      SerialUSB.print('\t');
    }
    SerialUSB.print(entry.name());
    if (entry.isDirectory()) {
      SerialUSB.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      SerialUSB.print("\t\t");
      SerialUSB.println(entry.size(), DEC);
    }
    entry.close();
  }
}








