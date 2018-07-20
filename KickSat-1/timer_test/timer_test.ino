#define DEBUG  //includes serial debugging

#include <EasyTransfer.h>
#include <Snooze.h>
#include <EEPROM.h>
#include <burn.h>
#include <battery.h>
#include <ax25.h>
#include <PiksiGPS.h>

#ifdef DEBUG
  #define BEAT_PERIOD    3000   // 3 seconds
  #define DATA_PERIOD    24000  // 1 day
  #define BATTERY_PERIOD 10000 // 10 secs
  #define RADIO_PERIOD   20000 // 20 secs
  #define LONG_RADIO_PERIOD 20000
  #define RADIO_RX_TIME  13000 // 15 seconds
  #define SLEEP_TIME     BEAT_PERIOD - 500
  #define SERIAL_TIMEOUT 2000
  #define LED            13
  #define CMD_LENGTH     16
  #define GPS_TIMEOUT    20000
#else
  #define BEAT_PERIOD    3000   // 3 seconds
  #define DATA_PERIOD    24000  // 1 day
  #define BATTERY_PERIOD 10000 // 10 secs
  #define RADIO_PERIOD   100000 // 100 secs
  #define LONG_RADIO_PERIOD 200000
  #define RADIO_RX_TIME  15000 // 15 seconds
  #define SLEEP_TIME     BEAT_PERIOD - 500
  #define SERIAL_TIMEOUT 2000
  #define CMD_LENGTH            16
  #define GPS_TIMEOUT    600000  // 10 min
#endif

//TODO: set these and check they work with CRC
#define GPS_CMD                "GPS_ON_QPWO30417"
#define SW_FLAG_CMD            "SW_FLAG_9BHS48LZ"
#define ARM_CMD                "ARM_SPRITES_PASL"
#define DEPLOY_CMD             "DEPLOY_C8w90@3ZS"
#define TX_KILL_ENABLE_CMD     "TX_OFF_DI4203921"
#define TX_KILL_DISABLE_CMD    "TX_ON_PPLFeiD321"
#define PWR_CONSERVE_MODE_CMD  "PWR_CONSERVE_93X"
#define LOW_PWR_MODE_CMD       "LOW_PWR_Wple342t"

#define WD_PIN                  0
#define SW_PIN                  1
#define KICKSAT_RADIO_INTERUPT  2
#define KICKSAT_RADIO_SDN       9
#define KICKSAT_RADIO_SS        10
#define GPS_PIN                 17

// EEPROM locations
#define EEP_FIRST_WAKEUP        0
#define EEP_ANTENNA_DEPLOYED    1
#define EEP_BIT_FLAG            2
#define EEP_ARMED               3
#define EEP_DEPLOYED            4


#define LOW_BATTERY_VOLTAGE   6.8 
#define PWR_CONSERVE_VOLTAGE  7.1
#define PACKET_SIZE           sizeof(info)
#define ACK_SIZE              10


#define RADIO_TRIES             3


#define SerialGPS Serial3

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
uint64_t seconds = 0;       //not necessary?
unsigned long now = 0;

unsigned long radio_period = RADIO_PERIOD;
unsigned long prev_beat_sent;
unsigned long prev_led_sent;
unsigned long prev_data_sent;
unsigned long prev_battery_update;
unsigned long prev_radio_sent;
unsigned long radioRxEnd;
unsigned long gpsEnd;

bool low_power_mode = 0;
bool pwr_cnsrv_mode = 0;
bool gps_on = 0;
bool radioRxMode = 0;
bool radioTxKillSwitch = 0;
bool position_updated = 0;
bool velocity_updated = 0;

byte RcvSequence[MAX_LENGTH_FINAL];
uint8_t len = sizeof(RcvSequence);

char serializedInfo[PACKET_SIZE];
char* radioAckMsg;

int buffer_index = 0;
char buffer[8];
uint32_t start_t;
uint32_t tries = 0;
uint32_t ack_received = 0;

#ifdef DEBUG
int led_flip = 1;
#endif

msg_pos_ecef_t     position;
msg_vel_ecef_t     velocity;

unsigned int radio_tries = 0;

// Object for transferring Data
EasyTransfer ET;
// Sleep config
SnoozeBlock config;
PiksiGPS gps;

AX25 radio = AX25(KICKSAT_RADIO_SS, KICKSAT_RADIO_INTERUPT, KICKSAT_RADIO_SDN ); 
info sat_info;
battery battery;
burn burn;

