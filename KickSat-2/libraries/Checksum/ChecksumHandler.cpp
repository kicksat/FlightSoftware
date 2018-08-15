#include "ChecksumHandler.h"

byte ChecksumHandler::calculateChecksum(byte* message, int len) {
  byte val = 0;
  for (int i = 0; i < len; i++) {
    val ^= message[i];
  }
  return val;
}

bool ChecksumHandler::evaluateChecksum(char* message, int len) {
  byte val = 0;
  for (int i = 0; i < len; i++) {
    val ^= message[i];
  }
  return val == 0;
}

ChecksumHandler Checksum;
