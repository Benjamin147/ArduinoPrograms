int sensorpin = A0;
void setup() {
  // initialize serial communication at 115200 bits per second to match that of the python script:
  Serial.begin(115200);
}
void loop() {
  // read the input on analog pin 0:########################################################
  float sensorValue = analogRead(sensorpin);
  byte data = Serial.read();
  if (data == 's')
  {
    Serial.println(sensorValue);
    delay(10);        // delay in between reads for stability
  }
}
