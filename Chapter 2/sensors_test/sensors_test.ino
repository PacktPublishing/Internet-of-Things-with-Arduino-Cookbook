// Libraries
#include "DHT.h"

// Pins
int sensorPin = A0;
#define DHTPIN 5
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {

  // Serial
  Serial.begin(115200);

  // Init DHT 
  dht.begin();

}

void loop() {

  // Reading temperature and humidity
  float humidity = dht.readHumidity();
  // Read temperature as Celsius
  float temperature = dht.readTemperature();

  // Display
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Reading from analog sensor
  int sensorValue = analogRead(sensorPin);
  float lightLevel = sensorValue/1024.*100;

  // Display
  Serial.print("Light level: ");
  Serial.print(lightLevel);
  Serial.println(" %");
  Serial.println();

  // Wait  
  delay(500);

}
