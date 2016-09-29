// Libraries
#include <SPI.h>
#include <WiFi101.h>

// WiFi credentials
char ssid[] = "wifi-name";      //  your network SSID (name)
char pass[] = "wifi-pass";   // your network password
int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;

// Server
char server[] = "dweet.io";

// Connection interval
unsigned long lastConnectionTime = 0;            
const unsigned long postingInterval = 10L * 1000L;

void setup() {
  
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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
  
  // Print connection status
  printWifiStatus();
}

void loop() {

  // Read incoming data
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // Send request at regular intervals
  if (millis() - lastConnectionTime > postingInterval) {

    // Measure light level
    int sensorData = analogRead(A0);

    // Send request
    httpRequest(sensorData);
  }

}

// Make HTTP request
void httpRequest(int sensorData) {

  // Close existing connection
  client.stop();

  // Connect & send request
  if (client.connect(server, 80)) {
    
    Serial.println("connecting...");
    
    // Send the HTTP PUT request:
    client.println("GET /dweet/for/myarduino?light=" + String(sensorData) + " HTTP/1.1");
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


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


