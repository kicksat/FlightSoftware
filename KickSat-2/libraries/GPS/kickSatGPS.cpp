/*
Project: KickSat 2
Peripheral: GPS
File: KickSat GPS CPP file
Author: Priyanka Mathikshara M
Hardware: - KMB21
          - Adafruit GPS module (9600 baudrate)
          - Antenna
Software: - Adafruit GPS library
Ports: GPS pins: 
Functionality: returns the latitude, longitude, altitude, date and time.
Details: 
  - One can call this file with an object for GPSHandle.
	Example: GPSHandle GPS;
			 uint16_t *position[5] = GPS.read();
	function read returns an array pointer, which we can use to extract the data.
	The data is returned in the following order and data type,
		- Latitude - float
		- Longitude - float
		- Altitude - float
		- Time - integer
		- Date - integer
	Latitude and Longitude do not have a NSEW marking, instead they have a positive and negative sign, which we can use to plot and parse easily.
	Time and Date are as given by the gprmc string of NMEA. 
	
Note:
  - We have'nt used interrupt.
    Every time the function is called the GPS fix is checked.
    A buffer is filled with 300 characters (4 nmea string approximately). 
    Another funtion looks for the $GPRMC substring in the buffer.
    We also check for the V/A (not valid/valid) part of the GPRMC.
    If everything seems okay, then we can get the Latitude and Longitude.
    Minor detail, we donot let the library parse the nmea while receving the data.
    If we let it do so, then we would get no meaningful output (it would just output 0.00) output (usually 0.00)

IMPORTANT: GPS WOULD NOT ENTER STANDBY MODE AFTER DELIVERING MEANINGFUL DATA, UNTILL IT IS WOKEN UP AGAIN


*/

#include "KickSatGPS.h"

Adafruit_GPS GPS(&Serial1);
int index1;

GPSHandle::GPSHandle() {

}

void GPSHandle::init()
{
  SerialUSB.begin(115200);
  // SerialUSB.println("KickSat2 GPS test !!"); // debugging UART test (comment out if not debugging)
  GPS.begin(9600); // adafruit GPS works with 9600 baudrate
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
}

void GPSHandle::printGPSdata() {
  bool data = false;
  char c;
  String GPSbuff="";
  int sz = 0;
  for(int i=0;i<300;i++)
  {
  GPSbuff.concat(GPS.read());
  }
  SerialUSB.println(GPSbuff); // comment out if not debugging
 
  if (GPS.newNMEAreceived()) {  // dont parse while receiving part
    if (!GPS.parse(GPS.lastNMEA()))  
      return; 
  }

  if (checkForSubString(GPSbuff))
  {
    if(GPSbuff[index1+18]=='A') // check if its a valid string
    {
    	// GPSDebuggerTime(); // uncomment to debug
   	if (GPS.fix)
	{
	   // GPSDebuggerLocation(); // uncomment to debug
	   // GPS.sendCommand(PMTK_STANDBY); // Go to standby mode
        }
     } 
   } 
}

boolean GPSHandle::checkForSubString(String main)
{
  int flag = 0;
  int i;
  int len = main.length();
  for(i=0;i<len;i++)
  {
    if(main[i]=='$')
    {
      if(main[i+1]=='G')
      {
        if(main[i+2]=='P')
	{
          if(main[i+3]=='R')
	  {
            if(main[i+4]=='M')
	    {
              if(main[i+5]=='C')
	      {
                flag = 1;
                index1 = i;  }}}}}}
       
    }
    if(flag == 1)
    {
      return true;
    }
    else
    {
      return false;
    }
    
 }


void GPSHandle::GPSDebuggerTime()
{  
  SerialUSB.print("\nTime: ");
  SerialUSB.print(GPS.hour, DEC); SerialUSB.print(':');
  SerialUSB.print(GPS.minute, DEC); SerialUSB.print(':');
  SerialUSB.print(GPS.seconds, DEC); SerialUSB.print('.');
  SerialUSB.println(GPS.milliseconds);
  SerialUSB.print("Date: ");
  SerialUSB.print(GPS.day, DEC); SerialUSB.print('/');
  SerialUSB.print(GPS.month, DEC); SerialUSB.print("/20");
  SerialUSB.println(GPS.year, DEC);
  SerialUSB.print("Fix: "); SerialUSB.print((int)GPS.fix);
  SerialUSB.print(" quality: "); SerialUSB.println((int)GPS.fixquality); 
}

void GPSHandle::GPSDebuggerLocation()
{      
  SerialUSB.print("Location: ");
  SerialUSB.print(GPS.latitude, 4); SerialUSB.print(GPS.lat);
  SerialUSB.print(", "); 
  SerialUSB.print(GPS.longitude, 4); SerialUSB.println(GPS.lon);
  SerialUSB.print("Location (in degrees, works with Google Maps): ");
  SerialUSB.print(GPS.latitudeDegrees, 4);
  SerialUSB.print(", "); 
  SerialUSB.println(GPS.longitudeDegrees, 4);  
  SerialUSB.print("Speed (knots): "); SerialUSB.println(GPS.speed);
  SerialUSB.print("Angle: "); SerialUSB.println(GPS.angle);
  SerialUSB.print("Altitude: "); SerialUSB.println(GPS.altitude);
  SerialUSB.print("Satellites: "); SerialUSB.println((int)GPS.satellites);
 }
 
void GPSHandle::read(float buf[])
{
  String h = (char*)GPS.hour;
  String m=  (char*)GPS.minute;
  String s = (char*)GPS.seconds;
  String d = (char*)GPS.day;
  String mt = (char*)GPS.month;
  String y = (char*)GPS.year;

  String timeGpsString = h+m+s;
  String dateGpsString = d+mt+y;

  int timeGpsInt = timeGpsString.toInt();
  int dataGpsInt = dateGpsString.toInt();
  
  printGPSdata();
  buf[0] = GPS.latitudeDegrees;
  buf[1] = GPS.longitudeDegrees;
  buf[2] = GPS.altitude;
  buf[3] = timeGpsInt;
  buf[4] = dataGpsInt;
  
}
