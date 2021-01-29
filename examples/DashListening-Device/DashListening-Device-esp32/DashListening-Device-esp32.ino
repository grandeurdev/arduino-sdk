/**
 * @file DashListening-Device-esp32.ino
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 * Grandeur.h is used for device's communication with Grandeur.
 * WiFi.h is used for handling device's WiFi.
 * 
 * Dash listening is for one-way listening.
 * This example illustrates the use case of a device listening for updates from the app.
 * It would be useful in building an INTERNET SWITCH to help you control your device without
 * caring about how your device responds to your commands.
*/

#include <Grandeur.h>
#include <WiFi.h>

// Device's connection configurations
String apiKey = "YOUR-PROJECT-APIKEY";
String deviceID = "YOUR-DEVICE-ID";
String token = "YOUR-ACCESS-TOKEN";
String ssid = "YOUR-WIFI-SSID";
String passphrase = "YOUR-WIFI-PASSWORD";

// Declaring and initializing other variables
Project myProject;
Device myDevice;
int statePin = 4;

// Function prototypes
void WiFiEventCallback(WiFiEvent_t event);
void setupWiFi(void);
void connectionCallback(bool state);
void initializeState(Var getResult);
void stateUpdatedCallback(Var updatedData);

void setup() {
  Serial.begin(9600);
  // This sets up the device WiFi.
  setupWiFi();
  // This initializes the SDK's configurations and returns reference to your project.
  myProject = grandeur.init(apiKey, token);
  // Getting reference to your device.
  myDevice = myProject.device(deviceID);
  // This schedules the connectionCallback() function to be called when connection with Grandeur
  // is made/broken.
  myProject.onConnection(connectionCallback);
  // This schedules stateUpdatedCallback() function to be called when device state is changed on
  // Grandeur.
  myDevice.on("state", stateUpdatedCallback);
}

void loop() {
  // The SDK only runs when the WiFi is connected.
  myProject.loop(WiFi.status() == WL_CONNECTED);
}

void WiFiEventCallback(WiFiEvent_t event) {
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      // This runs when the device connects with WiFi.
      Serial.printf("\nDevice has successfully connected to WiFi. Its IP Address is: %s\n",
        WiFi.localIP().toString().c_str());
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      // This runs when the device disconnects with WiFi.
      Serial.println("Device is disconnected from WiFi.");
      break;
    default: break;
  }
}

void setupWiFi(void) {
  // Disconnecting WiFi if it"s already connected
  WiFi.disconnect();
  // Setting it to Station mode which basically scans for nearby WiFi routers
  WiFi.mode(WIFI_STA);
  // Setting WiFi event handlers
  WiFi.onEvent(WiFiEventCallback);
  // Begin connecting to WiFi
  WiFi.begin(ssid, passphrase);
  Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid, passphrase);
}

void connectionCallback(bool status) {
  switch(status) {
    case CONNECTED:
      // On successful connection with Grandeur, we initialize the device's *state*.
      // To do that, we get device state from Grandeur and set the *state pin* to its
      // value.
      Serial.println("Device is connected with Grandeur.");
      myDevice.get("state", initializeState);
      Serial.println("Listening for data update from Grandeur...");
      break;
    case DISCONNECTED:
      Serial.println("Device's connection with Grandeur is broken.");
      break;
  }
}

void initializeState(Var getResult) {
  // This function sets the *state pin* to the *state value* that we received in data
  // from Grandeur.
  if(getResult["code"] == "DEVICE-DATA-FETCHED") {
    int state = getResult["data"];
    Serial.printf("State is: %d\n", state);
    digitalWrite(statePin, state);
    return;
  }
  // If the data could not be fetched.
  Serial.println("Failed to Fetch Data");
  return;
}

void stateUpdatedCallback(Var updatedData) {
  // This function gets the *updated state* from the device data and sets the *state pin*
  // to its value.
  Serial.printf("Updated State is: %d\n", (int) updatedData["state"]);
  digitalWrite(statePin, (int) updatedData["state"]); 
}