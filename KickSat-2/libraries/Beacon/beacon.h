/*  Beacon.h
 *  Sends a packet of info based on user input
 *  This will eventually need to interface with the log files to read all the useful information
 *  Not just pulling random numbers
*/
#ifndef BEACON_h
#define BEACON_h

#include <ax25.h>
#include <RH_RF22.h>
#include <RHGenericDriver.h>
#include <RH_RF22.h>
#include "wiring_private.h" // pinPeripheral() function
#define RH_RF22_HAVE_SERIAL

class beacon {
public:
  // creates a beacon object
  beacon();

  // initializes the beacon object
  void init_beacon();

  // allows you to set the next downlink message
  void set_message(String in_string);

  // sends the message[] member variable as a part of an ax25 packet
  void send_beacon();

  // a function that handles listening mode & sets the uplink_message member variable
  // if an uplink was received
  void listen();

  // number of uplinks received
  int num_received;

  // the message that was received
  char* uplink_message;

  // determines whether or not there was an uplink received
  bool uplink_received;

  // message to be sent in next command
  String message;

  // sequence of stuff received
  byte rcv_sequence[450];

  // size of received buffer
  uint8_t len = sizeof(rcv_sequence);

  // the ax25 radio member variable
  AX25 radio = AX25(SPI_CS_RFM, RF_NIRQ, RF_SDN);
};


#endif
