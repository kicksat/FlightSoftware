#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ax25.h"

AX25::AX25() {
  strcpy(_SrcCallsign,"KD2BHC");
  strcpy(_DestCallsign,"CQ    ");
  _ssid_source = 0x60;
  _ssid_destination = 0x61;
  _index = 0;
  _FCS = 0;
}

void AX25::setSSIDsource(byte ssid_src) {
  _ssid_source = ssid_src;
}
void AX25::setSSIDdest(byte ssid_dest) {
  _ssid_destination = ssid_dest;
}
void AX25::setFromCallsign(char *fromcallsign) {
  strcpy(_SrcCallsign,fromcallsign);
}
void AX25::setToCallsign(char *tocallsign) {
  strcpy(_DestCallsign,tocallsign);
}

byte* AX25::encodePacket(char* message) {
  _index = 0;

  //Array Initialization
  for (int i=0; i< MAX_LENGTH * 8 ;i++) _bitSequence[i] = 0;
  for (int i=0; i< MAX_LENGTH_FINAL ;i++) _finalSequence[i] = 0;
  for (int i=0; i< MAX_LENGTH_FINAL ;i++) _rcvSequence[i] = 0;
  for (int j=0; j< MAX_LENGTH_AX25 ;j++) strcpy(_message," ");

  //Set CallSign and SSID
  setFromCallsign(_SrcCallsign);
  setToCallsign(_DestCallsign);
  setSSIDdest(_ssid_destination);
  setSSIDsource(_ssid_source);

  //MESSAGE to transmit
  strcpy(_message, message);

  //----------------------- Start Ax25 Packet format ----------------------//

  //Add Header
  AddHeader(_bitSequence);

  //Add Message
  for (int i=0; i < strlen(_message) ; i++) _bitSequence[_index++] = _message[i];

  //Convert bit sequence from MSB to LSB
  for (int i=0; i < _index ; i++) _bitSequence[i] = MSB_LSB_swap_8bit(_bitSequence[i]);

  //Compute Frame check sequence : CRC
  _FCS = CRC_CCITT(_bitSequence, _index);

  //Add _FCS in MSB form
  //Add MS byte
  _bitSequence[_index++] = (_FCS >> 8) & 0xff;
  //Add LS byte
  _bitSequence[_index++] = _FCS & 0xff;

  //radio.printBuffer("Init Message:", _bitSequence, _index);

  //Bit Processing...Bit stuff, add FLAG and do NRZI enconding...
  BitProcessing(_bitSequence,_index);

  return _finalSequence;
}

void AX25::AddHeader(byte *Buffer) {

  //Shift bits 1 place to the left in order to allow for HDLC extension bit
  for (int i=0; i < strlen(_DestCallsign) ; i++) Buffer[_index++] = _DestCallsign[i]<<1;

  // Append SSID Destination
  Buffer[_index++] = _ssid_destination;

  //Append Source Callsign
  for (int i=0; i < strlen(_SrcCallsign) ; i++) Buffer[_index++] = _SrcCallsign[i]<<1;

  //Append SSID Source
  Buffer[_index++] = _ssid_source;

  //Append Control bits
  Buffer[_index++] = CONTROL;

  //Append Protocol Identifier
  Buffer[_index++] = PID;
}

