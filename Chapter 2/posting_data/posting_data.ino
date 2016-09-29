// Libraries
#include <SPI.h>
#include <WiFi101.h>
#include "DHT.h"

// Pins
int sensorPin = A0;
#define DHTPIN 5
#define DHTTYPE DHT11

// WiFi credentials
char ssid[] = "wifi-name";      //  your network SSID (name)
char pass[] = "wifi-pass";   // your network password
int status = WL_IDLE_STATUS;

// Thing name
char * thingName = "mymkr1000";

// Server
char server[] = "dweet.io";

// Connection interval
unsigned long lastConnectionTime = 0;            
const unsigned long postingInterval = 10L * 1000L;

// Initialize the Wifi client library
WiFiClient client;

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {

  // Serial
  Serial.begin(115200);

  // Init DHT 
  dht.begin();

  // Check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // Attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

}

void loop() {

  // Read incoming data
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // Send request at regular intervals
  if (millis() - lastConnectionTime > postingInterval) {

    // Reading temperature and humidity
    float humidity = dht.readHumidity();
    
    // Read temperature as Celsius
    float temperature = dht.readTemperature();

    // Reading from analog sensor
    int sensorValue = analogRead(sensorPin);
    float lightLevel = sensorValue/1024.*100;

    // Send request
    httpRequest(temperature, humidity, lightLevel);
  }

}

// Make HTTP request
void httpRequest(float temperature, float humidity, float lightLevel) {

  // Close existing connection
  client.stop();

  // Connect & send request
  if (client.connect(server, 80)) {
    
    Serial.println("connecting...");

    // Prepare request
    String request = "GET /dweet/for/";
    request += String(thingName);
    request += "?temperature=" + String(temperature);
    request += "&humidity=" + String(humidity);
    request += "&light=" + String(lightLevel);
    request += " HTTP/1.1";
    
    // Send the HTTP request:
    client.println(request);
    client.println("Host: dweet.io");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();

    // Note the time that the connection was made:
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}