void setup() {
  
  #ifdef DEBUG
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
   digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
  delay(500);
  #endif
  
  pinMode(WD_PIN, OUTPUT);
  pinMode(SW_PIN, OUTPUT);
  pinMode(KICKSAT_RADIO_SDN, OUTPUT);
  digitalWrite(KICKSAT_RADIO_SDN, HIGH);
  config.setTimer(SLEEP_TIME);
  burn.init();  //TODO : test
  battery.init();
  //TODO: add pins init
  //TODO: add checking eeprom and updating stuff correctly

  #ifdef DEBUG
 

  Serial.println(sizeof(prev_beat_sent));
  Serial.println(sizeof(sat_info.x_ecef));
  Serial.println(sizeof(sat_info));

#endif

}

void loop() {

//  Snooze.deepSleep( config );
//  systick_millis_count += SLEEP_TIME; // b/c millis() doesn't work in sleep

  now = millis();
  
  // Update seconds timer (taken from Time library)
  while (now - prev_millis >= 1000) {
    seconds++;
    prev_millis += 1000;
  }
  sat_info.seconds = seconds;
  #ifdef DEBUG
  Serial.print("Main Seconds: ");
  Serial.print(uint32_t(sat_info.seconds));
  Serial.print("\tMillis() = ");
  Serial.println(millis());
  Serial.print("EEPROM - first woke: "); Serial.print(EEPROM.read(EEP_FIRST_WAKEUP));
  Serial.print("\tant dep: "); Serial.print(EEPROM.read(EEP_ANTENNA_DEPLOYED));
  Serial.print("\tbit flag: "); Serial.print(EEPROM.read(EEP_BIT_FLAG));
  Serial.print("\tarmed: "); Serial.print(EEPROM.read(EEP_ARMED));
  Serial.print("\tdeplyed: "); Serial.print(EEPROM.read(EEP_DEPLOYED));
  #endif

  //************************************************************
  // Start of Schedulder
  //
  //************************************************************
  
  // Heartbeat message -> Pin change
  if (now - prev_beat_sent > BEAT_PERIOD) {
    #ifdef DEBUG
    Serial.println("heartbeat");
//    digitalWrite(LED, HIGH);
    #endif
    digitalWrite(WD_PIN, HIGH);
    delay(100);
    digitalWrite(WD_PIN,LOW);
    #ifdef DEBUG
//    digitalWrite(LED, LOW);
    #endif
    prev_beat_sent = now;
    }
    
  // Update battery info;
  if (now - prev_battery_update > BATTERY_PERIOD) {
    #ifdef DEBUG
    Serial.println("Battery Update");
    #endif
    battery.update();
    sat_info.voltage = battery.getVoltage();
    sat_info.currentIn = battery.getCurrentIn();
    sat_info.currentOut = battery.getCurrentOut();
    #ifdef DEBUG
    Serial.print("Voltage"); Serial.println(sat_info.voltage);
    Serial.print("Voltage"); Serial.println(sat_info.currentOut);
    #endif
    if (sat_info.voltage < LOW_BATTERY_VOLTAGE) {
//      low_power_mode = 1;
//      pwr_cnsrv_mode = 0;
      #ifdef DEBUG
      Serial.println("Low Power Mode");
      #endif
    } else if (sat_info.voltage < PWR_CONSERVE_VOLTAGE) {
      #ifdef DEBUG
      Serial.println("Pwr Convserve Mode");
      #endif
//      pwr_cnsrv_mode = 1;
//      low_power_mode = 0;
//      radio_period = LONG_RADIO_PERIOD;
//      conservePwr();
      // TODO: function to go into pwr conserve mode
      //TODO: figure out how to get out of low power modes
    } else {
      #ifdef DEBUG
      Serial.println("Battery Volt Good");
      #endif
      low_power_mode = 0;
      pwr_cnsrv_mode = 0;
      radio_period = RADIO_PERIOD;
      //TODO: to get out of pwrconserve or low power mode
    }
    prev_battery_update = now;
  }

  // check radio for messages
  if (radioRxMode) {
    if (now < radioRxEnd) {
      #ifdef DEBUG
      Serial.println("Radio Receiving");
      #endif
      if (radio.available()){
        // should be reply message
        Serial.println("Available Message");
        if (radio.receive(RcvSequence, &len)) {
          #ifdef DEBUG
          Serial.print("length = ");Serial.println(len);
          Serial.println("Received Message");
          for (int i=0; i< len ;i++) {Serial.print(RcvSequence[i]);Serial.print(" ");}
          Serial.println("");
          #endif
          char* tempRx;
          tempRx = radio.demod(RcvSequence,len);
          processPacket(tempRx);
          // TODO: test ack packet
          if (!radioTxKillSwitch) {  //TODO: double check?
            radio.setTxMode();
            radio.transmit(radioAckMsg, ACK_SIZE);   
        }
          radioRxMode = 0;
          digitalWrite(KICKSAT_RADIO_SDN, HIGH);
        }
      }
    } else {
      // did not receive a message
      #ifdef DEBUG
      Serial.println("Did not receive Message");
      #endif
      radioRxMode = 0;
      digitalWrite(KICKSAT_RADIO_SDN, HIGH);
    }
  }
//  Serial.println(low_power_mode);
  if (!low_power_mode) { /// OR just change radio period
    if ((now - prev_radio_sent > radio_period)) {
      #ifdef DEBUG
      Serial.println("Radio Transmitting");
      /////////////
//       sat_info.seconds = 0x100;
//       sat_info.x_ecef = 0xDDDDDDDDDDDDDDDD;
////      sat_info.x_ecef = 139240.02;
//      sat_info.y_ecef = 1234.031;
//      sat_info.z_ecef = 0xBBBBBBBBBBBBBBBB;
//      sat_info.voltage = 0xAAAAAAAA;
//      sat_info.currentIn = 0x99999999;
//      //  sat_info.currentOut = 0x88888888;
//      sat_info.currentOut = 1.02;
//      sat_info.x_vel = 0x77777777;
//      sat_info.y_vel = 0x66666666;
//      sat_info.z_vel = 0x68656c6c;
//      sat_info.tow = 0x44444444;
//      sat_info.wn = 0x3333;
//      sat_info.burnwires = 0x22; 
      /////////////
      Serial.println(sat_info.x_ecef);
      Serial.println(sat_info.currentOut);
      #endif
      //TODO: test serialize
//      serializePacket();
      simpleSerialize();
      digitalWrite(KICKSAT_RADIO_SDN, LOW);
      if (radio.powerAndInit()) {  //takes around 3 seconds
        if (!radioTxKillSwitch) {
          radio.setTxMode();
          radio.transmit(serializedInfo, PACKET_SIZE);  //TODO: send and test serialized packet... 
        }
        
        //send watchdog reset /// TODO: DO WE NEED THIS? I don't think so
        digitalWrite(WD_PIN, HIGH);
        delay(100);
        digitalWrite(WD_PIN,LOW);
        /////////////////
  
        // switch to RX mode
        radio.setRxMode();    
        radioRxMode = 1;
        radioRxEnd = now + RADIO_RX_TIME;  // + sleep time?
        #ifdef DEBUG
        Serial.print("now: ");
        Serial.println(now);
        Serial.print("rx end time: ");
        Serial.println(radioRxEnd);
        #endif
      }
      prev_radio_sent = now;

//      radio_tries = 0;
//      while (radio_tries < RADIO_TRIES) {  //todo: and !rcv_msg??
////        if (1) {  // todo make (radio.waitAvailableTimeout(5000) -- soon//todo make timing based on heartbeat length?
////          //WD reset
////          digitalWrite(WD_PIN, HIGH);
////          delay(100);
////          digitalWrite(WD_PIN,LOW)
////          
////          // Should be a reply message for us now   
////          if (radio.recv(RcvSequence, &len)) {
////            Serial.println("got reply: ");
////            for (int i=0 ; i < len; i++) Serial.print(RcvSequence[i]);
////            Demod(RcvSequence,450);  //TODO: make library return pointer
////            //TODO: process command
////          }
////          else {
////            radio_tries++;
////          }
////        }
////        else Serial.println("No reply...");
//      }
    }
    // other high power stuff?
    
  }

  // TODO: test this
  if (gps_on  && gpsEnd < now) {
    #ifdef DEBUG
    Serial.println("GPS on");
    #endif
    gps.update();
    if (position.tow != gps.get_pos_ecef().tow) {
      #ifdef DEBUG
      Serial.println("New ECEF message");
      Serial.print("x: ");
      Serial.println(position.x);
      Serial.print("y: "); 
      Serial.println(position.y);
      Serial.print("z: ");
      Serial.println(position.z);
      #endif
      /// TODO: make this correct
      position = gps.get_pos_ecef();
      sat_info.x_ecef = position.x;
      sat_info.y_ecef = position.y;
      sat_info.z_ecef = position.z;
      position_updated = 1;
    }
  
    if (velocity.tow != gps.get_vel_ecef().tow) {
      #ifdef DEBUG
      Serial.println("New velocity message");
      Serial.print("x vel: ");
      Serial.println(velocity.x);
      Serial.print("y vel: ");
      Serial.println(velocity.y);
      Serial.print("z vel: ");
      Serial.println(velocity.z);
      #endif
      velocity = gps.get_vel_ecef();                          
      sat_info.x_vel = velocity.x;
      sat_info.y_vel = velocity.y;
      sat_info.z_vel = velocity.z;
      velocity_updated = 1;
    }
    if (position_updated && velocity_updated && gps_on) {
      #ifdef DEBUG
      Serial.println("GPS off");
      Serial.println("gps turned off");
      #endif
      gps_on = 0;
      digitalWrite(GPS_PIN, LOW);

      // TODO: reset position/velocity_updated to 0
     }
  } else {
    gps_on = 0;
  }


  // for tranferring data
  /*
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
  */

  delay(1000);
}

