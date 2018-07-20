#include <PiksiGPS.h>

#define led 13
#define SerialGPS Serial3
#define GPS_PIN 17
#define GPS_PERIOD 10000

PiksiGPS gps;
int counter = 0;
char thing;
char test1[1024]; 
msg_pos_ecef_t     pos_ecef1;
msg_vel_ecef_t  velocity;
volatile int gps_on = 1;
int position_updated = 0;
int velocity_updated = 0;
unsigned long now;
unsigned long prev_gps;

unsigned long firstgps = 3;
unsigned long start;

msg_acq_result_t              acq_result1;
tracking_channel_state_t*    tracking_state1;
msg_obs_t               obs1;
msg_log_t logging1;

void setup() {
  Serial.begin(9600);
  SerialGPS.begin(57600);
  delay(1000);
  gps.initialize();
  Serial.println("started.");  
  pinMode(led,OUTPUT);
  pinMode(GPS_PIN ,OUTPUT);
  digitalWrite(GPS_PIN, HIGH);
  start = millis();
  delay(1000);
  
  Serial.println("started.");  
//  printGpsData();
  delay(1000);
  
  
  digitalWrite(led, HIGH);
//  gps.setThis("uart_uarta", "sbp_message_mask", "65535");
//  gps.setThis("uart_uarta", "baudrate", "115200");
//  gps.setThis("simulator", "enabled", "True");
//     gps.readThis("simulator", "enabled", test1);
    gps.readThis("system_info", "firmware_version", test1);
//    gps.setThis("simulator", "enabled", "True");
    gps.setThis("uart_uartb", "sbp_message_mask", "65535");
    gps.setThis("uart_uarta", "sbp_message_mask", "65535");
    gps.setThis("simulator", "enabled", "False");
}

