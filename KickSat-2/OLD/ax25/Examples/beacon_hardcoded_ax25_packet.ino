// rf22_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF22 class. RH_RF22 class does not provide for addressing or
// reliability, so you should only use RH_RF22 if you do not need the higherM
// level messaging abilities.
// It is designed to work with the other example rf22_server
// Tested on Duemilanove, Uno with Sparkfun RFM22 wireless shield
// Tested on Flymaple with sparkfun RFM22 wireless shield
// Tested on ChiKit Uno32 with sparkfun RFM22 wireless shield

#include <SPI.h>
#include <RH_RF22.h>
#include <RHGenericDriver.h>
#include <RH_RF22.h>
#include "wiring_private.h" // pinPeripheral() function

/// RH_RF22 driver;
/// RHReliableDatagram manager(driver, CLIENT_ADDRESS);

RHHardwareSPI spi;
// const int RF_SDN = 42;
// Singleton instance of the radio driver

RH_RF22 rf22 (SPI_CS_RFM, RF_NIRQ, spi);
//RHReliableDatagram manager(rf22, CLIENT_ADDRESS);

RH_RF22::ModemConfig FSK1k2 = 
{
  0x2B, //reg_1c
  0x03, //reg_1f
  0x41, //reg_20
  0x60, //reg_21
  0x27, //reg_22
  0x52, //reg_23
  0x00, //reg_24
  0x9F, //reg_25
  0x2C, //reg_2c - Only matters for OOK mode
  0x11, //reg_2d - Only matters for OOK mode
  0x2A, //reg_2e - Only matters for OOK mode
  0x80, //reg_58
  0x60, //reg_69
  0x09, //reg_6e
  0xD5, //reg_6f
  0x24, //reg_70
  0x22, //reg_71
  0x01  //reg_72
};

extern "C" char *sbrk(int i);

int FreeRam () 
{
 char stack_dummy = 0;
 return &stack_dummy - sbrk(0);
}

void setup() 
{
 SerialUSB.begin(115200);
 delay(1000);
 SerialUSB.println("I'm Alive");
 SerialUSB.print("FreeRam: ");
 SerialUSB.println(FreeRam());
 
 SPI.begin();
 delay(1000);
 
 spi.begin();
 
 // declare MOSI as alternate
 pinPeripheral(7, PIO_SERCOM_ALT);
 
 if (!rf22.init())
 {
   SerialUSB.println("init failed");
//   while(1)
//   {
//    
//   }
     delay(1000);
 }
 else
 {
   SerialUSB.println("init successful");
 }
 
 spi.attachInterrupt();
 rf22.setModemRegisters(&FSK1k2);
 
 SerialUSB.print("RH_PLATFORM: ");
 SerialUSB.println(RH_PLATFORM);
 // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
 // rf22.setTxPower(RH_RF22_RF23B_TXPOW_1DBM);
 //interrupts();
}

uint8_t packet[] = {
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b01111111,
  0b01110101, 0b10010110, 0b01010100, 0b10101011, 0b01010100, 0b10101011, 0b01010111, 0b01110010,
  0b01011010, 0b01001000, 0b10110101, 0b10101101, 0b10001010, 0b00101000, 0b11101010, 0b10101111,
  0b10110011, 0b00110111, 0b00010000, 0b10110000, 0b11011000, 0b10000111, 0b00001000, 0b10101001,
  0b00101110, 0b10010001, 0b00010001, 0b00000011, 0b01111101, 0b11000000, 0b01111111
};

void loop()
{
 SerialUSB.println("Sending to rf22_server");
 delay(100);
 // Send a message to rf22_server
 // uint8_t data[] = "Log#120898 BAT:88%";
 rf22.send(packet, 95);
 
 // SerialUSB.println(sizeof(data));
 SerialUSB.println("waiting for package sent");
 rf22.waitPacketSent();
 SerialUSB.println("package has been sent");
 // Now wait for a reply

 uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
 uint8_t len = sizeof(buf);
 if(rf22.waitAvailableTimeout(500))
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
