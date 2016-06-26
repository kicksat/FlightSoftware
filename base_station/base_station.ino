#include <ax25.h>

#define KICKSAT_RADIO_SS        10
#define KICKSAT_RADIO_INTERUPT  2
#define KICKSAT_RADIO_SDN       9
#define TEENSY_LED              16
//#define RH_RF22_HAVE_SERIAL

#define GPS_CMD                "GPS_ON_QPWO30417"
#define SW_FLAG_CMD            "SW_FLAG_9BHS48LZ"
#define ARM_CMD                "ARM_SPRITES_PASL"
#define DEPLOY_CMD             "DEPLOY_C8w90@3ZS"
#define TX_KILL_ENABLE_CMD     "TX_OFF_DI4203921"
#define TX_KILL_DISABLE_CMD    "TX_ON_PPLFeiD321"
#define PWR_CONSERVE_MODE_CMD  "PWR_CONSERVE_93X"
#define LOW_PWR_MODE_CMD       "LOW_PWR_Wple342t"

#define CMD_LENGTH 16

AX25 radio = AX25(KICKSAT_RADIO_SS, KICKSAT_RADIO_INTERUPT, KICKSAT_RADIO_SDN);

byte RcvSequence[450];
uint8_t len = sizeof(RcvSequence);

char command_buffer[4];

void setup() {
  Serial.begin(9600);
  pinMode(TEENSY_LED, OUTPUT);
  pinMode(KICKSAT_RADIO_SDN, OUTPUT);
  Serial.write("Testing 123");
  digitalWrite(KICKSAT_RADIO_SDN, LOW);
  radio.powerAndInit();

  

}

void loop() {
  Serial.println(strlen(GPS_CMD));
//  delay(1000);
  if(Serial.available() > 3) {
    Serial.readBytes(command_buffer, 3);
    Serial.println(command_buffer);
    
    if(strcmp(command_buffer, "GPS") == 0) {
      Serial.println("GPS command"); 
      radio.transmit(GPS_CMD, CMD_LENGTH);
    } else if(strcmp(command_buffer, "SWF") == 0) {
      Serial.println("Software Flag");
      radio.transmit(SW_FLAG_CMD, CMD_LENGTH);
    } else if(strcmp(command_buffer, "ARM") == 0) {
      Serial.println("Arming Sprite Deployer");
      radio.transmit(ARM_CMD, CMD_LENGTH);
    } else if(strcmp(command_buffer, "DEP") == 0) {
      Serial.println("Deploying Sprites");
      radio.transmit(DEPLOY_CMD, CMD_LENGTH);
    } else if(strcmp(command_buffer, "TXF") == 0) {
      Serial.println("radio tx off");
      radio.transmit(TX_KILL_ENABLE_CMD, CMD_LENGTH);
    } else if(strcmp(command_buffer, "TXN") == 0) {
      Serial.println("radio tx BACK ON");
      radio.transmit(TX_KILL_DISABLE_CMD, CMD_LENGTH);
    } else if(strcmp(command_buffer, "PWC") == 0) {
      Serial.println("power conserve mode");
      radio.transmit(PWR_CONSERVE_MODE_CMD, CMD_LENGTH);
    } else if(strcmp(command_buffer, "LOW") == 0) {
      Serial.println("LOW power mode");
      radio.transmit(LOW_PWR_MODE_CMD, CMD_LENGTH);
    }
    while(Serial.available())
    {
      Serial.read();
    }

  }

  radio.setRxMode();
  Serial.print("RXing");

  delay(4000);

  if (radio.available()) {
    // should be reply message
    Serial.print("msg available");
    if (radio.receive(RcvSequence, &len)) {
      
      Serial.println("Received Message");
      Serial.print("len = "); Serial.println(len);
      char* tempRx;
      tempRx = radio.demod(RcvSequence,len);
      for (int i=0; i < len ; i++) {Serial.print(tempRx[i], HEX); Serial.print(" ");}
//      for (int i=0; i< len ;i++) Serial.print(tempRx[i], DEC); Serial.print(" ");
      Serial.println("");
//      processPacket(tempRx);
    }
  } else {
    // did not receive a message
    Serial.println("Did not receive Message");
    }


}
