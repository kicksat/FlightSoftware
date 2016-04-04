#include <EasyTransfer.h>
#include <LowPower.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// Pins and Constants
#define RESET_ENB   12
#define LED         13
#define WD_PIN      0

unsigned long last_updated = 0;  // seconds
unsigned long now = 0;    // milliseconds
volatile char reset_flag = 0;     // for hysteresis
int led_flip = 1;

// Structs for serial communication

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

EasyTransfer ET_data;

info sat_info;

void setup() {
//  Serial.begin(9600);
  
  pinMode(RESET_ENB, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(WD_PIN, INPUT);
  
//  ET_data.begin(details(sat_info), &Serial);

//  delay(1000);
  for (int k = 0; k < 3; k++) {
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(250);
   }
  digitalWrite(LED, LOW);
//  wdt_enable(WDTO_8S);
//  wdt_setup();
  pinSetup();
//  Serial.println("Watchdog Ready");

}

void loop() {
  LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//  sleep_enable();
}

void wdt_setup() {

  wdt_reset();
  // disable interrupts
  cli();
  // Enter Watchdog Configuration mode:
  WDTCSR |= (1<<WDCE) | (1<<WDE);   
  // Set Watchdog settings:
  //        enb int     no reset
  // WDTCSR = (1<<WDIE) | (0<<WDE) ;
   WDTCSR = 0b01100000;   // last bit goes between 4 and 8 secs
   sei();
}

ISR(WDT_vect) {
  digitalWrite(LED, HIGH);
//  (led_flip) ? digitalWrite(LED, HIGH) : digitalWrite(LED, LOW);
//  led_flip = !led_flip;
//  if (reset_flag) {
//    digitalWrite(LED,HIGH);
//  }
  reset_flag = 1;
}

ISR(PCINT2_vect) {
//  if (digitalRead(WD_PIN)) {
  if(PIND & (1 << PD1)){
    reset_flag = 0;
    digitalWrite(LED, LOW);
    wdt_reset();
//    (led_flip) ? digitalWrite(LED, HIGH) : digitalWrite(LED, LOW);
//    led_flip = !led_flip;
  }
}

void sleepNow() {

    attachInterrupt(0, pinInterrupt, LOW);
    delay(50);
    // Choose our preferred sleep mode:
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
 
    // Set sleep enable (SE) bit:
    sleep_enable();
 
    // Put the device to sleep:
    sleep_mode();
 
    // Upon waking up, sketch continues from this point.
    sleep_disable();
}

void pinSetup() {
  cli();
  PCICR |= 0b00000100;    // turn on port d
  PCMSK2 |= 0b00000010;    // ,PCINT17, physical pin tx0
  sei();
}

void pinInterrupt(void)
{
    detachInterrupt(0);
}

// TODO: fix this
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
  

