#include <SPI.h>
#include "KickSat_MRAM.h"

uint8_t MRAM_CS = A2;
KickSat_MRAM mram = KickSat_MRAM(MRAM_CS);

uint8_t addrSizeInBytes = 3; //24 bit address but only uses first 18??

void setup(void) {
  Serial.begin(115200);
  while(!Serial);
  if (mram.begin(MRAM_CS, addrSizeInBytes)) {
    Serial.println("Found MRAM");
  } else {
    Serial.println("NO MRAM found ...");
    while (1);
  }
  uint8_t data[100];
  uint8_t checkDat[100];
  for (uint8_t i = 0; i < 100; i++)  {
    data[i]=i;
  }

//  mram.writeEnable(true);
//  Serial.println("WEL set!");  
//  mram.write(0x001,data, sizeof(data));
//  mram.writeEnable(false);
  mram.read(0x001, checkDat, sizeof(checkDat));
  Serial.println("Reading: 100 bytes to starting at: 0x001");
  for (uint8_t i = 0; i < 100; i++)  {
    Serial.println(checkDat[i], HEX);
  }
  Serial.println("");  
}

void loop(void) {

}
