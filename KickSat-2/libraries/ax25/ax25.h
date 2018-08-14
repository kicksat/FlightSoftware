#ifndef AX25_h
#define AX25_h

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

  uint8_t encodedlength;
  uint8_t decodedlength;
  char message[MAX_LENGTH_AX25];
  uint8_t encodedMessage[MAX_LENGTH_FINAL];
  char decodedMessage[MAX_LENGTH_FINAL];

  AX25();
  void encodePacket(char* message);
  void decodePacket(uint8_t *Buffer, uint8_t uint8_tlength);

private:
  uint8_t _bitSequence[280*8];
  uint8_t _finalSequence[MAX_LENGTH_FINAL];
  uint8_t _rcvSequence[MAX_LENGTH_FINAL];
  uint8_t len = sizeof(_rcvSequence);
  char _SrcCallsign[7];
  char _DestCallsign[7];
  uint8_t _ssid_source;
  uint8_t _ssid_destination;
  int _index;
  unsigned int _FCS; // CRC-CCITT

  void setSSIDsource(uint8_t ssid_src);
  void setSSIDdest(uint8_t ssid_dest);
  void setFromCallsign(char *fromcallsign);
  void setToCallsign(char *tocallsign);

  void AddHeader(uint8_t *Buffer);
  uint8_t BitProcessing(uint8_t *Buffer, uint8_t uint8_tlength);
  bool logicXOR(bool a, bool b);
  unsigned int CRC_CCITT (uint8_t *Buffer, uint8_t uint8_tlength);
  uint8_t MSB_LSB_swap_8bit(uint8_t v);
  unsigned int MSB_LSB_swap_16bit(unsigned int v);
};

extern AX25 ax25;

#endif
