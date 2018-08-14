#include "ax25.h"

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB);
  SerialUSB.println("Serial Initialized");
  delay(500);
  char message[256];
  byte* encodedMessage;
  char* decodedMessage;
  strcpy(message,"Hello world from the KickSat team!");
  SerialUSB.print("Message:"); SerialUSB.println(message);
  SerialUSB.println("Encoding message...");
  encodedMessage = ax25.encodePacket(message);
  SerialUSB.println("Message encoded...");
  for (int i = 0; i < 450; i++) {
    SerialUSB.print(encodedMessage[i], BIN);
    SerialUSB.print(" ");
  }
  decodedMessage = ax25.decodePacket(encodedMessage,450);
  SerialUSB.println("Final decoded Message");
  for (int i=0; i < strlen(decodedMessage); i++) {
    SerialUSB.print(decodedMessage[i]);
  }
}

void loop() {

}
