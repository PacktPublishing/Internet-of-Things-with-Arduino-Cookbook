// Import required libraries
#include <SPI.h>
#include <WiFi101.h>
#include <aREST.h>

// Status
int status = WL_IDLE_STATUS;

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid     = "wifi-name";
const char* password = "wifi-pass";

// The port to listen for incoming TCP connections
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
bool ledState = false;

// Function
int ledControl(String command);

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

  // Function to be exposed
  rest.function("toggle", ledToggle);

  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("mkr_led");

  // Connect to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);

    // Wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Pin 6 as output
  pinMode(6, OUTPUT);
  
}

void loop() {

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);

}

// Custom function accessible by the API
int ledToggle(String command) {

  ledState = !ledState;

  digitalWrite(6, ledState);
  return 1;
}
