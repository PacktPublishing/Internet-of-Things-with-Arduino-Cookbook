// Libraries
#include <SPI.h>
#include <WiFi101.h>
#include <Temboo.h>
#include "TembooAccount.h" // Contains Temboo account information

// WiFi parameters
char ssid[] = "wifi-name";
char password[] = "wifi-pass";

// WiFi client
WiFiClient client;

// Status
int status = WL_IDLE_STATUS;

void setup() {

  // Serial
  Serial.begin(115200);

  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);

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

    // Runnin Choreo
    TembooChoreo StatusesUpdateChoreo(client);

    // Invoke the Temboo client
    StatusesUpdateChoreo.begin();

    // Set Temboo account credentials
    StatusesUpdateChoreo.setAccountName(TEMBOO_ACCOUNT);
    StatusesUpdateChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    StatusesUpdateChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String StatusUpdateValue = "Hello from Arduino!";
    StatusesUpdateChoreo.addInput("StatusUpdate", StatusUpdateValue);
    String ConsumerKeyValue = "key";
    StatusesUpdateChoreo.addInput("ConsumerKey", ConsumerKeyValue);
    String AccessTokenValue = "token";
    StatusesUpdateChoreo.addInput("AccessToken", AccessTokenValue);
    String ConsumerSecretValue = "secret";
    StatusesUpdateChoreo.addInput("ConsumerSecret", ConsumerSecretValue);
    String AccessTokenSecretValue = "secretToken";
    StatusesUpdateChoreo.addInput("AccessTokenSecret", AccessTokenSecretValue);

    // Identify the Choreo to run
    StatusesUpdateChoreo.setChoreo("/Library/Twitter/Tweets/StatusesUpdate");

    // Run the Choreo; when results are available, print them to serial
    StatusesUpdateChoreo.run();

    while(StatusesUpdateChoreo.available()) {
      char c = StatusesUpdateChoreo.read();
      Serial.print(c);
    }
    StatusesUpdateChoreo.close();

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between StatusesUpdate calls
}
