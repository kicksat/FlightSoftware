
//Important Pins
#define ANTENNA_BURN_1  3
#define ANTENNA_BURN_2  4
#define DEPLOY_BURN_1   5
#define DEPLOY_BURN_2   6
#define ENAB_GPS        17
#define LED             13

//Constants
#define ANTENNA_BURN_TIME   2000 //milliseconds
#define SPRITE_BURN_TIME   12000 //milliseconds

unsigned char antenna1_deployed = 0;
unsigned char antenna2_deployed = 0;
unsigned char sprites_armed = 0;
unsigned char sprites_deployed = 0;

char command_buffer[4];

void setup() {
  Serial.begin(9600);
  
  pinMode(ANTENNA_BURN_1, OUTPUT);
  digitalWrite(ANTENNA_BURN_1, LOW);
  
  pinMode(ANTENNA_BURN_2, OUTPUT);
  digitalWrite(ANTENNA_BURN_2, LOW);
  
  pinMode(DEPLOY_BURN_1, OUTPUT);
  digitalWrite(DEPLOY_BURN_1, LOW);
  
  pinMode(DEPLOY_BURN_2, OUTPUT);
  digitalWrite(DEPLOY_BURN_2, LOW);
  
  pinMode(ENAB_GPS, OUTPUT);
  digitalWrite(ENAB_GPS, LOW);
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.println("Ready");
}

void loop() {
  delay(1000);
  if(Serial.available() == 3)
  {
    Serial.readBytes(command_buffer, 3);
    Serial.println(command_buffer);
    
    if(strcmp(command_buffer, "AB1") == 0)
    {
      Serial.println("Deploying Antenna 1");
      delay(1000);
      digitalWrite(LED, HIGH);
      for (unsigned int k = 0; k < ANTENNA_BURN_TIME/100; ++k)
      {
        // PWM with 10% duty cycle
        digitalWrite(ANTENNA_BURN_1, HIGH);
        delay(5);
        digitalWrite(ANTENNA_BURN_1, LOW);
        delay(45);
        digitalWrite(ANTENNA_BURN_1, HIGH);
        delay(5);
        digitalWrite(ANTENNA_BURN_1, LOW);
        delay(45);
      }
      digitalWrite(LED, LOW);
      antenna1_deployed = 1;
    }
    else if(strcmp(command_buffer, "AB2") == 0)
    {
      Serial.println("Deploying Antenna 2");
      delay(1000);
      digitalWrite(LED, HIGH);
      for (unsigned int k = 0; k < ANTENNA_BURN_TIME/100; ++k)
      {
        // PWM with 10% duty cycle
        digitalWrite(ANTENNA_BURN_2, HIGH);
        delay(5);
        digitalWrite(ANTENNA_BURN_2, LOW);
        delay(45);
        digitalWrite(ANTENNA_BURN_2, HIGH);
        delay(5);
        digitalWrite(ANTENNA_BURN_2, LOW);
        delay(45);
      }
      digitalWrite(LED, LOW);
      antenna2_deployed = 1;
    }
    else if(strcmp(command_buffer, "DB1") == 0)
    {
      Serial.println("Arming Sprite Deployer");
      delay(1000);
      digitalWrite(LED, HIGH);
      for (unsigned int k = 0; k < ANTENNA_BURN_TIME/100; ++k)
      {
        // PWM with 10% duty cycle
        digitalWrite(DEPLOY_BURN_1, HIGH);
        delay(5);
        digitalWrite(DEPLOY_BURN_1, LOW);
        delay(45);
        digitalWrite(DEPLOY_BURN_1, HIGH);
        delay(5);
        digitalWrite(DEPLOY_BURN_1, LOW);
        delay(45);
        
      }
      digitalWrite(LED, LOW);
      sprites_armed = 1;
    }
    else if(strcmp(command_buffer, "DB2") == 0)
    {
      Serial.println("Deploying Sprites");
      delay(1000);
      digitalWrite(LED, HIGH);
      for (unsigned int k = 0; k < SPRITE_BURN_TIME/100; ++k)
      {
        // PWM with 30% duty cycle
        digitalWrite(DEPLOY_BURN_2, HIGH);
        delay(15);
        digitalWrite(DEPLOY_BURN_2, LOW);
        delay(35);
        digitalWrite(DEPLOY_BURN_2, HIGH);
        delay(15);
        digitalWrite(DEPLOY_BURN_2, LOW);
        delay(35);
      }
      digitalWrite(LED, LOW);
      sprites_deployed = 1;
    }
    
    while(Serial.available())
    {
      Serial.read();
    }
  }
}
