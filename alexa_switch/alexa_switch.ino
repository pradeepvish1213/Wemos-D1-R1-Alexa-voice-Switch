#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();
 
// Change this before you flash
const char* ssid = "Red Jaguar";
const char* password = "Qwerty@12345";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *kitchen = NULL;
Switch *door = NULL;
Switch *bed = NULL;
Switch *bath = NULL;

bool isKitchenLightstsOn = false;
bool isDoorLightstsOn = false;
bool isBedLightstsOn = false;
bool isBathLightstsOn = false;

// declared Pins
int bedRoomPin = 5;
int bathRoomPin = 13;
int doorLightPin = 3;
int kitchenRoomPin = 4;

void setup()
{
  Serial.begin(115200);
  
  //Setting Pins out
    pinMode(LED_BUILTIN,OUTPUT);
    pinMode(kitchenRoomPin, OUTPUT);
    pinMode(doorLightPin, OUTPUT);
    pinMode(bedRoomPin, OUTPUT);
    pinMode(bathRoomPin, OUTPUT);
  
  // Setting initial off all pins
    digitalWrite(kitchenRoomPin, HIGH);
    digitalWrite(doorLightPin, HIGH);
    digitalWrite(bedRoomPin, HIGH);
    digitalWrite(bathRoomPin, HIGH);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
      kitchen = new Switch("kitchen light", 84, kitchenLightsOn, kitchenLightsOff);
      door = new Switch("door light", 83, doorLightsOn, doorLightsOff);
      bed = new Switch("bedroom light", 82, bedLightsOn, bedLightsOff);
      bath = new Switch("bath room light", 81, bathLightsOn, bathLightsOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*kitchen);
    upnpBroadcastResponder.addDevice(*door);
    upnpBroadcastResponder.addDevice(*bed);
    upnpBroadcastResponder.addDevice(*bath);
  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      kitchen->serverLoop();
      door->serverLoop();
      bed->serverLoop();
      bath->serverLoop();

    digitalWrite(LED_BUILTIN,LOW);
    delay(1500);
    digitalWrite(LED_BUILTIN,HIGH);
    delay(1500);
	 }
}

// Kitchen room light
bool kitchenLightsOn() {
    Serial.println("Switch 4 turn on ...");
    isKitchenLightstsOn = true;
    digitalWrite(kitchenRoomPin, LOW);
    return isKitchenLightstsOn;
}

bool kitchenLightsOff() {
  Serial.println("Switch 4 turn off ...");
  isKitchenLightstsOn = false;
  digitalWrite(kitchenRoomPin, HIGH);
  return isKitchenLightstsOn;
}

// door room light
bool doorLightsOn() {
    Serial.println("Switch 3 turn on ...");
    isDoorLightstsOn = true;
    digitalWrite(doorLightPin, LOW);
    return isDoorLightstsOn;
}

bool doorLightsOff() {
  Serial.println("Switch 3 turn off ...");
  isDoorLightstsOn = false;
  digitalWrite(doorLightPin, HIGH);
  return isDoorLightstsOn;
}


// bed room light
bool bedLightsOn() {
    Serial.println("Switch 2 turn on ...");
    isBedLightstsOn = true;
    digitalWrite(bedRoomPin, LOW);
    return isBedLightstsOn;
}

bool bedLightsOff() {
  Serial.println("Switch 2 turn off ...");
  isBedLightstsOn = false;
  digitalWrite(bedRoomPin, HIGH);
  return isBedLightstsOn;
}


// bed room light
bool bathLightsOn() {
    Serial.println("Switch 1 turn on ...");
    isBathLightstsOn = true;
    digitalWrite(bathRoomPin, LOW);
    return isBathLightstsOn;
}

bool bathLightsOff() {
  Serial.println("Switch 1 turn off ...");
  isBathLightstsOn = false;
  digitalWrite(bathRoomPin, HIGH);
  return isBathLightstsOn;
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN,LOW);
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