void AX25::BitProcessing(byte *Buffer, uint8_t bytelength) {

  byte BitSequence[bytelength*8+1];
  byte BitSequenceStuffed[bytelength*8+bytelength*8/5+1];
  int k = 0; //general counter
  int _size = 0;
  int s = 0; //stuffed sequence counter
  uint8_t cnt = 0 ;//Bit stuff counter
  uint8_t remBits = 0;
  byte temp = 0;
  byte byte_temp[255*8];//max _message lenght 255 bytes

  k = 0;
  //Convert bits to byte size
  for (int i = 0; i< bytelength ; i++)
  {
    for (register uint8_t t=128; t>0 ; t = t/2) {
      if (Buffer[i] & t) BitSequence[k++] = 0x01;
      else BitSequence[k++] = 0x00;
    }
  }

  // stuff a 0 after five consecutive 1s.
  for (int i = 0; i < k ; i++)
  {
    if (BitSequence[i] == 0x01) cnt++;
    else cnt = 0; // restart count at 1

    BitSequenceStuffed[s++] = BitSequence[i]; // add the bit to the final sequence

    if (cnt == 5) // there are five consecutive bits of the same value
    {
      BitSequenceStuffed[s++] = 0x00; // stuff with a zero bit
      cnt = 0; // and reset cnt to zero
    }
  }

  _size = 0;
  //Recreate 0b01111110 (FLAG) in byte size
  for (int i=0; i < 64 ; i++)
  {
    Buffer[_size++] = 0x00;
    for (int j=0; j < 6 ; j++)
    {
      Buffer[_size++] = 0x01;
    }
    Buffer[_size++] = 0x00;
  }

  for (int i=0; i < s ; i++) Buffer[_size++] = BitSequenceStuffed[i];

  //Insert 0b01111110 (FLAG)
  Buffer[_size++] = 0x00;
  for (int j=0; j < 6 ; j++)
  {
    Buffer[_size++] = 0x01;
  }
  Buffer[_size++] = 0x00;

  for (int i = 0; i< 255*8 ; i++) byte_temp[i] = 0x00;

  //NRZI encoding
  for (int i=0; i < _size ; i++)
  {
    if (Buffer[i] == 0x00)
    {
      byte_temp[i+1] = ! byte_temp[i];
    }
    else
    {
      byte_temp[i+1] = byte_temp[i];
    }
  }

  //extrabits = (_size+1) % 8;
  if (((_size+1) % 8) > 0) remBits = 8 - ((_size+1) % 8);

  for (int i = (_size + 1) ; i < (_size + 1 + remBits ) ; i++)
  {
    byte_temp[i] = 0x01;
  }

  //Convert to bit after NRZI and added remaining bits to form byte array
  _index = 0;
  for (int i = 0; i < (_size + 1 + remBits); i = i + 8)
  {
    temp = 0;
    if  (byte_temp[i] == 0x01)   temp = temp + 0b10000000;
    if  (byte_temp[i+1] == 0x01) temp = temp + 0b01000000;
    if  (byte_temp[i+2] == 0x01) temp = temp + 0b00100000;
    if  (byte_temp[i+3] == 0x01) temp = temp + 0b00010000;
    if  (byte_temp[i+4] == 0x01) temp = temp + 0b00001000;
    if  (byte_temp[i+5] == 0x01) temp = temp + 0b00000100;
    if  (byte_temp[i+6] == 0x01) temp = temp + 0b00000010;
    if  (byte_temp[i+7] == 0x01) temp = temp + 0b00000001;
    _finalSequence[_index++] = temp;
  }
}

