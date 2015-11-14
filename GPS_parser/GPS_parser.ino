#include <PiksiGPS.h>

#define led 13
#define SerialGPS Serial3


PiksiGPS gps;
int counter = 0;
char thing;
char test1[1024]; 

void setup() {
  Serial.begin(9600);
  SerialGPS.begin(115200);
  
  Serial.println("started.");  
  pinMode(led,OUTPUT);
  gps.initialize();
  printGpsData();
  delay(1000);
 
  

}

void loop() {
  gps.update();
  
//  delay(500);
  int x;
  if (counter > 100000) {
    printLongLat();
//    Serial.println(thing, HEX);
    msg_log_t logging;
    Serial.println(logging.level & 0x07);
    logging = gps.get_logging();
    String log_str = String(logging.text);
    char test[100] = "hi whatis";
    int i = 0;
    while (logging.text[i]) {
      Serial.write(logging.text[i]);
      i++;
    }
    Serial.println("log messages: ");
//    Serial.println(test);
//    Serial.println(logging.text[0], BIN);
    
    Serial.println(test);
    gps.readThis("simulator", "enabled", test1);
    Serial.print("test1:");
    Serial.println(test1);
//    x = Serial3.write();
//    Serial.println(x);
    
    counter = 0;
  } else {
    counter++;
  }
  
  
  
}

void printLongLat () {
    double longitude, latitude;
    longitude = gps.get_long();
    latitude = gps.get_lat();
    Serial.print("Longitude ====");
    printDouble(longitude, 10000);
    Serial.print("Latitude ======");
    printDouble(latitude, 10000);
    
  
};

void printGpsData () {
    Serial.print("x ====");
    Serial.println(gps.get_pos_x());
    Serial.print("y ======");
    Serial.println(gps.get_pos_y());
    Serial.print("z =======");
    Serial.println(gps.get_pos_z());
    Serial.print("TOW: =======");
    Serial.println(gps.get_tow());
  
};

void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

   Serial.print (int(val));  //prints the int part
   Serial.print("."); // print the decimal point
   unsigned int frac;
   if(val >= 0)
     frac = (val - int(val)) * precision;
   else
      frac = (int(val)- val ) * precision;
   int frac1 = frac;
   while( frac1 /= 10 )
       precision /= 10;
   precision /= 10;
   while(  precision /= 10)
       Serial.print("0");

   Serial.println(frac,DEC) ;
}

