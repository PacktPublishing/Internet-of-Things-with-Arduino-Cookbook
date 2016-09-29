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

     TembooChoreo PostChoreo(client);

    // Invoke the Temboo client
    PostChoreo.begin();

    // Set Temboo account credentials
    PostChoreo.setAccountName(TEMBOO_ACCOUNT);
    PostChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    PostChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    String MessageValue = "Hello from Arduino";
    PostChoreo.addInput("Message", MessageValue);
    String AccessTokenValue = "token";
    PostChoreo.addInput("AccessToken", AccessTokenValue);

    // Identify the Choreo to run
    PostChoreo.setChoreo("/Library/Facebook/Publishing/Post");

    // Run the Choreo; when results are available, print them to serial
    PostChoreo.run();

    while(PostChoreo.available()) {
      char c = PostChoreo.read();
      Serial.print(c);
    }
    PostChoreo.close();


  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between Post calls

}
