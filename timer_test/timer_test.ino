#include <EasyTransfer.h>
#include <Snooze.h>
#include <burn.h>
#include <battery.h>
#include <ax25.h>

#define BEAT_PERIOD    3000   // 3 seconds
#define DATA_PERIOD    24000  // 1 day
#define BATTERY_PERIOD 10000 // 10 secs
#define RADIO_PERIOD   100000 // 100 secs
#define LED            13
#define WD_PIN         0
#define SW_PIN         1
#define SLEEP_TIME     BEAT_PERIOD - 500
#define SERIAL_TIMEOUT 2000
#define LOW_BATTERY_VOLTAGE 6.8  // make this settable through a command??

#define KICKSAT_RADIO_SS        10
#define KICKSAT_RADIO_INTERUPT  2
#define KICKSAT_RADIO_SDN       9
#define RADIO_TRIES             3

typedef struct info {
  uint64_t seconds;
  double x_ecef;
  double y_ecef;
  double z_ecef;
  float voltage;
  float currentIn;
  float currentOut;
  int32_t x_vel;
  int32_t y_vel;
  int32_t z_vel;
  uint32_t tow;
  uint16_t wn;
  uint8_t burnwires;
} info;


// for time keeping
uint32_t prev_millis = 0;
uint64_t seconds = 0;

unsigned long prev_beat_sent;
unsigned long prev_led_sent;
unsigned long prev_data_sent;
unsigned long prev_battery;
unsigned long prev_radio_sent;
int led_flip = 1;
int buffer_index = 0;
char buffer[8];
uint32_t start_t;
uint32_t tries = 0;
uint32_t ack_received = 0;
bool low_voltage_mode = 0;
bool gps_on = 0;

unsigned int radio_tries = 0;

// Object for transferring Data
EasyTransfer ET;
// Sleep config
SnoozeBlock config;

AX25 radio = AX25(KICKSAT_RADIO_SS, KICKSAT_RADIO_INTERUPT); //TODO MAKE LIBRARY TAKE IN SHUTDOWN PIN

info sat_info;
battery battery;

void setup() {
  ///// testing///////////////////////////////////////////////////////////////////////////////////
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  ////////////////////////////////////////////////////////////////////////////////////////
  
  pinMode(WD_PIN, OUTPUT);
  pinMode(SW_PIN, OUTPUT);
  config.setTimer(SLEEP_TIME);

//////////testing//////////////////////////////////////////////////////////////////////////////
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);

  Serial.println(sizeof(prev_beat_sent));
  Serial.println(sizeof(sat_info.x_ecef));
  Serial.println(sizeof(sat_info));

  // Test Data
  sat_info.seconds = 2;
  sat_info.x_vel =   4;
//  sat_info.burnwires = 54;
////////////////////////////////////////////////////////////////////////////////////////
  
}

void loop() {

//  Snooze.deepSleep( config );
//  systick_millis_count += SLEEP_TIME; // b/c millis() doesn't work in sleep

  unsigned long now = millis();
  
  // Update seconds timer (taken from Time library)
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
    
  // Update battery info;
  if (now - prev_battery > BATTERY_PERIOD) {
    battery.update();
    sat_info.voltage = battery.getVoltage();
    sat_info.currentIn = battery.getCurrentIn();
    sat_info.currentOut = battery.getCurrentOut();
    if (sat_info.voltage < LOW_BATTERY_VOLTAGE) {
      low_voltage_mode = 1;
    }
    prev_beat_sent = now;
  }

  if (!low_voltage_mode) {
    if (now - prev_radio_sent > RADIO_PERIOD) {
      //TODO SERIALIZE PACKET
      radio.powerAndInit(KICKSAT_RADIO_SDN);  //takes around 3 seconds
      radio.transmit("FFF");  //TODO make that serialized packet... also ASCII stuff

      //send watchdog reset
      digitalWrite(WD_PIN, HIGH);
      delay(100);
      digitalWrite(WD_PIN,LOW);

      radio_tries = 0;
      while (radio_tries < RADIO_TRIES) {  //todo: and !rcv_msg??
//        if (1) {  // todo make (radio.waitAvailableTimeout(5000) -- soon//todo make timing based on heartbeat length?
//          //WD reset
//          digitalWrite(WD_PIN, HIGH);
//          delay(100);
//          digitalWrite(WD_PIN,LOW)
//          
//          // Should be a reply message for us now   
//          if (radio.recv(RcvSequence, &len)) {
//            Serial.println("got reply: ");
//            for (int i=0 ; i < len; i++) Serial.print(RcvSequence[i]);
//            Demod(RcvSequence,450);  //TODO: make library return pointer
//            //TODO: process command
//          }
//          else {
//            radio_tries++;
//          }
//        }
//        else Serial.println("No reply...");
      }
    }
    // other high power stuff?
    
  }

  if (gps_on) {
    //todo implement
  }


  // for tranferring data
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


