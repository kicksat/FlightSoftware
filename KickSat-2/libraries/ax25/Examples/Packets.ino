#define CHIRP_PACKET_SIZE 160
#define NUMERICAL_PACKET_SIZE 30000

//data structure used to extract bytes from different types of data
typedef union {
  uint16_t int16val;
  uint32_t int32val;
  float floatVal;
  
  byte data[4];
} union_t;

//creates a packet of 2 byte numbers from 1 to 
void numericalPacket(byte* arr, int numBytes) { 
  union_t n;
  n.int16val = 1;

  for (int i = 0; i < numBytes; i += 2) {
    arr[i] = n.data[1];
    arr[i+1] = n.data[0];
    n.int16val++;
  }
}

//converts the packet passed in into a chirp packet with specified data
void fullChirp(byte* packet, int len, int logNum, byte stat,
byte vBatt, byte iBatt, byte iSolar, byte* data) {
  union_t uni;
    
  uni.int16val = logNum;
  packet[0] = uni.data[1];
  packet[1] = uni.data[0];

  packet[2] = stat;

  packet[3] = vBatt;

  packet[4] = iBatt;

  packet[5] = iSolar;

  for (int i = 0 ; i < len - 6; i++) {
    packet[6 + i] = data[i];
  }
}

//converts packet into a command packet, to be sent from the ground station to the satellite
void commandPacket(byte* packet, int len, byte* cmd, byte* metadata, int metadata_len) {
  for (int i = 0; i < 3; i++) {
    packet[i] = cmd[i];
  }

  for (int i = 0; i < metadata_len; i++) {
    packet[i + 3] = metadata[i];
  }
  
  for (int i = metadata_len + 3; i < len; i++) {
    packet[i] = 0x0;
  }
}

//helper function that prints the packet
void printPacket(byte* packet, int len) {
  Serial.println("====== Packet =======");
  for (int i = 0; i < len; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(packet[i]);
  }
}

void printPacketRaw(byte* packet, int len) {
  Serial.println("====== Packet Raw Data =======");

  //fencepost
  Serial.print("0x");
  Serial.print(packet[0], HEX);
  
  for (int i = 1; i < len; i++) {
    Serial.print(", ");
    Serial.print("0x");
    Serial.print(packet[i], HEX);
  }
  Serial.println();
}


//creates and sets up all the packets, for all your packety needs!
void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println("Start");

  //creates a very large (30KB) numerical packet for test 1
  //byte bigPacket[NUMERICAL_PACKET_SIZE];
  //numericalPacket(bigPacket, NUMERICAL_PACKET_SIZE);

  //creates a packet representing a single chirp
  //includes testing values
  byte chirpPacket[CHIRP_PACKET_SIZE];

  int logNum = 9147;
  byte stat = 0x41;
  byte vBatt = 0x1d;
  byte iBatt = 0x5a;
  byte iSolar = 0x9e;
  byte chirpData[154];
  numericalPacket(chirpData, 154);
  
  fullChirp(chirpPacket, CHIRP_PACKET_SIZE, logNum, stat, vBatt, iBatt, iSolar, chirpData);
  //printPacket(chirpPacket, CHIRP_PACKET_SIZE);

  //creates a command packet to be sent from ground to sat
  byte cmdPacket[20];
  byte cmd[3];
  cmd[0] = 'A';
  cmd[1] = 'R';
  cmd[2] = 'M';
  byte meta[17];
  numericalPacket(meta, 17);
  commandPacket(cmdPacket, 20, cmd, meta, 17);
  printPacketRaw(cmdPacket, 20);

  //at this point, chirpPacket is a fully formed chirp, bigPacket is a 30KB numerical packet,
  //and cmdPacket is a command packet commanding the sat to go into arming mode, with numerical testing metadata
  //use as you please
  
}

void loop() {
}