char* AX25::decodePacket(byte *Buffer, uint8_t bytelength) {

  byte BitSequence[bytelength*8];
  byte ByteSequence[bytelength];
  byte BitSequence_temp[bytelength*8];
  byte ByteSequence_temp[bytelength];
  char Message[MAX_LENGTH_AX25];
  byte Checksum[3];
  char DestCS[7];
  char SourceCS[7];
  int k = 0; //general counter
  int _size = 0;
  int s = 0;
  uint8_t cnt = 0 ;//Bit stuff counter
  uint8_t extraBit = 0;
  uint8_t extraByte = 0;
  byte temp = 0;
  boolean pastFlag;
  boolean BitFound;

  //Initialization
  for (int i=0; i < bytelength*8 ; i++) BitSequence[i] = 0x00;
  for (int i=0; i < bytelength*8 ; i++) BitSequence_temp[i] = 0x00;
  for (int i=0; i < bytelength ; i++) ByteSequence[i] = 0x00;
  for (int i=0; i < bytelength ; i++) ByteSequence_temp[i] = 0x00;

  //Convert bits to byte size
  for (int i = 0; i< bytelength ; i++)
  {
    for (register uint8_t t=128; t>0 ; t = t/2) {
      if (Buffer[i] & t) BitSequence[_size++] = 0x01;
      else BitSequence[_size++] = 0x00;
    }
  }

  for (int i=1; i < _size ; i++)
  {
    if (BitSequence[i] == BitSequence[i-1])
    {
      BitSequence_temp[i-1] = 0x01;
    } else BitSequence_temp[i-1] = 0x00;

  }

  //Convert bit to Byte
  k = 0;
  for (int i = 0; i < _size-1; i = i + 8)
  {
    temp = 0;
    if  (BitSequence_temp[i] == 0x01)   temp = temp + 0b10000000;
    if  (BitSequence_temp[i+1] == 0x01) temp = temp + 0b01000000;
    if  (BitSequence_temp[i+2] == 0x01) temp = temp + 0b00100000;
    if  (BitSequence_temp[i+3] == 0x01) temp = temp + 0b00010000;
    if  (BitSequence_temp[i+4] == 0x01) temp = temp + 0b00001000;
    if  (BitSequence_temp[i+5] == 0x01) temp = temp + 0b00000100;
    if  (BitSequence_temp[i+6] == 0x01) temp = temp + 0b00000010;
    if  (BitSequence_temp[i+7] == 0x01) temp = temp + 0b00000001;
    ByteSequence[k++] = temp;
  }
  //Test
  //    radio.printBuffer("NRZI:", ByteSequence, k);

  pastFlag = false;
  cnt = 0;
  //Find and Remove Flags
  for (int i = 0; i < k; i++)
  {
    if (ByteSequence[i] != FLAG)
    {
      pastFlag = true;
      ByteSequence_temp[cnt++] = ByteSequence[i];
    } else if (pastFlag) break;
  }

  //Re-init
  for (int i=0; i < bytelength*8 ; i++) BitSequence[i] = 0x00;
  k = 0;

  //Convert bits to byte size
  for (int i = 0; i< cnt ; i++)
  {
    for (register uint8_t t=128; t>0 ; t = t/2) {
      if (ByteSequence_temp[i] & t) BitSequence[k++] = 0x01;
      else BitSequence[k++] = 0x00;
    }
  }

  //Re-init
  for (int i=0; i < bytelength*8 ; i++) BitSequence_temp[i] = 0x00;

  //Bit unstuff : Remove 0 after five consecutive 1s.
  cnt = 0;
  s = 0;
  BitFound = false;
  extraBit = 0;

  for (int i = 0; i < k ; i++)
  {
    if (BitFound)
    {
      BitFound = false;
      extraBit++;
      continue;
    }

    if (BitSequence[i] == 0x01) cnt++;
    else cnt = 0; // restart count at 1

    if (cnt == 5) // there are five consecutive bits of the same value
    {
      BitFound = true;
      cnt = 0; // and reset cnt to zero
    }
    BitSequence_temp[s++] = BitSequence[i]; // add the bit to the final sequence
  }

  extraByte = (extraBit / 8);
  if ( ((extraBit) % 8) > 0) extraByte++ ;

  //Re-init ByteSequence
  for (int i=0; i < bytelength ; i++) ByteSequence[i] = 0x00;
  //Convert bit to Byte
  k = 0;
  for (int i = 0; i < s - extraByte*8; i = i + 8)
  {
    temp = 0;
    if  (BitSequence_temp[i] == 0x01)   temp = temp + 0b10000000;
    if  (BitSequence_temp[i+1] == 0x01) temp = temp + 0b01000000;
    if  (BitSequence_temp[i+2] == 0x01) temp = temp + 0b00100000;
    if  (BitSequence_temp[i+3] == 0x01) temp = temp + 0b00010000;
    if  (BitSequence_temp[i+4] == 0x01) temp = temp + 0b00001000;
    if  (BitSequence_temp[i+5] == 0x01) temp = temp + 0b00000100;
    if  (BitSequence_temp[i+6] == 0x01) temp = temp + 0b00000010;
    if  (BitSequence_temp[i+7] == 0x01) temp = temp + 0b00000001;
    ByteSequence[k++] = temp;
  }

  // SerialUSB.println("Received Stream");
  // for (int i=0 ; i < k; i++) SerialUSB.print(ByteSequence[i],HEX);
  // SerialUSB.println("");
  // for (int i=0 ; i < k; i++) SerialUSB.print(ByteSequence[i]);
  // SerialUSB.println("");

  //Check if _message has errors
  //Compute _FCS on received byte stream
  _FCS = 0;
  _FCS = CRC_CCITT(ByteSequence, k-2);

  Checksum[1] = ByteSequence[k-2];
  Checksum[2] = ByteSequence[k-1];

  // SerialUSB.println("Checksums : ");
  // SerialUSB.println(Checksum[1],HEX);
  // SerialUSB.println(Checksum[2],HEX);
  // SerialUSB.println("_FCS in LSB: ");
  // SerialUSB.println(_FCS,HEX);
  // SerialUSB.println("Checksums computed: ");
  // SerialUSB.print((_FCS >> 8) & 0xff,HEX);
  // SerialUSB.println(_FCS & 0xff,HEX);

  if (Checksum[1] != ((_FCS >> 8) & 0xff))
  {
    SerialUSB.println("Error in Checksum 1 : ");
    SerialUSB.print(Checksum[1]);SerialUSB.print(" != ");SerialUSB.println((_FCS >> 8) & 0xff);
  }
  if (Checksum[2] != (_FCS & 0xff))
  {
    SerialUSB.println("Error in Checksum 2: ");
    SerialUSB.print(Checksum[2]);SerialUSB.print(" != ");SerialUSB.println(_FCS & 0xff);
  }

  //Convert form LSB to MSB
  for (int i=0; i < bytelength ; i++) ByteSequence_temp[i] = 0x00;
  for (int i=0; i < k-2 ; i++) ByteSequence_temp[i] = MSB_LSB_swap_8bit(ByteSequence[i]);

  cnt = 0;
  //Recover header
  for (int i=0; i < 6; i++) DestCS[i] = char(ByteSequence_temp[cnt++]>>1);

  //SSID Destination
  cnt++;

  //Append Source Callsign
  for (int i=0; i < 6; i++) SourceCS[i] = char(ByteSequence_temp[cnt++]>>1);

  //Append SSID Source
  cnt++;
  //Append Control bits
  cnt++;
  //Append Protocol Identifier
  cnt++;
  //Recover _message
  s = k-2-cnt;
  // SerialUSB.println("Final decoded Message");
  for (int i=0; i < s; i++) {
    Message[i] = char(ByteSequence_temp[cnt++]);
    // SerialUSB.print(Message[i]);
  }
  SerialUSB.println("");
  return Message;
}

