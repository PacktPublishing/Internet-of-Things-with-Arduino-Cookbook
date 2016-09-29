// Candle for Arduino, controlled from the cloud

// Libraries
#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include <aREST.h>
#include <Adafruit_NeoPixel.h>
#define PIN 5

// Status
int status = WL_IDLE_STATUS;

// WiFi parameters
const char* ssid     = "wifi-name";
const char* password = "wifi-pass";

// Clients
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Create aREST instance
aREST rest = aREST(client);

// Variables
bool candleState = false;

// Function
int candleControl(String command);
void callback(char* topic, byte* payload, unsigned int length);

// Color variables
int redPx = 255;
int grnHigh = 135; 
int bluePx = 15;

// Animation time variables
int burnDepth = 14; //10 for 5v, 14 for 3.3v -- how much green dips below grnHigh for normal burn - 
int flutterDepth = 30; //25 for 5v, 30 for 3.3v -- maximum dip for flutter
int cycleTime = 120; //120 -- duration of one dip in milliseconds

// Variables
int fDelay;
int fRep;
int flickerDepth;
int burnDelay;
int burnLow;
int flickDelay;
int flickLow;
int flutDelay;
int flutLow;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

   // Set callback
  client.setCallback(callback);

  // Function to be exposed
  rest.function("candle", candleControl);

  // Give name and ID to device
  rest.set_id("01e48c");
  rest.set_name("candle");

  // Connect to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);

    // Wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("WiFi connected");

  // Setup candle
  flickerDepth = (burnDepth + flutterDepth) / 2.4;
  burnLow = grnHigh - burnDepth;
  burnDelay = (cycleTime / 2) / burnDepth;
  flickLow = grnHigh - flickerDepth;
  flickDelay = (cycleTime / 2) / flickerDepth;
  flutLow = grnHigh - flutterDepth;
  flutDelay = ((cycleTime / 2) / flutterDepth);

  // Start
  strip.begin();
}

// In loop, call CANDLE STATES, with duration in seconds
// 1. on() = solid yellow
// 2. burn() = candle is burning normally, flickering slightly
// 3. flicker() = candle flickers noticably
// 4. flutter() = the candle needs air!

void loop() {

  // Connect to the cloud
  rest.handle(client);

  // Candle control
  if (candleState == true) {
    burn(10);
    flicker(5);
    burn(8);
    flutter(6);
    burn(3);
    on(10);
    burn(10);
    flicker(10);
  }
  else {
    strip.setPixelColor(0, 1, 1, 1);
    strip.show();
  }
  
}


// basic fire funciton - not called in main loop
void fire(int grnLow) {
  for (int grnPx = grnHigh; grnPx > grnLow; grnPx--) {
    strip.setPixelColor(0, redPx, grnPx, bluePx);
    strip.show();
    delay(fDelay);
  }  
  for (int grnPx = grnLow; grnPx < grnHigh; grnPx++) {
    strip.setPixelColor(0, redPx, grnPx, bluePx);
    strip.show();
    delay(fDelay);
  }
}

// fire animation
void on(int f) {
  fRep = f * 1000;
  int grnPx = grnHigh - 5;
  strip.setPixelColor(0, redPx, grnPx, bluePx);
  strip.show();
  delay(fRep);
}

void burn(int f) {
  fRep = f * 8;
  fDelay = burnDelay;
  for (int var = 0; var < fRep; var++) {
    fire(burnLow);
  }  
}

void flicker(int f) {
  fRep = f * 8;
  fDelay = burnDelay;
  fire(burnLow);
  fDelay = flickDelay;
  for (int var = 0; var < fRep; var++) {
    fire(flickLow);
  }
  fDelay = burnDelay;
  fire(burnLow);
  fire(burnLow);
  fire(burnLow);
}

void flutter(int f) {
  fRep = f * 8;  
  fDelay = burnDelay;
  fire(burnLow);
  fDelay = flickDelay;
  fire(flickLow);
  fDelay = flutDelay;
  for (int var = 0; var < fRep; var++) {
    fire(flutLow);
  }
  fDelay = flickDelay;
  fire(flickLow);
  fire(flickLow);
  fDelay = burnDelay;
  fire(burnLow);
  fire(burnLow);
}

// Custom function accessible by the API
int candleControl(String command) {

  candleState = !candleState;
  return 1;
}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}
