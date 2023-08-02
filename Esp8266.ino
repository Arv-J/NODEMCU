#include "ESP8266WiFi.h" 
#include <FirebaseArduino.h> 
#include <DHT.h>
#define FIREBASE_HOST "airquality-sys-default- rtdb.firebaseio.com"
#define FIREBASE_AUTH "Q2R7brsqakpefGkbAm1FdFJpw02 bUEMPUG3Iv Vmy"

//wifi connect
const char* KNOWN_SSID[] =
{"Stephanie 4G", "BatStateU ICT", "Ahjin"};
const char* KNOWN_PASSWORD[] =
{"Gwen2020", "Leading Innovations, Transforming LIves", "14151620"};
const int KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]
); // number of known networks #define DHTPIN D2	// Digital pin connected to DHT11 #define DHTTYPE
DHT11	// Initialize dht type as DHT 11
DHT dht(DHTPIN, DHTTYPE);
//new setup void setup() {
//
------
WiFi.mode(WIFI_STA); WiFi.disconnect(); delay(100); Serial.println("Setup done");
//  	
// WiFi.scanNetworks will return the number of networks found
// -------------------------------------------
---------------
------
Serial.println(F("scan start"));
int nbVisibleNetworks = WiFi.scanNetworks(); Serial.println(F("scan done"));
if (nbVisibleNetworks == 0) {
Serial.println(F("no networks found. Reset to try again"));
while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
}
//
------
// if you arrive here at least some networks are visible
//
boolean wifiFound = false; int i, n; Serial.begin(115200);
------
Serial.print(nbVisibleNetworks); Serial.println(" network(s) found");
dht.begin(); dht sensor data
//reads
// -------------------------------------------
---------------
------
//  	 	
// Set WiFi to station mode and disconnect from an AP if it was previously connected
// check if we recognize one by comparing the visible networks
// one by one with our list of known networks
//
for (i = 0; i < nbVisibleNetworks;
++i) {
Serial.println(WiFi.SSID(i)); // Print current SSID
for (n = 0; n < KNOWN_SSID_COUNT; n++) {
// walk through the list of known SSID and check for a match
if (strcmp(KNOWN_SSID[n], WiFi.SSID(i).c_str())) {
Serial.print(F("\tNot matching ")); Serial.println(KNOWN_SSID[n]);
} else { // we got a match wifiFound
= true;
break; // n is the network index we found
delay(500); Serial.print(".");
}
Serial.println("");
// -------------------------------------------
---------------
------
// SUCCESS, you are connected to the known WiFi network
// -------------------------------------------
---------------
Serial.println(F("WiFi connected, your IP address is "));
Serial.println(WiFi.localIP());
}
} // end for each known wifi SSID
if (wifiFound) break; // break from the "for each visible network" loop
} // end for each visible network if (!wifiFound) {
Serial.println(F("no Known network identified. Reset to try again"));
while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
}
// -------------------------------------------
----------------
-----
// if you arrive here you found 1 known SSID
//
------
Serial.print(F("\nConnecting to ")); Serial.println(KNOWN_SSID[n]);
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);	// connect to the firebase
}
//end of new setup void loop()
{
float h =
dht.readHumidity(); // Read Humidity
float t =
dht.readTemperature();	// Read temperature
float air_q =
((analogRead(A0)/2));	//Read Air quality
if (isnan(h) || isnan(t) ||
isnan(air_q)) // Checking sensor working
{
//
------
// We try to connect to the WiFi network we found
//
WiFi.begin(KNOWN_SSID[n], KNOWN_PASSWORD[n]);
while (WiFi.status() != WL_CONNECTED) {
Serial.println(F("Failed to read from DHT sensor!"));
return;
}
Serial.print("Humidity: "); Serial.print(h);
String fireHumid = String(h) + String("
%");	//Humidity integer to string conversion

Serial.print("% Temperature: "); Serial.print(t);
Serial.println("°C ");
String fireTemp = String(t) + String("
°C"); //Temperature integer to string conversion
Serial.print("% Air Quality: "); Serial.print(air_q); Serial.print("PPM");
String fireAq = String (air_q) + String(" PPM"); delay(5000);
Firebase.pushString("/AirqIOT/AQ", fireAq);	//setup path to send Humidity readings
Firebase.pushString("/AirqIOT/Hum ", fireHumid);	//setup path to send Temperature readings
Firebase.pushString("/AirqIOT/Temp ", fireTemp); if (Firebase.failed())
{
Serial.print("pushing /logs failed:"); Serial.println(Firebase.error()); return;
}
}
