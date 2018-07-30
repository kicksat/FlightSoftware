/*  Beacon.cpp
 *  implements the beacon functions
 *  
*/
#include "beacon.h"
#define MAX_OUT_CHARS 16 // TODO: edit this

// creates a beacon object
beacon::beacon()
{
  SerialUSB.println("Creating a beacon object...");
}

// initializes the beacon object
void beacon::init_beacon()
{
  SerialUSB.println("Initializing the beacon...");
  // declare MOSI as alternate
  pinPeripheral(7, PIO_SERCOM_ALT);
  radio.powerAndInit();
  pinMode(RF_SDN, OUTPUT);
  message = "Beacon Initialized";
  SerialUSB.println("Initialization complete");
}

// allows you to set the next downlink message 
void beacon::set_message(String in_string)
{
  message = in_string;
}

// sends the message[] member variable as a part of an ax25 packet
void beacon::send_beacon()
{
  // do we have to set Tx mode??
  SerialUSB.println("converting string to char array");
  int n = message.length(); 
  // declaring character array
  char char_arr[n+1]; 
  // copying the contents of the 
  // string to char array
  strcpy(char_arr, message.c_str()); 

  // Send the message
  SerialUSB.println("Sending the message");
  radio.transmit(char_arr, sizeof(char_arr));
  SerialUSB.println("The message has been sent");
}

// a function that handles listening mode & sets the uplink_message member variable
  // if an uplink was received
void beacon::listen()
{
  int listen_time = 15; // time to listen for a chirp (in seconds)
  // do we have to set Rx mode??

  // create the buffer to fill with a received message!!
  char buff[MAX_OUT_CHARS + 1];  

  // check for a reply
  if(radio.radio.waitAvailableTimeout(1000*listen_time)) // this goes ax25 --> RH_RF22 --> waitTimeoutAvailable()
  {
    uplink_received = true;
    if(radio.receive(rcv_sequence, &len))
    {
      ++num_received;
      SerialUSB.print("got reply number ");
      SerialUSB.println(num_received);
      
      for(int i = 0; i < len; i++)
      {
        SerialUSB.println(rcv_sequence[i]);
      }
      // uplink message is a char*
      // TODO: turn this into a string so that you can parse through this and find out what was said...
      uplink_message = radio.demod(rcv_sequence, len);
    }
  }
  else // no message received
  {
    uplink_received = false;
    uplink_message = ""; // no message 
  }
  
}

/* member variables 
// message to be sent in next command
char message[];

// this is an instruction from Ground Station for what to send on next downlink
short int next_beacon;
*/

