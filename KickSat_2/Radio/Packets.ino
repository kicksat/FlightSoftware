

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

//helper function that prints the packet
void printPacket(byte* packet, int len) {
  Serial.println("====== Packet =======");
  for (int i = 0; i < len; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(packet[i]);
  }
}

//creates and sets up all the packets, for all your packety needs!
void setup() {
  delay(2000);
  Serial.begin(9600);

  //creates a very large (30KB) numerical packet for test 1
  byte bigPacket[NUMERICAL_PACKET_SIZE];
  numericalPacket(bigPacket, NUMERICAL_PACKET_SIZE);

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

  //at this point, chirpPacket is a fully formed chirp, and bigPacket is a 30KB numerical packet
  //use as you please
  
}
