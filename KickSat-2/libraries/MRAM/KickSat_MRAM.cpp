/**************************************************************************/
// adapted from adafruit FRAM libraries
// working on MRAM (SPI) from Everspin: https://www.everspin.com/family/mr25h40?npath=259
//
//
// last edited 2018-10-03 M.Holliday
/**************************************************************************/

#include <stdlib.h>
#include <math.h>
#include "KickSat_MRAM.h"


KickSat_MRAM::KickSat_MRAM(int8_t cs){
  _cs = cs;
  _clk = _mosi = _miso = -1;
  _mramInitialised = false;
}

boolean KickSat_MRAM::begin(int8_t cs, uint8_t nAddressSizeBytes){
  _cs = cs;
  setAddressSize(nAddressSizeBytes);
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
  SPI.begin();
  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
  _mramInitialised = true;
  return true;
}

void KickSat_MRAM::writeEnable (bool enable){
  digitalWrite(_cs, LOW);
  if (enable) {
    SPItransfer(OPCODE_WREN);
  }
  else {
    SPItransfer(OPCODE_WRDI);
  }
  digitalWrite(_cs, HIGH);
}

void KickSat_MRAM::write8 (uint32_t addr, uint8_t value){
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_WRITE);
  writeAddress(addr);
  SPItransfer(value);
  digitalWrite(_cs, HIGH);
}

void KickSat_MRAM::write (uint32_t addr, const uint8_t *values, size_t count){
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_WRITE);
  writeAddress(addr);
  for (size_t i = 0; i < count; i++){
    SPItransfer(values[i]);
  }
  digitalWrite(_cs, HIGH);
}

uint8_t KickSat_MRAM::read8 (uint32_t addr){
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_READ);
  writeAddress(addr);
  uint8_t x = SPItransfer(0);
  digitalWrite(_cs, HIGH);
  return x;
}

void KickSat_MRAM::read (uint32_t addr, uint8_t *values, size_t count){
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_READ);
  writeAddress(addr);
  for (size_t i = 0; i < count; i++) {
    uint8_t x = SPItransfer(0);
    values[i] = x;
  }
  digitalWrite(_cs, HIGH);
}

uint8_t KickSat_MRAM::getStatusRegister(void){
  uint8_t reg = 0;
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_RDSR);
  reg = SPItransfer(0);
  digitalWrite(_cs, HIGH);
  return reg;
}

void KickSat_MRAM::setStatusRegister(uint8_t value){
  digitalWrite(_cs, LOW);
  SPItransfer(OPCODE_WRSR);
  SPItransfer(value);
  digitalWrite(_cs, HIGH);
}

void KickSat_MRAM::setAddressSize(uint8_t nAddressSize){
  _nAddressSizeBytes = nAddressSize;
}

uint8_t KickSat_MRAM::SPItransfer(uint8_t x) {
  return SPI.transfer(x);
}

void KickSat_MRAM::writeAddress(uint32_t addr){
  if (_nAddressSizeBytes>3)
  	SPItransfer((uint8_t)(addr >> 24));
  if (_nAddressSizeBytes>2)
  	SPItransfer((uint8_t)(addr >> 16));
  SPItransfer((uint8_t)(addr >> 8));
  SPItransfer((uint8_t)(addr & 0xFF));
}
