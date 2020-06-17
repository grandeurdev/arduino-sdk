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
String deviceID = "YOUR-DEVICE-ID";
String apiKey = "YOUR-PROJECT-APIKEY";
String token = "YOUR-ACCESS-TOKEN";
String ssid = "YOUR-WIFI-SSID";
String passphrase = "YOUR-WIFI-PASSWORD";

// Declaring and initializing other variables
ApolloDevice device;

// Function prototypes
void setupWiFi(void);

void setup() {
  Serial.begin(9600);
  // This sets up the device WiFi.
  setupWiFi();
  // This initializes the SDK's configurations and returns a new object of ApolloDevice class.
  device = apollo.init(deviceID, apiKey, token);
  Serial.printf("\nDevice %s is saying hello to Grandeur Cloud using API Key %s and Access Token %s.\n", device.getDeviceID().c_str(), device.getApiKey().c_str(), device.getToken().c_str());
}

void loop() {
  if(device.getState() == CONNECTED) {
    // When the device's connection with Grandeur Cloud is established, this if-block runs.
    Serial.println("\nDevice has made a successful connection with the Cloud!");
    Serial.println("Grandeur Cloud says hi. Grandeur Cloud will now respond to your commands...");
    Serial.println("Try saving your device state to the Cloud and then retrieving it back.");
    Serial.println("Visit https://grandeur.tech/docs to learn how!");
    Serial.println("Also checkout other examples: \n- DashListening-Device \n- DashListening-App \n- CrossListening.\n");
  }
  // This runs the SDK only when the WiFi is connected.
  device.loop(WiFi.status() == WL_CONNECTED);
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