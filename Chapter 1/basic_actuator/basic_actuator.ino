// Pins
int relayPin = 5;

void setup() {

  // Set pin as output
  pinMode(relayPin, OUTPUT);
  
}

void loop() {

  // Set relay ON
  digitalWrite(relayPin, HIGH);

  // Wait
  delay(1000);

  // Set relay OFF
  digitalWrite(relayPin, LOW);

  // Wait
  delay(1000);

}
