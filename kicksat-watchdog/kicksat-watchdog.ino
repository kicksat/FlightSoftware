#include <EasyTransfer.h>

// Pins and Constants
#define RESET_ENB   12
#define LED         13
#define WATCHDOG_TIMEOUT 5000

unsigned long last_updated = 0;  // seconds
unsigned long now = 0;    // milliseconds
char reset_flag = 0;     // for hysteresis

// Structs for serial communication

typedef struct info {
  uint64_t seconds;
  double x_ecef;
  double y_ecef;
  double z_ecef;
  uint8_t burnwires;
} info;


EasyTransfer ET_data;

info sat_info;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  pinMode(RESET_ENB, OUTPUT);
  pinMode(LED, OUTPUT);
  
  ET_data.begin(details(sat_info), &Serial1);

  delay(1000);
  digitalWrite(LED, HIGH);
  Serial.println("Watchdog Ready");
}

void loop() {
  
  now = millis();

  if (ET_data.receiveData()) {
    // new gps data received
    Serial.print("seconds: ");
    Serial.printf("%llu\n",sat_info.seconds);
    Serial.print("x: ");
    Serial.println(sat_info.x_ecef);
    Serial.print("burnwires: ");
    Serial.println(sat_info.burnwires);
    digitalWrite(LED, LOW);
    // Write new data to EEPROM
    // send ack??
    last_updated = now;
  } 
//  if (ET_beat.receiveData()) {
//    Serial.println("heartbeat received");
//    last_updated = now;
//    // update EEPROM with last updated
//    // send an ack??
//  }

  if ((now - last_updated) > WATCHDOG_TIMEOUT) {
    // write time of reset to EEPROM??
    Serial.println("watchdog reset triggered");
    resetBoard();
    digitalWrite(LED, HIGH);
  }

  // sleep??
//  delay (2000);
    
}

void resetBoard() {
  if (reset_flag == 0) {
    reset_flag = 1;
    Serial.println("reset flag set");
    // send signal to main board?
  } else if (reset_flag == 1) {
    // actually reset
    digitalWrite(LED, LOW);
    Serial.println("resetting");
    reset_flag = 0;
    delay(100);
  }
  
  
//  digitalWrite(RESET_ENB, HIGH);
//  delay(4000);
//  digitalWrite(RESET_ENB, LOW);
  // recovery from reset process?
}
