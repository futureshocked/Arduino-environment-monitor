void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  int lux = 1.68 * sensorValue - 833;
  Serial.println(lux);
  delay(100);
}