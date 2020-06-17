/**
 * @file CrossListening.ino
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 * Apollo.h is used for device's communication with Grandeur Cloud.
 * ESP8266WiFi.h is used for handling device's WiFi.
 * 
 * Cross listening means when the device listens for updates from the app and the app
 * listens for updates from the device.
 * This example illustrates pretty much every basic thing you'd need in order to monitor /
 * control your device through Grandeur Cloud. Here are some of those:
 * 1. Listen to the cloud for updates in parms variables.
 * 2. Publish updates in summary and parms variables to the cloud every 5 seconds.
 * 3. Controlling SDK's internal valve. This helps if you want to run the SDK only when a
 * certain condition is true; in our case, if the WiFi is connected.
*/

#include <Apollo.h>
#include <ESP8266WiFi.h>

// Device's connection configurations
String deviceID = "YOUR-DEVICE-ID";
String apiKey = "YOUR-PROJECT-APIKEY";
String token = "YOUR-ACCESS-TOKEN";
String ssid = "YOUR-WIFI-SSID";
String passphrase = "YOUR-WIFI-PASSWORD";

// Declaring and initializing other variables
unsigned long current = millis();
ApolloDevice device;
WiFiEventHandler onWiFiConnectedHandler;
WiFiEventHandler onWiFiDisconnectedHandler;
int statePin = D0;
int voltagePin = A0;

// Function prototypes
void setupWiFi(void);
void connectionCallback(JSONObject updateObject);
void initializeState(JSONObject payload);
void parmsUpdatedCallback(JSONObject updatedParms);
void summarySetCallback(JSONObject payload);
void parmsSetCallback(JSONObject payload);


void setup() {
  Serial.begin(9600);
  // This sets up the device WiFi.
  setupWiFi();
  // This initializes the SDK's configurations and returns a new object of ApolloDevice class.
  device = apollo.init(deviceID, apiKey, token);
  // This schedules the connectionCallback() function to be called when the device makes/breaks
  // connection with the cloud.
  device.onConnection(connectionCallback);
  // This schedules parmsUpdatedCallback() function to be called when someone changes any
  // variable stored in device's parms on the Cloud.
  device.onParmsUpdated(parmsUpdatedCallback);
}

void loop() {
  // In this loop() function, after every five seconds, we send the updated values of our
  // device's voltage and state to the Cloud.
  if(device.getState() == CONNECTED) {
    if(millis() - current >= 5000) {
      // This if-condition makes sure that the code inside this block runs only after
      // every five seconds.

      Serial.println("Setting Summary");
      JSONObject summary;
      summary["voltage"] = analogRead(voltagePin);
      // This updates the summary of our device on the Cloud and schedules summarySetCallback()
      // function to be called when the Cloud responds with the SUMMARY UPDATED message.
      device.setSummary(summary, summarySetCallback);

      Serial.println("Setting Parms");
      JSONObject parms;
      parms["state"] = digitalRead(statePin);
      // This updates the parms of our device on the Cloud and schedules parmsSetCallback()
      // function to be called when the Cloud responds with the PARMS UPDATED message.
      device.setParms(parms, parmsSetCallback);

      // This updates the millis counter for
      // the five seconds scheduler.
      current = millis();
    }
  }

  // This runs the SDK only when the WiFi is connected.
  device.loop(WiFi.status() == WL_CONNECTED);
}

void setupWiFi(void) {
  // Disconnecting WiFi if it"s already connected
  WiFi.disconnect();
  // Setting it to Station mode which basically scans for nearby WiFi routers
  WiFi.mode(WIFI_STA);
  // Setting WiFi event handlers
  onWiFiConnectedHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
    // This runs when the device connects with WiFi.
    Serial.printf("\nDevice has successfully connected to WiFi. Its IP Address is: %s\n",
      WiFi.localIP().toString().c_str());
  });
  onWiFiDisconnectedHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
    // This runs when the device disconnects with WiFi.
    Serial.println("Device is disconnected from WiFi.");
  });
  // Begin connecting to WiFi
  WiFi.begin(ssid, passphrase);
  Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid.c_str(), passphrase.c_str());
}

void connectionCallback(JSONObject updateObject) {
  switch((int) updateObject["event"]) {
    case CONNECTED:
      // On successful connection with the cloud, we initialize the device's *state*.
      // We set the *state pin* to the value of *state* that we receive from the cloud in parms.
      Serial.println("Device is connected to the cloud.");
      device.getParms(initializeState);

      // Initializing the millis counter for the five
      // seconds timer.
      current = millis();
      break;
    case DISCONNECTED:
      Serial.println("Device is disconnected from the cloud.");
      break;
  }
}

void initializeState(JSONObject payload) {
  // This function sets the *state pin* to the *state value* that we received in parms
  // from the cloud.
  if(payload["code"] == "DEVICE-PARMS-FETCHED") {
    int state = payload["deviceParms"]["state"];
    digitalWrite(statePin, state);
    return;
  }
  // If the parms could not be fetched.
  Serial.println("Failed to Fetch Parms");
  return;
}

void parmsUpdatedCallback(JSONObject updatedParms) {
  // This function gets the *updated state* from the device parms and set the *state pin*
  // with *state value*.
  Serial.printf("Updated State is: %d\n", (bool) updatedParms["state"]);
  digitalWrite(statePin, (bool) updatedParms["state"]); 
}

void summarySetCallback(JSONObject payload) {
  if(payload["code"] == "DEVICE-SUMMARY-UPDATED") {
    Serial.printf("Voltage is updated to: %d\n", (int) payload["update"]["voltage"]);
    
    /* You can set some pins or trigger events here which depend on successful
    ** device summary update.
    */
    return;
  }
  // If the summary could not be updated.
  Serial.println("Failed to Update Summary");
  return;
}

void parmsSetCallback(JSONObject payload) {
  if(payload["code"] == "DEVICE-PARMS-UPDATED") {
    Serial.printf("State is updated to: %d\n", (bool) payload["update"]["state"]);

    /* You can set some pins or trigger events here which depend on successful
    ** device parms update.
    */
    return;
  }
  // If the parms could not be updated.
  Serial.println("Failed to Update Parms");
  return;
}