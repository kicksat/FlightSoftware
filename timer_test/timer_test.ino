#include <EasyTransfer.h>

#define BEAT_PERIOD 4000   // 4 seconds
#define DATA_PERIOD 10000  // 30 seconds
#define LED        13

typedef struct info {
  uint64_t seconds;
  double x_ecef;
  double y_ecef;
  double z_ecef;
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

info sat_info;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(LED, OUTPUT);
  
  ET_data.begin(details(sat_info), &Serial1);
//  beat.state = 1;
  sat_info.seconds = 0;
  sat_info.x_ecef = 40.53;
  sat_info.burnwires = 54;
  
}

void loop() {
//   taken from Time library
  while (millis() - prev_millis >= 1000) {
    seconds++;
    prev_millis += 1000;
  }
  
  unsigned long now = millis();
  sat_info.seconds = seconds;

  if (now - prev_led_sent > 1000) {
    (led_flip) ? digitalWrite(LED, HIGH) : digitalWrite(LED, LOW);
    led_flip = !led_flip;
    prev_led_sent = now;
  }
  if (now - prev_data_sent > DATA_PERIOD) {
    // send data
    Serial.println("DATA message");
    ET_data.sendData();
    prev_data_sent = now;
  } 
//  if (now - prev_beat_sent > BEAT_PERIOD) {
//    // send heartbeat
//    Serial1.write("KSH");
//    Serial.println("heartbeat message");
//    ET_beat.sendData();
//    prev_beat_sent = now;
//  }

  
}


