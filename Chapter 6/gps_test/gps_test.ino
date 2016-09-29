// Based on the FONA example code by Adafruit

// Libraries
#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

// Pins
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

// Buffer
char replybuffer[255];

// Instances
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

// Fona instance
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
uint8_t type;

void setup() {

  // Initi serial
  while (!Serial);
  Serial.begin(115200);
  Serial.println(F("FONA reading SMS"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      Serial.println(F("FONA 3G (European)")); break;
    default: 
      Serial.println(F("???")); break;
  }
  
  // Print module IMEI number.
  char imei[15] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }

  // Setup GPRS settings
  //fona.setGPRSNetworkSettings(F("your APN"), F("your username"), F("your password"));

  // Read all SMS
  int8_t smsnum = fona.getNumSMS();
  uint16_t smslen;
  int8_t smsn;

  // Check if 3G
  if ( (type == FONA3G_A) || (type == FONA3G_E) ) {
    smsn = 0; // zero indexed
    smsnum--;
  } else {
    smsn = 1;  // 1 indexed
  }

  // Enable GPS
  if (!fona.enableGPS(true)) {
    Serial.println(F("Failed to turn on"));
  }

  // Check GPS fix
  int8_t stat;
  stat = fona.GPSstatus();
  if (stat < 0)
    Serial.println(F("Failed to query"));
  if (stat == 0) Serial.println(F("GPS off"));
  if (stat == 1) Serial.println(F("No fix"));
  if (stat == 2) Serial.println(F("2D fix"));
  if (stat == 3) Serial.println(F("3D fix"));

  // Check for GPS location
  char gpsdata[120];
  fona.getGPS(0, gpsdata, 120);
  if (type == FONA808_V1)
    Serial.println(F("Reply in format: mode,longitude,latitude,altitude,utctime(yyyymmddHHMMSS),ttff,satellites,speed,course"));
  else 
    Serial.println(F("Reply in format: mode,fixstatus,utctime(yyyymmddHHMMSS),latitude,longitude,altitude,speed,course,fixmode,reserved1,HDOP,PDOP,VDOP,reserved2,view_satellites,used_satellites,reserved3,C/N0max,HPA,VPA"));
  Serial.println(gpsdata);

  // Format Data
  String latitude = getLatitudeGPS(gpsdata);
  String longitude = getLongitudeGPS(gpsdata);

  float latitudeNumeric = convertDegMinToDecDeg(latitude.toFloat());
  float longitudeNumeric = convertDegMinToDecDeg(longitude.toFloat());

  Serial.print("Latitude, longitude: ");
  Serial.print(latitudeNumeric, 4);
  Serial.print(",");
  Serial.println(longitudeNumeric, 4);
  

}
void loop() {
  
}

double convertDegMinToDecDeg (float degMin) {
  double min = 0.0;
  double decDeg = 0.0;
 
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
 
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
 
  return decDeg;
}

String getLatitudeGPS(String data) {

  // Find commas
  int commaIndex = data.indexOf(',');
  int secondCommaIndex = data.indexOf(',', commaIndex+1);
  int thirdCommaIndex = data.indexOf(',', secondCommaIndex+1);
  
  return data.substring(commaIndex + 1, secondCommaIndex);
}


String getLongitudeGPS(String data) {

  // Find commas
  int commaIndex = data.indexOf(',');
  int secondCommaIndex = data.indexOf(',', commaIndex+1);
  int thirdCommaIndex = data.indexOf(',', secondCommaIndex+1);
  
  return data.substring(secondCommaIndex + 1, thirdCommaIndex);
}