boolean AX25::logicXOR(boolean a, boolean b) {
  return (a||b) && !(a && b);
}

unsigned int AX25::CRC_CCITT (byte *Buffer, uint8_t bytelength) {
  uint8_t OutBit = 0;
  unsigned int XORMask = 0x0000;
  unsigned int SR = 0xFFFF;

  for (int i=0; i<bytelength ; i++)
  {
    for (uint8_t b = 128 ; b > 0 ; b = b/2) {

      OutBit = SR & 1 ? 1 : 0; //Bit shifted out of shift register

      SR = SR>>1; // Shift the register to the right and shift a zero in

      XORMask = logicXOR((Buffer[i] & b),OutBit) ? MSB_LSB_swap_16bit(CRC_POLYGEN) : 0x0000;

      SR = SR ^ XORMask;
    }
  }
  return  MSB_LSB_swap_16bit(~SR);
}

byte AX25::MSB_LSB_swap_8bit(byte v) {
  // swap odd and even bits
  v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);
  // swap consecutive pairs
  v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);
  // swap nibbles ...
  v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4);
  return v;
}

unsigned int AX25::MSB_LSB_swap_16bit(unsigned int v) {
  // swap odd and even bits
  v = ((v >> 1) & 0x5555) | ((v & 0x5555) << 1);
  // swap consecutive pairs
  v = ((v >> 2) & 0x3333) | ((v & 0x3333) << 2);
  // swap nibbles ...
  v = ((v >> 4) & 0x0F0F) | ((v & 0x0F0F) << 4);
  // swap bytes
  v = ((v >> 8) & 0x00FF) | ((v & 0x00FF) << 8);
  return v;
}

AX25 ax25;
