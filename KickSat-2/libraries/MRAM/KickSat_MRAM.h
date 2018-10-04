/**************************************************************************/
// 4MB MRAM (SPI)
// last edited 2018-10-03 M.Holliday
/**************************************************************************/

#ifndef _KICKSAT_MRAM_H
#define _KICKSAT_MRAM_H

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <SPI.h>

typedef enum opcodes_e
{
  OPCODE_WREN   = 0x06,     /* Write Enable Latch */
  OPCODE_WRDI   = 0x04,     /* Reset Write Enable Latch */
  OPCODE_RDSR   = 0x05,     /* Read Status Register */
  OPCODE_WRSR   = 0x01,     /* Write Status Register */
  OPCODE_READ   = 0x03,     /* Read Memory */
  OPCODE_WRITE  = 0x02,     /* Write Memory */
  OPCODE_RDID   = 0x48,      /* Read Device ID */
  OPCODE_SLEEP  = 0xB9,
  OPCODE_WAKE   = 0xAB
} opcodes_t;

class KickSat_MRAM {
 public:
  KickSat_MRAM(int8_t cs);
  boolean  begin  (int8_t cs, uint8_t nAddressSizeBytes);
  void     writeEnable (bool enable);
  void     write8 (uint32_t addr, uint8_t value);
  void     write (uint32_t addr, const uint8_t *values, size_t count);
  uint8_t  read8  (uint32_t addr);
  void     read (uint32_t addr, uint8_t *values, size_t count);
  uint8_t  getStatusRegister(void);
  void     setStatusRegister(uint8_t value);
  void     setAddressSize(uint8_t nAddressSize);
  void     sleep();
  void     wake();
  bool     MRAMsleeping = false;

 private:
  uint8_t  SPItransfer(uint8_t x);
  void     writeAddress(uint32_t addr);

  boolean _mramInitialised;
  uint8_t  _nAddressSizeBytes;
  int8_t _cs, _clk, _mosi, _miso;
};

#endif
