/**
 * @file Searching-esp32.ino
 * @date 21.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 * Grandeur.h is used for device's communication to Grandeur.
 * WiFi.h is used for handling device's WiFi.
 * 
 * This example illustrates the finding documents in datastore.
 * It would be useful in setting device to a previous state or just displaying the data on an LCD.
*/

#include <Grandeur.h>
#include <WiFi.h>

// Device's connection configurations
String apiKey = "YOUR-PROJECT-APIKEY";
String token = "YOUR-ACCESS-TOKEN";
const char* ssid = "YOUR-WIFI-SSID";
const char* passphrase = "YOUR-WIFI-PASSWORD";

// Declaring and initializing other variables
Grandeur::Project myProject;
Grandeur::Project::Datastore myDatastore;
int statePin = 4;
unsigned long current = millis();

// Function prototypes
void WiFiEventCallback(WiFiEvent_t event);
void setupWiFi(void);
void connectionCallback(bool status);
void searchCallback(const char* code, Var result);

void setup() {
  Serial.begin(9600);
  // This sets up the device WiFi.
  setupWiFi();
  // This initializes the SDK's configurations and returns a reference to my project on Grandeur.
  myProject = grandeur.init(apiKey, token);
  // Getting object of Datastore class.
  myDatastore = myProject.datastore();
  // This schedules the connectionCallback() function to be called when connection with Grandeur
  // is made/broken.
  myProject.onConnection(connectionCallback);
}

void loop() {
  if(myProject.isConnected()) {
    if(millis() - current >= 5000) {
      // This if-condition makes sure that the code inside this block runs only after
      // every five seconds.
      // This fetches 1st page of all the documents stored in the datastore.
      Var filter;
      filter["voltage"]["$gt"] = 1;
      myDatastore.collection("logs").search(filter, {}, 0, searchCallback);
      // This updates the millis counter for
      // the five seconds scheduler.
      current = millis();
    }
  }
  
  // The SDK only runs when the WiFi is connected.
  if(WiFi.status() == WL_CONNECTED) project.loop();
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
      // To do that, we set the *state pin* to the value of *state* from Grandeur.
      Serial.println("Device is connected with Grandeur.");
      Serial.println("Logging voltage to Grandeur...");
      break;
    case DISCONNECTED:
      Serial.println("Device's connection with Grandeur is broken.");
      break;
  }
}

void searchCallback(const char* code, Var result) {
  // This function prints if the datastore search for the docs was successfully or not.
  if(searchResult["code"] == "DATASTORE-DOCUMENTS-FETCHED") {
    Serial.print("Documents fetched from Grandeur: ");
    Serial.println(searchResult["documents"].length());
    // Printing all the fetched documents.
    for(int i = 0; i < searchResult["documents"].length(); i++) {
      Serial.println(JSON.stringify(searchResult["documents"][i]).c_str());
      // Just to keep the watchdog timer from tripping.
      delay(1);
    }
    Serial.println("");
    return;
  }
  // If search is not successful.
  Serial.println("Failed to fetch documents.");
  return;
}
