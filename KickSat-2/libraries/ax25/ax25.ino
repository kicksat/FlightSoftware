#include <Arduino.h>
#include <ax25.h>

#define FLAG 0x7E
#define PID 0xF0 //Protocol Identifier. 0xF0 means : No Layer 3 protocol implemented
#define CONTROL 0x03 //Control Field Type for Unnumbered Information Frames : 0x03
#define MAX_LENGTH 280
#define MAX_LENGTH_FINAL 450

char message[256];
uint8_t encodedMessage[256];
uint8_t encodedlength;

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB);
  SerialUSB.println("Serial Initialized");
  delay(3000);
}

void loop() {

  strcpy(message,"Hello world from the KickSat Team!"); // Message to send
  SerialUSB.println("Message...");
  SerialUSB.println(ax25.message);

  SerialUSB.println("Encoding message...");
  ax25.encodePacket(message);

  SerialUSB.println("Message encoded...");
  for (int i = 0; i < ax25.encodedlength; i++) {
    SerialUSB.print(ax25.encodedMessage[i], BIN);
    SerialUSB.print(" ");
  }
  SerialUSB.println();

  encodedlength = ax25.encodedlength;
  memcpy(encodedMessage,ax25.encodedMessage,ax25.encodedlength);

  SerialUSB.println("Decoding message...");
  ax25.decodePacket(encodedMessage,encodedlength);

  SerialUSB.println("Message decoded...");
  for (int i=0; i < strlen(ax25.decodedMessage); i++) {
    SerialUSB.print(ax25.decodedMessage[i]);
  }
  SerialUSB.println();

  SerialUSB.println();
  delay(2000);

}
