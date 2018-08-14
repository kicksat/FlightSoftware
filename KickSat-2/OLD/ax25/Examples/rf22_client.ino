// rf22_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF22 class. RH_RF22 class does not provide for addressing or
// reliability, so you should only use RH_RF22 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf22_server
// Tested on Duemilanove, Uno with Sparkfun RFM22 wireless shield
// Tested on Flymaple with sparkfun RFM22 wireless shield
// Tested on ChiKit Uno32 with sparkfun RFM22 wireless shield

#include <SPI.h>
#include <RH_RF22.h>
#include <RHGenericDriver.h>


RHHardwareSPI spi;

RH_RF22 rf22 (10,2,spi); //chipSelect=10, interruptPin=2

void setup() 
{
 SerialUSB.begin(115200);
 SerialUSB.println("workings1");
 SerialUSB.println("workings2");
 if (!rf22.init())
   SerialUSB.println("init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
spi.attachInterrupt();
  // rf22.setTxPower(RH_RF22_RF23BP_TXPOW_1DBM);
}
void loop()
{

 SerialUSB.println("Sending to rf22_server");
 // Send a message to rf22_server
 uint8_t data[] = "Hello World!";
 rf22.send(data, sizeof(data));
 SerialUSB.println("waiting");
  rf22.waitPacketSent();
  SerialUSB.println("clear");
 // Now wait for a reply
 uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
 uint8_t len = sizeof(buf);
 if (rf22.waitAvailableTimeout(500))
 {
   // Should be a reply message for us now  
   if (rf22.recv(buf, &len))
   {
     SerialUSB.print("got reply: ");
     SerialUSB.println((char*)buf);
   }
   else
   {
     SerialUSB.println("recv failed");
   }
 }
 else
 {
   SerialUSB.println("No reply, is rf22_server running?");
   delay(100);
   SerialUSB.print(rf22.statusRead());
 }
 delay(400);
}
