#define LED             13

unsigned int val;
float v, i;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReadRes(10);
  analogReadAveraging(32);
  analogReference(DEFAULT);


  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  digitalWrite(LED, HIGH);
  
  val = analogRead(A1);
  v = val*(426.0/110.0)*(3.3/1024);
  Serial.print("Voltage: ");
  Serial.print(v);
  Serial.println(" v");

  val = analogRead(A0);
  i = val*200.0*(3.3/1024);
  Serial.print("Current Out: ");
  Serial.print(i);
  Serial.println(" mA");

  val = analogRead(A2);
  i = val*200.0*(3.3/1024);
  Serial.print("Current In: ");
  Serial.print(i);
  Serial.println(" mA");
  
  delay(1000);
  digitalWrite(LED,LOW);
  delay(1000);
}
