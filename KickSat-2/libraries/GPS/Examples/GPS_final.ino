/*
Project: KickSat 2
Peripheral: GPS
Author: Priyanka Mathikshara M
Hardware: - Arduino UNO
          - Adafruit GPS module (9600 baudrate)
          - Antenna
Software: - Adafruit GPS library
Note:
  - We have'nt used interrupt. Every time the function is called the GPS fix is checked.
    A buffer is filled with 300 characters (4 nmea string approximately). 
    Another funtion looks for the $GPRMC substring
    We also check for the V/A (not valid/valid) part of the GPRMC
    If everything seems okay, then we can get the Latitude and Longitude
    Minor detail, we donot let the library parse the nmea while receving one
    If we let it do so, then we would get no meaningful output (usually 0.00)
*/

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#define TX_PIN 3
#define RX_PIN 2


#if ARDUINO >= 100
  SoftwareSerial mySerial(TX_PIN, RX_PIN);
#else
  NewSoftSerial mySerial(TX_PIN, RX_PIN);
#endif

Adafruit_GPS GPS(&mySerial);
#define GPSECHO  true // If we want to debug -> true
int index;


void setup()  
{    
  Serial.begin(115200);
  Serial.println("KickSat2 GPS test !!");
  GPS.begin(9600); // adafruit GPS works with 9600 baudrate
  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
}

void printGPSdata() {
  bool data = false;
  char c;
  String GPSbuff="";
  int sz = 0;
  for(int i=0;i<300;i++)
  {
  GPSbuff.concat(GPS.read());
  }
  Serial.println(GPSbuff); // comment out if not debugging
 
  if (GPS.newNMEAreceived()) {  // dont parse while receiving part
    if (!GPS.parse(GPS.lastNMEA()))  
      return; 
  }
  
  if (checkForSubString(GPSbuff)) {
    if(GPSbuff[index+18]=='A') // check if its a valid string
    {
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(GPS.longitudeDegrees, 4);  
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      GPS.sendCommand(PMTK_STANDBY); // Go to standby mode
    }
   } 
 } 
}

boolean checkForSubString(String main)
{
  
  int flag = 0;
  int i;
  int len = main.length();
  for(i = 0;i<len;i++)
  {
    if(main[i]=='$'){
      if(main[i+1]=='G'){
        if(main[i+2]=='P'){
          if(main[i+3]=='R'){
            if(main[i+4]=='M'){
              if(main[i+5]=='C'){
                flag = 1;
                index = i;  }}}}}}
       
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

void loop()
{
  printGPSdata();
}

