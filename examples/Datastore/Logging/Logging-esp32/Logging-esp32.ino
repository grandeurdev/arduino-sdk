/**
 * @file Logging-esp32.ino
 * @date 21.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 * Grandeur.h is used for device's communication to Grandeur.
 * WiFi.h is used for handling device's WiFi.
 * 
 * This example illustrates the insertion of data into the datastore.
 * It would be useful in logging data on the cloud to visualize it in form of tables or graphs.
*/

#include <Grandeur.h>
#include <WiFi.h>

// Device's connection configurations
String apiKey = "YOUR-PROJECT-APIKEY";
String token = "YOUR-ACCESS-TOKEN";
const char* ssid = "YOUR-WIFI-SSID";
const char* passphrase = "YOUR-WIFI-PASSWORD";

// Declaring and initializing other variables
Project myProject;
Datastore myDatastore;
int statePin = 4;
unsigned long current = millis();

// Function prototypes
void WiFiEventCallback(WiFiEvent_t event);
void setupWiFi(void);
void connectionCallback(bool status);
void insertCallback(JSONObject payload);

void setup() {
  Serial.begin(9600);
  // This sets up the device WiFi.
  setupWiFi();
  // This initializes the SDK's configurations and returns a reference to my project on Grandeur.
  myProject = grandeur.init(apiKey, token);
  // Getting object of Datastore class.
  myDatastore = myProject.datastore();
  // This schedules the connectionCallback() function to be called when connection with the cloud
  // is made/broken.
  myProject.onConnection(connectionCallback);
}

void loop() {
  if(myProject.isConnected()) {
    if(millis() - current >= 5000) {
      // This if-condition makes sure that the code inside this block runs only after
      // every five seconds.
      JSONObject logs;
      logs[0]["voltage"] = analogRead(A0);
      myDatastore.collection("logs").insert(logs, insertCallback);
      // This updates the millis counter for
      // the five seconds scheduler.
      current = millis();
    }
  }
  
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
  // Setting WiFi event handler
  WiFi.onEvent(WiFiEventCallback);
  // Begin connecting to WiFi
  WiFi.begin(ssid, passphrase);
  Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid, passphrase);
}

void connectionCallback(bool status) {
  switch(status) {
    case CONNECTED:
      // On successful connection with the cloud, we initialize the device's *state*.
      // To do that, we get device data from the cloud and set the *state pin* to the
      // value of *state* in those data.
      Serial.println("Device is connected to the cloud.");
      Serial.println("Logging voltage to Grandeur...");
      break;
    case DISCONNECTED:
      Serial.println("Device is disconnected from the cloud.");
      break;
  }
}

void insertCallback(JSONObject insertResult) {
  // This function prints if the logs were successfully inserted into the datastore or not.
  if(insertResult["code"] == "DATASTORE-DOCUMENTS-INSERTED") {
    Serial.println("Voltage is successfully logged to Grandeur.");
    return;
  }
  // If insertion is not successful.
  Serial.println("Failed to log voltage");
  return;
}