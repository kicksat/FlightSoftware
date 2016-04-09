#include <EasyTransfer.h>
#include <Snooze.h>

#define BEAT_PERIOD 3000   // 3 seconds
#define DATA_PERIOD 86400000  // 1 day
#define LED        13
#define WD_PIN      0
#define SWITCH_PIN  1
#define SLEEP_TIME BEAT_PERIOD

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

// Object for transferring Data
EasyTransfer ET_data;
// Sleep config
SnoozeBlock config;

info sat_info;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(WD_PIN, OUTPUT);
  config.setTimer(SLEEP_TIME);

  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  
//  ET_data.begin(details(sat_info), &Serial1);

  // Test Data
  sat_info.seconds = 0;
  sat_info.x_ecef = 40.53;
  sat_info.burnwires = 54;
  
}

void loop() {

  Snooze.deepSleep( config );
  systick_millis_count += SLEEP_TIME; // b/c millis() doesn't work in sleep

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
      digitalWrite(WD_PIN, HIGH);
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(WD_PIN,LOW);
      digitalWrite(LED, LOW);
      prev_beat_sent = now;
    }

  if (now - prev_data_sent > DATA_PERIOD) {
    // TODO : implement
    serialMode()
    ET_data.sendData();
    // TODO: check??
    prev_data_sent = now;
    // TODO: switch back to watchdog mode
  } 
  
}

// function to switch into serial
void serialMode() {
  digitalWrite(SWITCH_PIN, HIGH);
  delay(100);
  digitalWrite(SWITCH_PIN, LOW);
  // TODO: some type of check
  Serial1.begin(9600);
  ET_data.begin(details(sat_info), &Serial1); // need to test
}

//    (led_flip) ? digitalWrite(LED, HIGH) : digitalWrite(LED, LOW);
//    led_flip = !led_flip;


