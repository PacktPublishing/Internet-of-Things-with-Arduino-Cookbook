// Libraries
#include <SPI.h>
#include <WiFi101.h>
#include "DHT.h"

// WiFi parameters
char ssid[] = "wifi-name";
char password[] = "wifi-pass";

// IFTTT settings
const char* host = "maker.ifttt.com";
const char* eventName   = "alert_light_level";
const char* key = "key";

// Status
int status = WL_IDLE_STATUS;

void setup() {

  Serial.begin(115200);
  delay(10);

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

  // Measure light level
  int sensorData = analogRead(0);
  float lightLevel = (sensorData/1024.)*100;

  // Check humidity data
  if (lightLevel < 50.00) {

    Serial.print("Connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    // We now create a URI for the request
    String url = "/trigger/";
    url += eventName;
    url += "/with/key/";
    url += key;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    int timeout = millis() + 5000;
    while (client.available() == 0) {
      if (timeout - millis() < 0) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");

    // Wait a long time before new alert
    delay(10 * 60 * 1000);

  }
}
