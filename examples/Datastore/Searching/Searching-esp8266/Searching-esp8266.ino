/**
 * @file Searching-esp8266.ino
 * @date 21.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 * Apollo.h is used for device's communication to Grandeur Cloud.
 * ESP8266WiFi.h is used for handling device's WiFi.
 * 
 * This example illustrates the insertion of data into the datastore.
 * It would be useful in logging data on the cloud to visualize it in form of tables or graphs.
*/

#include <Apollo.h>
#include <ESP8266WiFi.h>

// Device's connection configurations
String apiKey = "YOUR-PROJECT-APIKEY";
String token = "YOUR-ACCESS-TOKEN";
String ssid = "YOUR-WIFI-SSID";
String passphrase = "YOUR-WIFI-PASSWORD";

// Declaring and initializing other variables
Project myProject;
Datastore myDatastore;
WiFiEventHandler onWiFiConnectedHandler;
WiFiEventHandler onWiFiDisconnectedHandler;
int statePin = D0;
unsigned long current = millis();

// Function prototypes
void setupWiFi(void);
void connectionCallback(bool status);
void searchCallback(JSONObject payload);

void setup() {
  Serial.begin(9600);
  // This sets up the device WiFi.
  setupWiFi();
  // This initializes the SDK's configurations and returns a reference to my project on the Cloud.
  myProject = apollo.init(apiKey, token);
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
      // This fetches 1st page of all the documents stored in the datastore.
      myDatastore.collection("logs").search({}, {}, 0, searchCallback);
      // This updates the millis counter for
      // the five seconds scheduler.
      current = millis();
    }
  }
  
  // The SDK only runs when the WiFi is connected.
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
      // On successful connection with the cloud, we initialize the device's *state*.
      // To do that, we get device parms from the cloud and set the *state pin* to the
      // value of *state* in those parms.
      Serial.println("Device is connected to the cloud.");
      Serial.println("Fetching documents from the Cloud...");
      break;
    case DISCONNECTED:
      Serial.println("Device is disconnected from the cloud.");
      break;
  }
}

void searchCallback(JSONObject searchResult) {
  // This function prints if the datastore search for the docs was successfully or not.
  if(searchResult["code"] == "DATASTORE-DOCUMENTS-FETCHED") {
    Serial.print("Documents fetched from the Cloud: ");
    Serial.println(searchResult["documents"].size());
    // Printing all the fetched documents.
    for(int i = 0; i < searchResult["documents"].size(); i++) {
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