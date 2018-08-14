#ifndef AX25_h
#define AX25_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>

#define FLAG 0x7E
#define PID 0xF0 // Protocol Identifier. 0xF0 means : No Layer 3 protocol implemented
#define CONTROL 0x03 // Control Field Type for Unnumbered Information Frames : 0x03
#define MAX_LENGTH 280
#define MAX_LENGTH_FINAL 450
#define MAX_LENGTH_AX25 256
#define CRC_POLYGEN 0x1021 //CRC-CCITT


class AX25 {
public:

  AX25();

  char _SrcCallsign[7];
  char _DestCallsign[7];
  byte _ssid_source;
  byte _ssid_destination;

  byte _bitSequence[280*8];
  byte _finalSequence[MAX_LENGTH_FINAL];
  byte _rcvSequence[MAX_LENGTH_FINAL];
  uint8_t len = sizeof(_rcvSequence);
  char _message[MAX_LENGTH_AX25];

  int _index;
  unsigned int _FCS; // CRC-CCITT



  void setSSIDsource(byte ssid_src);
  void setSSIDdest(byte ssid_dest);
  void setFromCallsign(char *fromcallsign);
  void setToCallsign(char *tocallsign);

  void AddHeader(byte *Buffer);

  byte* encodePacket(char* message);

  void BitProcessing(byte *Buffer, uint8_t bytelength);
  char* decodePacket(byte *Buffer, uint8_t bytelength);
  bool logicXOR(bool a, bool b);
  unsigned int CRC_CCITT (byte *Buffer, uint8_t bytelength);
  byte MSB_LSB_swap_8bit(byte v);
  unsigned int MSB_LSB_swap_16bit(unsigned int v);

};

extern AX25 ax25;

#endif
