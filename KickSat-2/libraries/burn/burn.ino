

#include "burn.h"

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB);
  SerialUSB.println("Serial Initialized");
  delay(5000);



}



void loop() {


}
