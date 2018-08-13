#include "Checksum.h"

Checksum checksumHandler;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Serial Connected.");
}

void loop() {
  //set up checksum test
  int before, after, result;
  String message = "Ralen sucks lol";
  byte data[message.length() + 1];
  strToBytes(message, data);

  //calculate checksum
  before = checksumHandler.calculateChecksum(data, message.length());
  data[message.length()] = before;

  //introduce any errors you want to test
  data[4] ^= 0x4;

  //validate checksum
  after = checksumHandler.calculateChecksum(data, message.length());
  result = checksumHandler.evaluateChecksum(data, message.length() + 1);

  //print results
  Serial.print("Checksum: ");
  Serial.println(before);
  Serial.print("After Error: ");
  Serial.println(after);
  Serial.println(result);
}

void strToBytes(String msg, byte* data) {
  for (int i = 0; i < msg.length(); i++) {
    data[i] = msg[i];
  }
}
