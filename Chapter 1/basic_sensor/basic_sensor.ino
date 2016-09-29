// Pins
int sensorPin = A0;

void setup() {

  // Serial
  Serial.begin(115200);

}

void loop() {

  // Reading 
  int sensorValue = analogRead(sensorPin);

  // Display
  Serial.print("Sensor reading: ");
  Serial.println(sensorValue);

  // Wait
  delay(500);

}
