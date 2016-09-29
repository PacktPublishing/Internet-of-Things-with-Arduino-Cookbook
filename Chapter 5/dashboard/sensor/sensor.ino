// Import required libraries
#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include <aREST.h>
#include "DHT.h"

// Sensor pin & type
#define DHTPIN 6
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Status
int status = WL_IDLE_STATUS;

// WiFi parameters
const char* ssid     = "wifi-name";
const char* password = "wifi-password";

// Clients
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Create aREST instance
aREST rest = aREST(client);

// Function
void callback(char* topic, byte* payload, unsigned int length);

// Variables to be exposed to the API
int temperature;
int humidity;

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);
  
  // Give name and ID to device
  rest.set_id("84e4g3");
  rest.set_name("sensor");

  // Expose variables
  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);

  // Connect to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);

    // Wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("WiFi connected");
  
}

void loop() {

  // Reading temperature and humidity
  humidity = dht.readHumidity();
  
  // Read temperature as Celsius
  temperature = dht.readTemperature();

  // Connect to the cloud
  rest.handle(client);

}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}
