#define KICKSAT_RADIO_SS        PORT_PA28
#define KICKSAT_RADIO_INTERUPT  PORT_PA02
#define KICKSAT_RADIO_SDN       PORT_PA03
#define TEENSY_LED              PORT_PA07
#define RH_RF22_HAVE_SERIAL

AX25 radio = AX25(KICKSAT_RADIO_SS, KICKSAT_RADIO_INTERUPT);

void setup() {
  Serial.begin(9600);
  pinMode(TEENSY_LED, OUTPUT);
  pinMode(KICKSAT_RADIO_SDN, OUTPUT);
  Serial.write("Testing 123");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(TEENSY_LED, LOW);
  Serial.println("sending");
  radio.powerAndInit();
  char* payload = "FFF";
  radio.transmit(payload, 12);
  digitalWrite(TEENSY_LED, HIGH);

  delay(1000);

}
