// Import required libraries
#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include <aREST.h>

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
int gas_level;

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);
  
  // Give name and ID to device
  rest.set_id("84e4r3");
  rest.set_name("gas_sensor");

  // Expose variables
  rest.variable("gas", &gas_level);

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
  gas_level = analogRead(A0)/1024.*100;

  // Connect to the cloud
  rest.handle(client);

}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}
