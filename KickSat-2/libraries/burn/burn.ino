

#include "burn.h"

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB);
  SerialUSB.println("Serial Initialized");
  delay(5000);

  burn.burnAntennaOne();
  burn.burnAntennaTwo();
  burn.burnSpriteOne();
  burn.burnSpriteTwo();
  burn.burnSpriteThree();

}



void loop() {



}
