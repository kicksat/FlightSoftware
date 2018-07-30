/* Example main function 
 *  1) wake up after some pre-set amount of time (randomized number)
 *  2) chirp down some prerequested data
 *  3) listen for next instruction
*/

#include "beacon.h"
#include <stdio.h>

#define MAX_OUT_CHARS 16 // max downlink message length

// variables
int sensor_number;
float sensor_data;
// this is an instruction from Ground Station for what to send on next downlink
int next_beacon;
float battery_level;
int log_number;

// create the beacon object
beacon myBeacon = beacon();

void setup() 
{
  // put your setup code here, to run once:
  SerialUSB.begin(115200);
  while(!SerialUSB)
  {
    // wait for SerialUSB connectiong to initialize
  }
  myBeacon.init_beacon();
  next_beacon = 0;  // first message will communicate that the beacon is alive!!
}

void loop() 
{
  // put your main code here, to run repeatedly: DONT TELL ME WHAT TO DO, ARDUINO!!

  // loop of sleep --> wake up --> check battery level until you are charged above threshold level
  while(1)
  {
    // Enter Sleep Mode... 
    delay(60*1000); // sleep for one minute
    // TODO: use arduino low power library for a timed wake up every minute
    // TODO: analog read to find battery level
    // if (battery_level > threshold)
    break;
  }
  
  // Gather data
  // TODO: read the sensors, write to SD card memory
  // TODO: implement sensor reading scheme... we need to read different sensors at different intervals of time

  // Send a basic Chirp
  basic_chirp();

  // listen for an uplink for some command...
  myBeacon.listen();

  // TODO: parse through myBeacon's member variable "uplink_message" which is a char*
  // TODO: parse through and figure out what command was received

  // send a beacon!! 
  switch (next_beacon)
  {
    // initialize
    case 0:
      myBeacon.set_message("Hello, Earthlings");
      myBeacon.send_beacon();
    // Downlink Sensors
    case 1:
      // TODO: read sensor data how you want to read it @Connor @Max
      if(sensor_number == 5)
      {
        sensor_number = 0;
      }
      else
      {
        sensor_number++;
      }
      // TODO: read from this sensor
      sensor_data = 3.14159265;
      // transmit the sensor data, example:
      char buff[MAX_OUT_CHARS + 1];
      sprintf(buff,"S%d: %.2f", sensor_number, sensor_data);   // TODO: verify that we want 2 decimal places of accuracy
      myBeacon.set_message(buff);
      
      myBeacon.send_beacon();
    
    // Downlink Logs
    case 2:
      // TODO: read logs into strings

      // send down the logs... maybe more than one downlink is required here
      
    // Sensor Config rewrite
    case 3:
      // TODO: rewrite the sensor config file based on uplink

    // Reflash the motherboard's code from MRAM
    case 4:
      // TODO: reflash the motherboard's code from MRAM

    // Send mission config
    case 5:
      // TODO: read the mission config and send to GS

    // Enter Arming mode
    case 6:
      // TODO: enter arming mode, adjust flags as neede

    // Enter End of Life Mode
    case 7:
      // TODO: enter end of life mode... 

    // any other commands that we need??
    case 8:

    // no command for a special downlink or other special chore
    default:
      break;
  }

  // Done handling uplink request --> go back to sleep... 
  // TODO: handle going back to sleep... 
  
}


// this will send the basic chirp that is sent every minute or so after waking up... 
void basic_chirp()
{
  char buff[MAX_OUT_CHARS + 1];
  sprintf(buff,"L%d BAT:%.2f", log_number, battery_level);   // TODO: verify that we want 2 decimal places of accuracy
  myBeacon.set_message(buff);
}

