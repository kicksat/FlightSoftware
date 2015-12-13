unsigned int val;
float v, i;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReadRes(10);
  analogReadAveraging(32);
  analogReference(DEFAULT);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(A1);
  v = val*(426.0/110.0)*(3.3/1024);
  Serial.print("Voltage: ");
  Serial.print(v);
  Serial.println(" v");

  val = analogRead(A0);
  i = val*200.0*(3.3/1024);
  Serial.print("Current: ");
  Serial.print(i);
  Serial.println(" mA");
  
  delay(2000);
}
