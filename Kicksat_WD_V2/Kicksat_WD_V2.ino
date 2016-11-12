#define NO_PORTB_PINCHANGES
#define NO_PORTC_PINCHANGES

#define DEBUG  //includes serial debugging

#include <LowPower.h>
#include <PinChangeInt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <EEPROM.h>

// Pins and Constants
#define RESET_ENB               12
#define LED                     13
#define WD_PIN                  1   // labeled TX0
#define TEST_LED                10

#define WD_MULT                 4  //Todo not defined

#define INITIAL_DELAY           32  // 45.33 minutes = 340 watchdog cycles
#define INITIAL_DELAY_CYCLES    (INITIAL_DELAY/8) // 340
#define RESET_DELAY             2000




unsigned long last_updated = 0;  // seconds
unsigned long now = 0;           // milliseconds
volatile bool reset_flag = 0;    // for hysteresis
volatile uint8_t wd_count = 0;
volatile bool led_flip = 1;

volatile bool initial_delay_complete = 0;
volatile unsigned int initial_delay_counter = 0;
volatile bool initial_delay_check = 0;

// Called when rising pin change detected on pin 0
void heartbeat() {
  reset_flag = 1;
//  wd_count = 0;
//  wdt_reset();
////////////////////////////////////////////////////////////
  digitalWrite(LED, LOW);
////////////////////////////////////////////////////////////
}

void setup() {
  pinMode(RESET_ENB, OUTPUT);
  pinMode(WD_PIN, INPUT);
  digitalWrite(RESET_ENB, HIGH);

//////////////////////////////////////////////////
//#ifdef DEBUG
  pinMode(LED, OUTPUT);
  pinMode(TEST_LED, OUTPUT);
  
  for (int k = 0; k < 3; k++) {
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(250);
   }
  delay(1000);

//  for(int i = (7); i>0; i--){
//    digitalWrite(13, HIGH);
//    delay(250);
//    digitalWrite(13, LOW);
//    delay(250);
//  }
  digitalWrite(LED, HIGH);
  digitalWrite(TEST_LED, LOW);
//  #endif

}

void loop() {
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  if (!initial_delay_complete) {
    digitalWrite(RESET_ENB, HIGH);
    initial_delay_counter++;
    if (initial_delay_counter >= INITIAL_DELAY_CYCLES) {
      // TODO: WRITE FLAG IN EEPROM
//      EEPROM.update(0, 2);
      initial_delay_complete = 1;
      digitalWrite(RESET_ENB, LOW);
      attachPinChangeInterrupt(WD_PIN, heartbeat, RISING);
//      #ifdef DEBUG
      digitalWrite(LED, LOW);
//      #endif
    } else if (initial_delay_counter > (INITIAL_DELAY_CYCLES/2)) {
//      EEPROM.update(0, 1);
    }
  } else {
//      digitalWrite(TEST_LED, HIGH);
      if (reset_flag) {
        wd_count = 0;
        reset_flag = 0;
      } else {
      wd_count++;
      }
      if (wd_count > WD_MULT - 1) {
        digitalWrite(LED, HIGH);
        resetBoard();
        wd_count = 0;
      }
//    digitalWrite(RESET_ENB, LOW);
    
  }
  (led_flip) ? digitalWrite(TEST_LED, LOW) : digitalWrite(TEST_LED, HIGH);
    led_flip = !led_flip;
}


//ISR(WDT_vect) {
//  if (!initial_delay_complete) {
//    initial_delay_counter++;
//    (led_flip) ? digitalWrite(TEST_LED, HIGH) : digitalWrite(TEST_LED, LOW);
//    led_flip = !led_flip;
//    wdt_reset();
//    if (initial_delay_counter >= INITIAL_DELAY_CYCLES) {
//      // TODO: WRITE FLAG IN EEPROM
////      EEPROM.update(0, 2);
//      initial_delay_complete = 1;
//      digitalWrite(RESET_ENB, LOW);
////      #ifdef DEBUG
//      digitalWrite(LED, LOW);
////      #endif
//    } else if (initial_delay_counter > (INITIAL_DELAY_CYCLES/2)) {
////      EEPROM.update(0, 1);
//    }
//  } else {
//    wd_count++;
//    
////    (led_flip) ? digitalWrite(TEST_LED, HIGH) : digitalWrite(TEST_LED, LOW);
////    led_flip = !led_flip;
//    if (wd_count > WD_MULT - 1) {
//      digitalWrite(LED, HIGH);
//      resetBoard();
////      digitalWrite(TEST_LED, HIGH);
//    //  digitalWrite(TEST_LED, HIGH);
//    //  (led_flip) ? digitalWrite(LED, HIGH) : digitalWrite(LED, LOW);
//    //  led_flip = !led_flip;
//    //  if (reset_flag) {
//    //    digitalWrite(LED,HIGH);
//    //  }
//      reset_flag = 1;
//      wd_count = 0;
//    } 
//    wdt_reset();
//  }
//}

// TODO: fix this
void resetBoard() {
  digitalWrite(RESET_ENB, HIGH);
  reset_flag = 0;
  delay(2000);
  digitalWrite(RESET_ENB, LOW);
//  if (reset_flag == 0) {
//    reset_flag = 1;
//    Serial.println("reset flag set");
//    // send signal to main board?
//  } else if (reset_flag == 1) {
//    // actually reset
//    digitalWrite(LED, LOW);
//    Serial.println("resetting");
//    reset_flag = 0;
//    delay(100);
//  }
}


void pinInterrupt(void)
{
    detachInterrupt(0);
}



