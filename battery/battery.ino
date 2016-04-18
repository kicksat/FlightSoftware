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
  digitalWrite(LED, LOW); // Shut off LED
}

void loop() {
  digitalWrite(LED, HIGH); // LED On
  
  val = analogRead(A1); // Read Voltage
  v = val*(426.0/110.0)*(3.3/1024); // Convert to Volts
  Serial.print("Voltage: "); 
  Serial.print(v);
  Serial.println(" v");

  val = analogRead(A0); // Read Current Out
  i = val*200.0*(3.3/1024); //Convert to mA
  Serial.print("Current Out: ");
  Serial.print(i);
  Serial.println(" mA");

  val = analogRead(A2); // Read Current In
  i = val*200.0*(3.3/1024); //Convert to mA
  Serial.print("Current In: ");
  Serial.print(i);
  Serial.println(" mA");
  
  delay(1000);
  digitalWrite(LED,LOW); // LED Off
  delay(1000);
}
