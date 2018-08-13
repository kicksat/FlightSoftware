// Checksum.h - Library for checksums

#ifndef CHECKSUM_h
#define CHECKSUM_h

#include <Arduino.h>

class Checksum {
  
  public:
    byte calculateChecksum(byte* message, int len);
    bool evaluateChecksum(byte* message, int len);
    
};

#endif
