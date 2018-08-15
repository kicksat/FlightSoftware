#include "ChecksumHandler.h"

//this function calculates a checksum byte based on a buffer of bytes passed in
//returns the checksum byte calculated
byte ChecksumHandler::calculateChecksum(byte* message, int len) {
  byte val = 0;
  for (int i = 0; i < len; i++) {
    val ^= message[i];
  }
  return val;
}

//this function checks a buffer containing a checksum
//assumes that the last byte of the buffer is the checksum byte
//if the checksum passes, returns true
//returns false otherwise
bool ChecksumHandler::evaluateChecksum(byte* message, int len) {
  byte val = 0;
  for (int i = 0; i < len; i++) {
    val ^= message[i];
  }
  return val == 0;
}

ChecksumHandler Checksum;
