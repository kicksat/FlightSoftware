/*
Project: KickSat 2
Peripheral: GPS
Author: Priyanka Mathikshara M
Hardware: - KMB21
          - Adafruit GPS module (9600 baudrate)
          - Antenna
Software: - Adafruit GPS library
File: Header file
Note:
  - We haven't used interrupt.
    Every time the function is called the GPS fix is checked.
    A buffer is filled with 300 characters (4 nmea string approximately). 
    Another funtion looks for the $GPRMC substring in the buffer.
    We also check for the V/A (not valid/valid) part of the GPRMC.
    If everything seems okay, then we can get the Latitude and Longitude.
    Minor detail, we donot let the library parse the nmea while receving the data.
    If we let it do so, then we would get no meaningful output (it would just output 0.00)
*/



#include <Adafruit_GPS.h>
#define GPSECHO  true // If we want to debug -> true

class GPSHandle {
  public:
  
  void init();
  void printGPSdata();
  boolean checkForSubString(String main);
  uint16_t* main();
  void GPSDebuggerTime();
  void GPSDebuggerLocation();
  
    
  uint16_t* read()
  {
    init();
    uint16_t *GPSdataPointer[5] ={0};
	*GPSdataPointer = main();
	return *GPSdataPointer;
  }
};

