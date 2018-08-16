#include <RTCCounter.h>

#define LISTENINGDURATION 15000 // milliseconds

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB);
  delay(5000);
}

void loop() {
  output_menu();
  listenForUplink();
  SerialUSB.println();
  delay(2000);
}

///////////////////////
// Listen for uplink //
//////////////////////////////////////////////////////////////////////////////
uint8_t listenForUplink() {
  SerialUSB.println("Listening for uplink...");
  timeout.start(LISTENINGDURATION);
  while(1) { // Wait for uplink, retreive from buffer
    if (SerialUSB.available()) { // TODO: change if radio is available, not serial
      uint8_t command = parseUplink();
      if (command > 0) {
        return command;
      }
    }
    if (timeout.triggered()) { // Checks time for timeout
      SerialUSB.println("Uplink Timeout");
      return 0;
    }
  }
}

uint8_t parseUplink() {
  ////////////////////////////////////////////////////////
  // TODO: change from Serial.read to radio read
  uint8_t command = SerialUSB.read(); // Reads the available char (ASCII)
  command = command - 48; // Convert char to int (ASCII)
  ////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////
  // TODO: Incorporate real checksum check here for input
  if (command < 1 || command > 8) {
    SerialUSB.println("NACK");
    return 0;
  } else {
    SerialUSB.println("ACK");
    return command;
  }
  ////////////////////////////////////////////////////////
}

///////////////////////
// Listen for uplink //
//////////////////////////////////////////////////////////////////////////////
void output_menu() { // TODO: this is an artifact of serial, it gets replaced by the ground station receiving a beacon
  // Listen for Uplink:
  SerialUSB.println("Choose a command from the following menu: ");
  SerialUSB.println("1) Send Sensor Data");
  SerialUSB.println("2) Downlink last 10 Logs");
  SerialUSB.println("3) Rewrite the sensor config files");
  SerialUSB.println("4) Reflash the motherboard's code from MRAM");
  SerialUSB.println("5) Send Mission Config Files");
  SerialUSB.println("6) Enter Arming Mode");
  SerialUSB.println("7) Enter End of Life Mode");
  SerialUSB.println("8) No command");
  SerialUSB.print("Choose a command: ");
}