void conservePwr() {
  // TODO: implement
  #undef RADIO_PERIOD
  #define RADIO_PERIOD 
}

void processPacket(char* msg) {
  #ifdef DEBUG
  for (int i=0 ; i < len; i++) Serial.print(msg[i]);
  #endif
  if (strncmp(GPS_CMD, msg, CMD_LENGTH) == 0) {
    gps_on = 1;
    gpsEnd = now + GPS_TIMEOUT;
    radioAckMsg = "ack_gps_on";
    #ifdef DEBUG
    Serial.println("GPS command");
    #endif
  } else if (strncmp(SW_FLAG_CMD, msg, CMD_LENGTH) == 0) {
    //TODO implement
    EEPROM.update(EEP_BIT_FLAG, 1);
    radioAckMsg = "ack_sw_flg";
    #ifdef DEBUG
    Serial.println("Softare bit flag command");
    #endif
  } else if (strncmp(ARM_CMD, msg, CMD_LENGTH) == 0) {
    //TODO implement
    burn.burnDB1();
    EEPROM.update(EEP_ARMED, 1);
    radioAckMsg = "ack_bw_arm";
    #ifdef DEBUG
    Serial.println("Arm sprites command");
    #endif
  } else if (strncmp(DEPLOY_CMD, msg, CMD_LENGTH) == 0) {
    //TODO implement
    
    // TODO: test -- checking bit flag and armed flag
    //TODO: CHANGE TO == 1
    if(EEPROM.read(EEP_BIT_FLAG) && EEPROM.read(EEP_ARMED)) {
      burn.burnDB2();
      EEPROM.update(EEP_DEPLOYED, 1);
      radioAckMsg = "ack_deploy";
    #ifdef DEBUG
    Serial.println("Deploy sprites command");
    #endif
    }
  } else if (strncmp(TX_KILL_ENABLE_CMD, msg, CMD_LENGTH) == 0) {
    radioTxKillSwitch = 1;
    //TODO test
    radioAckMsg = "ack_tx_off";
    #ifdef DEBUG
    Serial.println("Radio TX kill enable command");
    #endif
  } else if (strncmp(TX_KILL_DISABLE_CMD, msg, CMD_LENGTH) == 0) {
    radioTxKillSwitch = 0;
    //TODO test
    radioAckMsg = "ack_tx_on1";
    #ifdef DEBUG
    Serial.println("Radio TX kill disabled command");
    #endif
  } else if (strncmp(PWR_CONSERVE_MODE_CMD, msg, CMD_LENGTH) == 0) {
    //TODO implement increase radio time
    radioAckMsg = "ack_consrv";
    #ifdef DEBUG
    Serial.println("power conserve mode command");
    #endif
  } else if (strncmp(LOW_PWR_MODE_CMD, msg, CMD_LENGTH) == 0) {
    low_power_mode = 1;
    radioAckMsg = "ack_lowpwr";
    //TODO implement
    
    #ifdef DEBUG
    Serial.println("low power mode command");
    #endif
  } else {
    //
    radioAckMsg = "ack_no_cmd";
    #ifdef DEBUG
    Serial.println("did not match command");
    #endif
  }
  
}

void serializePacket() {
  for (int i=0; i < PACKET_SIZE ; i++) serializedInfo[i] = 0x00;
  memcpy(serializedInfo, &sat_info, PACKET_SIZE);
  #ifdef DEBUG
  Serial.println(PACKET_SIZE);
  Serial.print("serialized pacekt: ");
//  for (int i=0; i < PACKET_SIZE ; i++) {Serial.print(sat_info[i]); Serial.print(" ");}
  Serial.println(" ");
  for (int i=0; i < PACKET_SIZE ; i++) {Serial.print(serializedInfo[i]); Serial.print(" ");}
  Serial.println(" ");
  #endif
}

void simpleSerialize() {
  for (int i=0; i < PACKET_SIZE ; i++) serializedInfo[i] = 0x00;
  sprintf(serializedInfo, "T:%llu, V:%6.2f, CI: %6.2f, CO: %6.2f", sat_info.seconds, sat_info.voltage, sat_info.currentIn, sat_info.currentOut);
  #ifdef DEBUG
  Serial.print("serialized pacekt: ");
  for (int i=0; i < PACKET_SIZE*2 ; i++) {Serial.print(serializedInfo[i]); Serial.print(" ");}
  Serial.println(" ");
  #endif
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


