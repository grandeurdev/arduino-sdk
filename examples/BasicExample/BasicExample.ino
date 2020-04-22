/**
 * @file BasicExample.ino
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Hardware SDK for Grandeur Cloud.
 *
 * Apollo.h is used for device's communication to Grandeur Cloud.
 */

#include <Apollo.h>

String deviceID = "YOUR-DEVICE-ID";
String apiKey = "YOUR-PROJECT-APIKEY";
String token = "YOUR-ACCESS-TOKEN";
String ssid = "YOUR-WIFI-SSID";
String passphrase = "YOUR-WIFI-PASSWORD";

unsigned long current = millis();
ApolloDevice device;

void setup() {
    Serial.begin(9600);
    // Initialize the global object "apollo" with your configurations.
    device = apollo.init(deviceID, apiKey, token, ssid, passphrase);
}

void loop() {
  // This prints the state of the device in loop.
  Serial.println(device.getStringifiedState());

  // Initially, the device is not connected to WiFi (WIFI_NOT_CONNECTED).
  // Then it connects to WiFi using the SSID and Passphrase (WIFI_CONNECTED).
  // And finally it makes connection to Grandeur Cloud using the Device ID,
  // API Key and Access Token (APOLLO_CONNECTED).

  if(device.getState() == WIFI_NOT_CONNECTED) {
    Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n",
      device.getSSID(), device.getPassphrase());
  }
  else if(device.getState() == WIFI_CONNECTED) {
    Serial.printf("\nDevice has successfully connected to WiFi. Its IP Address is: %s\n",
      device.getDeviceIP());
    Serial.printf("\nConnecting the device %s to the Cloud using API Key %s and Access Token %s.\n",
      device.getDeviceID(), device.getApiKey(), device.getToken());
  }
  else if(device.getState() == APOLLO_CONNECTED) {
    Serial.println("\nDevice has made a successful connection with the Cloud.");
    Serial.println("You can now get and set the summary and parms of this device here.");
    Serial.println("Checkout examples \"GettingDataFromCloud\" and \"UpdatingDataOnCloud\". Also run \"FullExample\" to see your device in full action.\n");
  }

  // Keep updating the TCP buffer
  device.update();
}
