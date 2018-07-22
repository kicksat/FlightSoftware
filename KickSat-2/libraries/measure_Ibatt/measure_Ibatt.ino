/*
WORKING - KMB-21B 07/22 Max Holliday

Testing Ibatt measurement functionality on KMB-21. (Using altered variant files)

High-Side Current-Sense Amplifier with Voltage Output: MAX4372T (gain of 20V/V)
-20mohm sensing resistor installed. 

Max current measurement of 5A:
  5A * 0.02OHM = 100mV (Vsense)
  100mV * 20V/V = 2V measured at microcontroller (Vout)

Ibatt = Vout / (20V/V * 0.02OHM)
*/

const int analogInPin = A5;  

void setup() {
  SerialUSB.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);  
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);  
  SerialUSB.print("LED OFF     ");
  measureIbatt();

  digitalWrite(LED_BUILTIN, LOW);  
  SerialUSB.print("LED ON      ");
  measureIbatt();
  delay(2000);
}

void measureIbatt(){
  int sensorValue = 0;
  float iBatt = 0; 
  sensorValue = analogRead(analogInPin);
  float sensor = (sensorValue * (3.3/4096.0)); //12 bit ADC: 2^12 = 4096
  iBatt = (sensor / (20.0*0.02))*1000.0; //calculating actual Ibatt, putting it in mA
  SerialUSB.print("Ibatt = ");
  SerialUSB.print(iBatt,2);
  SerialUSB.println(" mA");
}

