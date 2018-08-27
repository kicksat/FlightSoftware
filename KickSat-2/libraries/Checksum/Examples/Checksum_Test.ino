/*
 * ChecksumHandler test - test the function of the ckecksum handler
 *                        the code introduces an error in a message, so the evaluateChecksum function should return 0
 *                        change line 27 to alter the introduce error in the message, or remove the error to have evaluateCkechsum return 1
 */

#include "ChecksumHandler.h"

void setup() {
  SerialUSB.begin(115200);
  while (!SerialUSB) {}
  SerialUSB.println("SerialUSB Connected.");
}

void loop() {
  //set up checksum test
  int before, after, result;
  String message = "Ralen sucks lol";
  byte data[message.length() + 1];
  strToBytes(message, data);

  //calculate checksum
  before = Checksum.calculateChecksum(data, message.length());
  data[message.length()] = before;

  //introduce any errors you want to test
  data[4] ^= 0x4;

  //validate checksum
  after = Checksum.calculateChecksum(data, message.length());
  result = Checksum.evaluateChecksum(data, message.length() + 1);

  //print results
  SerialUSB.print("Checksum: ");
  SerialUSB.println(before);
  SerialUSB.print("After Error: ");
  SerialUSB.println(after);
  SerialUSB.println(result);
}

void strToBytes(String msg, byte* data) {
  for (int i = 0; i < msg.length(); i++) {
    data[i] = msg[i];
  }
}
