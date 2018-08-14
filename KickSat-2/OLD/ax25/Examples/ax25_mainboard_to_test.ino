#include <ax25.h>
#include <SPI.h>
#include <RH_RF22.h>
#include <RHGenericDriver.h>
#include <RH_RF22.h>
#include "wiring_private.h" // pinPeripheral() function
#define RH_RF22_HAVE_SERIAL

AX25 radio = AX25(SPI_CS_RFM, RF_NIRQ, RF_SDN);


void setup()
{
  SerialUSB.begin(115200);
  delay(500);

  // declare MOSI as alternate
  pinPeripheral(7, PIO_SERCOM_ALT);
  radio.powerAndInit();
  pinMode(RF_SDN, OUTPUT);
  SerialUSB.println("Testing 123");
}
char data[] = "BAT:55% STATUS:GOOD";
void loop()
{
  // put your main code here, to run repeatedly:
  SerialUSB.println("sending");
  radio.transmit(data, sizeof(data));
  SerialUSB.print("sent");
  delay(1000);
}
