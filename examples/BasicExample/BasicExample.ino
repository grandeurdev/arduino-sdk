/**
 * @file BasicExample.ino
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 * Apollo.h is used for device's communication to Grandeur Cloud.
 * ESP8266WiFi.h is used for handling device's WiFi.
 */

#include <Apollo.h>
#include <ESP8266WiFi.h>

String deviceID = "YOUR-DEVICE-ID";
String apiKey = "YOUR-PROJECT-APIKEY";
String token = "YOUR-ACCESS-TOKEN";
String ssid = "YOUR-WIFI-SSID";
String passphrase = "YOUR-WIFI-PASSWORD";

ApolloDevice device;

void setupWiFi(void) {
  // Disconnecting WiFi if it"s already connected
  WiFi.disconnect();
  // Setting it to Station mode which basically scans for nearby WiFi routers
  WiFi.mode(WIFI_STA);
  // Begin connecting to WiFi
  WiFi.begin(ssid, passphrase);
  Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid.c_str(), passphrase.c_str());
}

void setup() {
  Serial.begin(9600);
  // This sets up the device WiFi
  setupWiFi();
  // This initializes the SDK's configurations and returns a new object of ApolloDevice class.
  device = apollo.init(deviceID, apiKey, token);
}

void loop() {
  // Initially, the device is not connected to WiFi (wifiConnected = false).
  // Then it connects to WiFi using the SSID and Passphrase (wifiConnected = true).
  // When that happens, the last parameter (the function) passed to apollo.init() returns
  // true and SDK begins connecting with the cloud.
  // Finally it makes connection to Grandeur Cloud using the Device ID,
  // API Key and Access Token (CONNECTED).
  if(WiFi.status() == WL_CONNECTED) {
    if(device.getState() == DISCONNECTED) {
      Serial.printf("\nDevice has successfully connected to WiFi. Its IP Address is: %s\n",
        WiFi.localIP().toString().c_str());
      Serial.printf("\nConnecting the device %s to the Cloud using API Key %s and Access Token %s.\n",
        device.getDeviceID().c_str(), device.getApiKey().c_str(), device.getToken().c_str()); 
    }
    else if(device.getState() == CONNECTED) {
      Serial.println("\nDevice has made a successful connection with the Cloud.");
      Serial.println("You can now get and set the summary and parms of this device here.");
      Serial.println("Checkout examples \"GettingDataFromCloud\" and \"UpdatingDataOnCloud\". Also run \"FullExample\" to see your device in full action.\n");
    }
    // This synchronizes the SDK with the cloud.
    device.update();
  }
}