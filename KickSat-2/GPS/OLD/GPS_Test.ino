// Test code for Adafruit GPS modules using MTK3329/MTK3339 driver
//
// This code turns on the LOCUS built-in datalogger. The datalogger
// turns off when power is lost, so you MUST turn it on every time
// you want to use it!
//
// Tested and works great with the Adafruit Ultimate GPS module
// using MTK33x9 chipset
//    ------> http://www.adafruit.com/products/746
// Pick one up today at the Adafruit electronics shop 
// and help support open source hardware & software! -ada

#include <Adafruit_GPS.h>
#if ARDUINO >= 100
 #include <SoftwareSerial.h>
#else
  // Older Arduino IDE requires NewSoftSerial, download from:
  // http://arduiniana.org/libraries/newsoftserial/
// #include <NewSoftSerial.h>
 // DO NOT install NewSoftSerial if using Arduino 1.0 or later!
#endif


#define TX_PIN 3
#define RX_PIN 2


#if ARDUINO >= 100
  SoftwareSerial mySerial(TX_PIN, RX_PIN);
#else
  NewSoftSerial mySerial(TX_PIN, RX_PIN);
#endif
Adafruit_GPS GPS(&mySerial);
// If using hardware serial (e.g. Arduino Mega), comment
// out the above six lines and enable this line instead:
//Adafruit_GPS GPS(&Serial1);


// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  true

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

void setup()  
{    
  while (!Serial);  // the Leonardo will 'wait' until the USB plug is connected

  // connect at 115200 so we can read the GPS fast enuf and
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS logging start test!");

  // 9600 NMEA is the default baud rate for MTK - some use 4800
  GPS.begin(9600);
  //GPS.end();
  
  // You can adjust which sentences to have the module emit, below
  // Default is RMC + GGA
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);//GGA
  // Default is 1 Hz update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  //useInterrupt(false);
  delay(500);
  Serial.print("\nSTARTING LOGGING....");
  //if (GPS.LOCUS_StartLogger())
    Serial.println(" STARTED!");
  //else
    Serial.println(" no response :(");
  delay(1000);
  //delay(5000);
}

#define threshold 55

void printGPSdata() {
  bool data = false;
  char c;
  int sz = 0;
  while (c = GPS.read()) {
    data = true;
    sz++;
    if (c == '\n') { 
      break;
    }
    Serial.print(c);
    delay(1);
  }
  if (data) {
    Serial.println();
    Serial.println(sz);
    if (sz < threshold) {
      Serial.println("Not long enough\n");
    } else {
      Serial.println("Valid data!\n");
    }
    data = false;
  }
}

void loop()
{
  printGPSdata();
}

/******************************************************************/

//// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
//SIGNAL(TIMER0_COMPA_vect) {
//  char c = GPS.read();
//  // if you want to debug, this is a good time to do it!
//  if (GPSECHO && c) {
//#ifdef UDR0
//    UDR0 = c;  
//    // writing direct to UDR0 is much much faster than Serial.print 
//    // but only one character can be written at a time. 
//#endif
//  }
//}
//
//void useInterrupt(boolean v) {
//  if (v) {
//    // Timer0 is already used for millis() - we'll just interrupt somewhere
//    // in the middle and call the "Compare A" function above
//    OCR0A = 0xAF;
//    TIMSK0 |= _BV(OCIE0A);
//    usingInterrupt = true;
//  } else {
//    // do not call the interrupt function COMPA anymore
//    TIMSK0 &= ~_BV(OCIE0A);
//    usingInterrupt = false;
//  }
//}

