/**
 * @file DashListening-App-esp8266.ino
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 * Grandeur.h is used for device's communication with Grandeur.
 * ESP8266WiFi.h is used for handling device's WiFi.
 * 
 * Dash listening is for one-way listening.
 * This example illustrates the use case of an app listening for updates from the device.
 * It would be useful in building a DEVICE MONITOR which would show how your devices are
 * behaving in terms of their energy units consumed for example.
*/

#include <Grandeur.h>
#include <ESP8266WiFi.h>

// Device's connection configurations
String apiKey = "YOUR-PROJECT-APIKEY";
String deviceID = "YOUR-DEVICE-ID";
String token = "YOUR-ACCESS-TOKEN";
String ssid = "YOUR-WIFI-SSID";
String passphrase = "YOUR-WIFI-PASSWORD";

// Declaring and initializing other variables
unsigned long current = millis();
Project myProject;
Device myDevice;
WiFiEventHandler onWiFiConnectedHandler;
WiFiEventHandler onWiFiDisconnectedHandler;
int voltagePin = A0;

// Function prototypes
void setupWiFi(void);
void connectionCallback(bool state);
void initializeState(Var getResult);
void voltageSetCallback(Var setResult);

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
}

void loop() {
  // In this loop() function, after every five seconds, we send the updated values of our
  // device's voltage and state to Grandeur.
  if(myProject.isConnected()) {
    if(millis() - current >= 5000) {
      // This if-condition makes sure that the code inside this block runs only after
      // every five seconds.

      Serial.println("Setting Voltage");
      int voltage = analogRead(voltagePin);
      // This updates the voltage of our device on Grandeur and schedules voltageSetCallback()
      // function to be called when Grandeur responds with the DATA UPDATED message.
      myDevice.set("voltage", voltage, voltageSetCallback);

      // This updates the millis counter for
      // the five seconds scheduler.
      current = millis();
    }
  }

  // This runs the SDK only when the WiFi is connected.
  myProject.loop(WiFi.status() == WL_CONNECTED);
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

void connectionCallback(bool status) {
  switch(status) {
    case CONNECTED:
      // On successful connection with Grandeur, we initialize the device's *state*.
      // To do that, we get device state from Grandeur and set the *state pin* to its
      // value.
      Serial.println("Device is connected with Grandeur.");
      myDevice.get("state", initializeState);
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

void voltageSetCallback(Var setResult) {
  if(setResult["code"] == "DEVICE-DATA-UPDATED") {
    Serial.printf("Voltage is updated to: %d\n", (int) setResult["update"]["voltage"]);
    
    /* You can set some pins or trigger events here which depend on successful
    ** voltage update.
    */
    return;
  }
  // If the voltage could not be updated.
  Serial.println("Failed to Update Voltage");
  return;
}