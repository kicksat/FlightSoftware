#include <SPI.h>
#include <SD.h>
#include <RH_RF22.h>
#include <RHGenericDriver.h>
#include <RH_RF22.h>
#include "wiring_private.h" // pinPeripheral() function

const int chipSelect = SPI_CS_SD;
volatile uint8_t TC5_error = 0;

RHHardwareSPI spi;
RH_RF22 rf22 (SPI_CS_RFM, RF_NIRQ, spi);
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

int freeRam() 
{
 char stack_dummy = 0;
 return &stack_dummy - sbrk(0);
}

void setup() {

  enable_interrupts();
  init_TC5();
  pinMode(WDT_WDI, OUTPUT);
  
  // Open serial communications and wait for port to open:
  SerialUSB.begin(9600);
  delay(2000);

  SerialUSB.print("Setting Pin Peripheral...");
  pinPeripheral(7, PIO_SERCOM_ALT);
  SerialUSB.println("Pin Peripheral Set");
  
  SerialUSB.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    SerialUSB.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  SerialUSB.println("Card initialized.");

  SerialUSB.println("I'm Alive");
  SerialUSB.print("FreeRam: ");
  SerialUSB.println(freeRam());

  SPI.begin();
  delay(1000);
 
  spi.begin();

  if (!rf22.init())
  {
     SerialUSB.println("rf22 init failed");
     delay(1000);
  }
 
  spi.attachInterrupt();

  SerialUSB.print("RH_PLATFORM: ");
  SerialUSB.println(RH_PLATFORM);
  
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    SerialUSB.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    SerialUSB.println("error opening datalog.txt");
  }

  delay(1000);

  SerialUSB.println("Sending to rf22_server");
  // Send a message to rf22_server
  uint8_t data[] = "Hello World!";
  rf22.send(data, sizeof(data));
  SerialUSB.println("waiting for package sent");
  rf22.waitPacketSent();
  SerialUSB.println("package has been sent");

  delay(500);
}

void init_TC5()
{
  /* Configure Timer/Counter 3 as a timer to blink LED0 */
  // Configure Clocks
  //                     Enable          Choose a clock generator       Select a generic clock to configure/use
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC4_TC5;
  //enabled the bus to TC5, somehow allowing its registers to be programmed
  REG_PM_APBCMASK |= PM_APBCMASK_TC5; // Enable TC5 bus clock
  
  // Configure TC5 (16 bit counter by default)
  //sets the prescaler value, which divides the clock frequency in order to provide lower frequency pulses
  REG_TC5_CTRLA |= TC_CTRLA_PRESCALER_DIV1024;
  
  // Enable interrupts
  //INTENSET register holds the data for which interrupts are enabled
  //each function of the micro has its own INTENSET register
  //in this case, we use the Timer/Counter 3 INTENSET to enable both the ERR and OVF interrupts
  REG_TC5_INTENSET = TC_INTENSET_OVF | TC_INTENSET_ERR;
  
  // Enable TC5
  //looks like the CTRLA register hold generic control items for the Timer/Counter
  //writing a 1 to the ENABLE bit enables the TC 
  REG_TC5_CTRLA |= TC_CTRLA_ENABLE;
  
  //waits until the synchronization between clocks is complete, then the status bit will be cleared
  while ( TC5->COUNT16.STATUS.bit.SYNCBUSY == 1 ){} // wait for TC5 to be enabled
}


void TC5_Handler()
{
  
  // Overflow interrupt triggered
  if ( TC5->COUNT16.INTFLAG.bit.OVF == 1 )
  {
    //toggles WDT
    REG_PORT_OUTTGL0 = WDT_WDI;
    
    //clears the overflow interrupt flag
    REG_TC5_INTFLAG = TC_INTFLAG_OVF;
  }
  
  // Error interrupt triggered
  else if ( TC5->COUNT16.INTFLAG.bit.ERR == 1 )
  {
    TC5_error = 1;
    
    //clears the error interrupt flag
    REG_TC5_INTFLAG = TC_INTFLAG_ERR;
  }
}


void enable_interrupts()
{
  NVIC_EnableIRQ( TC5_IRQn );
}
