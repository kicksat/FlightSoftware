#include <EasyTransfer.h>
#include <Snooze.h>

#define BEAT_PERIOD    3000   // 3 seconds
#define DATA_PERIOD    24000  // 1 day
#define LED            13
#define WD_PIN         0
#define SW_PIN         1
#define SLEEP_TIME     BEAT_PERIOD
#define SERIAL_TIMEOUT 2000

typedef struct info {
  uint64_t seconds;
  double x_ecef;
  double y_ecef;
  double z_ecef;
  int32_t x_vel;
  int32_t y_vel;
  int32_t z_vel;
  uint8_t burnwires;
} info;


// for time keeping
uint32_t prev_millis = 0;
uint64_t seconds = 0;

unsigned long prev_beat_sent;
unsigned long prev_led_sent;
unsigned long prev_data_sent;
int led_flip = 1;
int buffer_index = 0;
char buffer[8];
uint32_t start_t;
uint32_t tries = 0;
uint32_t ack_received = 0;

// Object for transferring Data
EasyTransfer ET;
// Sleep config
SnoozeBlock config;

info sat_info;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(WD_PIN, OUTPUT);
  pinMode(SW_PIN, OUTPUT);
  config.setTimer(SLEEP_TIME);

  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);

  Serial.println(sizeof(sat_info));
  
//  ET_data.begin(details(sat_info), &Serial1);

  // Test Data
  sat_info.seconds = 2;
  sat_info.x_vel =   4;
  sat_info.burnwires = 54;
  
}

void loop() {

//  Snooze.deepSleep( config );
//  systick_millis_count += SLEEP_TIME; // b/c millis() doesn't work in sleep

  unsigned long now = millis();
  
  // Taken from Time library
  while (now - prev_millis >= 1000) {
    seconds++;
    prev_millis += 1000;
  }
  sat_info.seconds = seconds;

  //************************************************************
  // Start of Schedulder
  //
  //************************************************************
  
  // Heartbeat message - Pin change
  if (now - prev_beat_sent > BEAT_PERIOD) {
      Serial.println("heartbeat");
      digitalWrite(WD_PIN, HIGH);
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(WD_PIN,LOW);
      digitalWrite(LED, LOW);
      prev_beat_sent = now;
    }

  if (now - prev_data_sent > DATA_PERIOD) {
    // TODO : implement
    Serial.print("Data message, tries: ");
    Serial.print(tries);
    serialMode();
    tries = 0;
    ack_received = 0;
//    delay(1000);
    digitalWrite(LED, HIGH);
    Serial.println(tries);
    while (tries < 3 && !ack_received) {
      Serial.println("in while loops");
      buffer_index = 0;
      ET.sendData();
      start_t = millis();
      while (millis() - start_t < SERIAL_TIMEOUT) {
        Serial.println("in other while loop");
        if (Serial1.available()) {
          char new_byte = Serial1.read();
          Serial.print("received charg:  ");
          Serial.println(new_byte);
          if (new_byte == '\n' || buffer_index == 4 ) {
            Serial.print(buffer);
            if (buffer[0] == 'K' && buffer[1] == 'S' && buffer[2] == 'A') {
              ack_received = 1;
              prev_data_sent = millis();
              Serial.println("ack ack");
            }
            break;
          } else if (new_byte != '\n') {
            buffer[buffer_index++] = new_byte;
          } 
          delay(50);  //TODO remove?
        } else {
        delay(200); // TODO: change?
        } 
      }
      tries++;
    }

    // TODO: switch back to watchdog mode
    Serial.println("out of while loop");
    digitalWrite(LED, LOW);
    Serial.println("serial ended 2");
    Serial1.end();
    watchdogMode();
  } 

  delay(2000);
}

// function to switch into serial
void serialMode() {
  digitalWrite(SW_PIN, HIGH);
  delay(100);
  digitalWrite(SW_PIN, LOW);
  // TODO: some type of check
  Serial1.begin(9600);
  ET.begin(details(sat_info), &Serial1); // need to test
}

void watchdogMode() {
  pinMode(LED, OUTPUT);
  pinMode(WD_PIN, OUTPUT);
  digitalWrite(WD_PIN, LOW);
  digitalWrite(SW_PIN, LOW);
}

//    (led_flip) ? digitalWrite(LED, HIGH) : digitalWrite(LED, LOW);
//    led_flip = !led_flip;


