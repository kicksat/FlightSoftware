#include <EasyTransfer.h>
#include <Snooze.h>


#define BEAT_PERIOD 2000   // 4 seconds
#define DATA_PERIOD 86400000  // 1 day
#define LED        13
#define WD_PIN      0
#define SWITCH_PIN  1
#define SLEEP_TIME 3000

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

EasyTransfer ET_data;
SnoozeBlock config;


info sat_info;

void setup() {
  Serial.begin(9600);
//  Serial1.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(WD_PIN, OUTPUT);
  
  config.setTimer(SLEEP_TIME);

  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  
//  ET_data.begin(details(sat_info), &Serial1);
//  beat.state = 1;
  sat_info.seconds = 0;
  sat_info.x_ecef = 40.53;
  sat_info.burnwires = 54;
  
}

void loop() {

  Snooze.deepSleep( config );
  systick_millis_count += SLEEP_TIME; // b/c millis() doesn't work in sleep
  digitalWrite(WD_PIN, HIGH);
//  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(WD_PIN, LOW);
//  digitalWrite(LED, LOW);
  
  

//   taken from Time library
  while (millis() - prev_millis >= 1000) {
    seconds++;
    prev_millis += 1000;
  }
  
  unsigned long now = millis();
  sat_info.seconds = seconds;

//   digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);               // wait for a second
//  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);               // wait for a second

  if (now - prev_led_sent > 1000) {
//    (led_flip) ? digitalWrite(LED, HIGH) : digitalWrite(LED, LOW);
//    led_flip = !led_flip;
    prev_led_sent = now;
  }
  if (now - prev_data_sent > DATA_PERIOD) {
    // send data
    Serial.println("DATA message");
    ET_data.sendData();
    prev_data_sent = now;
  } 
  if (now - prev_beat_sent > BEAT_PERIOD) {
    // send heartbeat
//    Serial1.write("KSH");
//    Serial.println("heartbeat message");
    digitalWrite(WD_PIN, HIGH);
    digitalWrite(LED, HIGH);
//    (led_flip) ? digitalWrite(LED, HIGH) : digitalWrite(LED, LOW);
//    led_flip = !led_flip;
    delay(100);
    digitalWrite(WD_PIN,LOW);
    digitalWrite(LED, LOW);
    prev_beat_sent = now;
  }

//  Snooze.sleep( config );


  
}

void wd_to_serial() {
  // function to signal watchdog to switch to serial
  
}


