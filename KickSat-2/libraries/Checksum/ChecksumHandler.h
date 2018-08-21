// ChecksumHandler.h - Library for checksums

#ifndef CHECKSUMHANDLER_h
#define CHECKSUMHANDLER_h

#include <Arduino.h>

class ChecksumHandler {

  public:
    byte calculateChecksum(byte* message, int len);
    bool evaluateChecksum(byte* message, int len);

};

extern ChecksumHandler Checksum;

#endif
