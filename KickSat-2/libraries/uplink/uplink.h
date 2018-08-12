#include <sd_log.h>

#define LISTENINGDURATION 15 // Defined in seconds

uint8_t parseUplink();
void output_menu();

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

void processUplink(uint8_t command) {

  int logs_to_send = 0;      // for command 2
  String multiple_logs = ""; // for command 2
  String config_string = "";

  // respond to the command
  switch(command)
  {
    // Send down Sensor data
    case 1:
    // send_sensor_data();
    break;
    // Downlink last x number of logs, depends on user input
    case 2:
    // Get some user input for the number of logs to send down
    // Normally this o
    // SerialUSB.println("How many logs shall we send down?");
    // logs_to_send = return_selection();
    // SerialUSB.print("Fetching the last ");
    // SerialUSB.print(logs_to_send);
    // SerialUSB.println(" logs");

    // TODO: @emma read the last logs_to_send logs into the multiple_logs

    // serial_transmit(multiple_logs);

    break;

    // Rewrite the sensor config files @connor @max
    case 3:
    SerialUSB.println("Doing command #3");
    // TODO: make a function that does this...
    break;

    // Reflash the motherboard's code from MRAM @connor @max
    case 4:
    // TODO: make a function that does this...
    SerialUSB.println("Doing command #4");
    break;

    // Send the mission config files
    case 5:
    // TODO: @emma
    // read the mission config files into config_string =
    // serial_transmit(config_string);
    SerialUSB.println("Doing command #5");
    break;

    // Enter arming mode, exit standby mode
    case 6:
    SerialUSB.println("Doing command #6");
    // TODO: enter arming mode
    // send: "Entered arming mode"
    // This is a transition condition in the more general state diagram
    // we will exit standby mode here
    break;

    // Enter End of Life mode
    case 7:
    SerialUSB.println("Doing command #7");
    // Send: ACK, are you sure you want to explode all of our hard work into pixie dust?
    // serial_transmit("Are you sure you want to kill KickSat II? (y/n)");
    // wait for response: if (response != "yes explode" ) --> go back into standby mode
    // command = return_selection();
    // SerialUSB.print("case 7, command: ");
    // SerialUSB.println(command);
    // if((command == 41) || (command == 73))
    // {
    //   // go into end of life mode
    //   // this is a state change into end of life mode
    //   SerialUSB.println("Killing the goddamn 3u that never deserved to live MUAHAHAHAHA!");
    // }
    // else
    // {
    //   // do not kill the satelite
    //
    //   command = 0;
    // }

    break;

    // No command --> go back to sleep and go through another standby mode loop
    case 8:
    //
    SerialUSB.println("Doing no command... going to sleep");
    break;

    // No command --> go back to sleep and go through another standby mode loop
    default:
    SerialUSB.println("Doing no command... going to sleep");
    break;
  }

}