void loop() {

//  digitalWrite(led,HIGH);
//  now = millis();

//  if (now - prev_gps > GPS_PERIOD) {
//    Serial.println("gps turned on");
//    digitalWrite(GPS_PIN, HIGH);
//    gps_on = 1;
//    position_updated = 0;
//    velocity_updated = 0;
//    prev_gps = millis();
//    gps.setThis("simulator", "enabled", "true");
//    
//  }
  
//    delay(50);
    gps.update();
//    gps.readThis("simulator", "enabled", test1);
//    gps.readThis("uart_uartb", "sbp_message_mask", test1);
//    gps.setThis("simulator", "enabled", "True");
//    gps.setThis("uart_uartb", "sbp_message_mask", "65535");
//    gps.setThis("uart_uarta", "sbp_message_mask", "65535");
//     Serial.println("gps turned on");
////    gps.setThis("simulator", "enabled", "true");
//  digitalWrite(led, LOW);
//  delay(500);
  
  
//  delay(500);
//  int x;
//  if (counter > 100000) {
//    printLongLat();
////    Serial.println(thing, HEX);
//    msg_log_t logging;
//    Serial.println(logging.level & 0x07);
//    logging = gps.get_logging();
//    String log_str = String(logging.text);
//    char test[100] = "hi whatis";
//    int i = 0;
//    while (logging.text[i]) {
//      Serial.write(logging.text[i]);
//      i++;
//    }
//    Serial.println("log messages: ");
//    Serial.println(test);
//    Serial.println(logging.text[0], BIN);
    
//    Serial.println(test);
//    gps.readThis("simulator", "numsats", test1);
//    gps.setThis("simulator", "enabled", "true");

  if (counter > 30000) {
//    gps.setThis("simulator", "enabled", "True");
        gps.setThis("simulator", "enabled", "False");
////        gps.setThis("uart_uarta", "baudrate", "115200");
//    gps.setThis("uart_uartb", "sbp_message_mask", "65535");
    gps.setThis("uart_uarta", "sbp_message_mask", "65535");
//    gps.readThis("system_info", "firmware_version", test1);
    Serial.println("we here");
      if (pos_ecef1.tow != gps.get_pos_ecef().tow) {
        if (firstgps == 3){
          firstgps = millis();
        }
        Serial.print("start: ");
        Serial.print(start);
        Serial.print("first: ");
        Serial.println(firstgps);
        pos_ecef1 = gps.get_pos_ecef();
        Serial.print("x: ");
        Serial.println(pos_ecef1.x);
        Serial.print("y: "); 
        Serial.println(pos_ecef1.y);
        Serial.print("z: ");
        Serial.println(pos_ecef1.z);
        position_updated = 1;
      }

      if (velocity.tow != gps.get_vel_ecef().tow) {
        velocity = gps.get_vel_ecef();
        Serial.print("x vel: ");
        Serial.println(velocity.x);
        Serial.print("y vel: ");
        Serial.println(velocity.y);
        Serial.print("z vel: ");
        Serial.println(velocity.z);
        velocity_updated = 1;
      }

//      if (position_updated && velocity_updated && gps_on) {
////        gps_on = 0;
//        digitalWrite(GPS_PIN, LOW);
//        Serial.println("gps turned off");
//      }
//      Serial.print("sending message");
//      gps.readThis("uart_uartb", "sbp_message_mask", test1);
//      gps.setThis("simulator", "enabled", "True");

      
//    logging1 = gps.get_logging();
//    String log_str = String(logging1.text);
//    Serial.println(log_str);
//    int i = 0;
//    while (logging1.text[i]) {
//      Serial.write(logging1.text[i]);
//      i++;
//    }
//      acq_result1 = gps.get_acq_result();
      tracking_state1 = gps.get_tracking_state();
      int i;
        for (i = 0; i<11; i++) {
          Serial.print(i);
          Serial.print(" :this  ");
          Serial.print(tracking_state1[i].state);
          Serial.print(" : ");
          Serial.print(tracking_state1[i].sid.sat);
          Serial.print(" : ");
          Serial.print(tracking_state1[i].sid.code);
          Serial.print(" : ");
          Serial.println(tracking_state1[i].cn0);
        }
//      for (i = 0; i<12; i++) {
//          Serial.print(i);
//          Serial.print(" track: ");
//          Serial.print(tracking_state1.states[i].sid.sat, DEC);
//          Serial.print(" CNO : ");
//          Serial.println(tracking_state1.states[i].cn0, DEC);
//        }
      
//      obs1 = gps.get_obs();
//      Serial.print
//      if (obs1.header.t.tow != gps.get_obs().header.t.tow) {
//        obs1 = gps.get_obs();
//        Serial.print("Number of observations: ");
//        Serial.println(obs1.header.n_obs, BIN);
//        int i;
//        for (i = 0; i<12; i++) {
//          Serial.print(i);
//          Serial.print(" Obs: ");
//          Serial.print(obs1.obs[i].sid.sat, DEC);
//          Serial.print(" CNO : ");
//          Serial.println(obs1.obs[i].cn0, DEC);
//        }
//      }

//      Serial.print("Size of observation header: ");
//      Serial.println(sizeof(obs1));
//      Serial.print("acq: ");
//      Serial.println(acq_result1.sid.code);
      
//      Serial.print("observatoin: ");
//      Serial.println(obs1.obs[0].cn0);
//      Serial.print("tracking state: ");
//      Serial.println(sizeof(tracking_state1.states));
//      Serial.println(tracking_state1.states[1].prn);
//      velocity = gps.get_vel_ecef();
//        Serial.print("x vel: ");
//        Serial.println(velocity.x);
//        Serial.print("y vel: ");
//        Serial.println(velocity.y);
//        Serial.print("z vel: ");
//        Serial.println(velocity.z);
//        pos_ecef1 = gps.get_pos_ecef();
//        Serial.print("x: ");
//        Serial.println(pos_ecef1.x);
//        Serial.print("y: ");
//        Serial.println(pos_ecef1.y);
//        Serial.print("z: ");
//        Serial.println(pos_ecef1.z);
//      
//      delay(10);
      
//    Serial.print("test1:");
//    Serial.println(test1);
//    x = Serial3.write();
//    Serial.println(x);
    
    counter = 0;
  } else {
    counter++;
//    delay(5);
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

