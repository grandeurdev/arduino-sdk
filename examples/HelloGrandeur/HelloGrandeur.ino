/**
 * @file HelloGrandeur.ino
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 * Apollo.h is used for device's communication to Grandeur Cloud.
 * ESP8266WiFi.h is used for handling device's WiFi.
 * 
 * This example is the HELLO WORLD of Grandeur Cloud.
*/

#include <Apollo.h>
#include <ESP8266WiFi.h>

// Device's connection configurations
String apiKey = "YOUR-PROJECT-APIKEY";
String deviceID = "YOUR-DEVICE-ID";
String token = "YOUR-ACCESS-TOKEN";
String ssid = "YOUR-WIFI-SSID";
String passphrase = "YOUR-WIFI-PASSWORD";

// Declaring and initializing other variables
Project project;
Device device;

// Function prototypes
void setupWiFi(void);

void setup() {
  Serial.begin(9600);
  // This sets up the device WiFi.
  setupWiFi();
  // This initializes the SDK's configurations and returns a new object of Project class.
  project = apollo.init(apiKey, deviceID, token);
  // Getting object of Device class
  device = project.device();
  Serial.printf("\nDevice %s is saying hello to Grandeur Cloud using API Key %s and Access Token %s.\n", deviceID.c_str(), apiKey.c_str(), token.c_str());
}

void loop() {
  if(project.isConnected()) {
    // When the device's connection with Grandeur Cloud is established, this if-block runs.
    Serial.println("\nDevice has made a successful connection with the Cloud!");
    Serial.println("Grandeur Cloud says hi. Grandeur Cloud will now respond to your commands...");
    Serial.println("Try saving your device state to the Cloud and then retrieving it back.");
    Serial.println("Visit https://grandeur.tech/docs to learn how!");
    Serial.println("Also checkout other examples: \n- DashListening-Device \n- DashListening-App \n- CrossListening.\n");
  }
  // This runs the SDK only when the WiFi is connected.
  project.loop(WiFi.status() == WL_CONNECTED);
}

void setupWiFi(void) {
  // Disconnecting WiFi if it"s already connected
  WiFi.disconnect();
  // Setting it to Station mode which basically scans for nearby WiFi routers
  WiFi.mode(WIFI_STA);
  // Begin connecting to WiFi
  WiFi.begin(ssid, passphrase);
  Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid.c_str(), passphrase.c_str());
  // Device program gets in the while loop as long as the WiFi isn't connected.
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  // This gets printed after the WiFi is connected.
  Serial.printf("\nDevice has successfully connected to WiFi. Its IP Address is: %s\n", WiFi.localIP().toString().c_str());
}