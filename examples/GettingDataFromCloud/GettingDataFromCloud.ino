/**
 * @file GettingDataFromCloud.ino
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 * Apollo.h is used for device's communication to Grandeur Cloud.
 * ESP8266WiFi.h is used for handling device's WiFi.
 * 
 * This example connects to the WiFi using the provided WiFi credentials. Then it
 * connects to the cloud using the API Key and Access Token and starts fetching
 * the Summary and Parms of this device from the cloud every 5 seconds.
*/

#include <Apollo.h>
#include <ESP8266WiFi.h>

String deviceID = "YOUR-DEVICE-ID";
String apiKey = "YOUR-PROJECT-APIKEY";
String token = "YOUR-ACCESS-TOKEN";
String ssid = "YOUR-WIFI-SSID";
String passphrase = "YOUR-WIFI-PASSWORD";

unsigned long current = millis();
ApolloDevice device;
bool wifiConnected = false;
WiFiEventHandler onWiFiConnectedHandler;
WiFiEventHandler onWiFiDisconnectedHandler;

void setupWiFi(void) {
  // Disconnecting WiFi if it"s already connected
  WiFi.disconnect();
  // Setting it to Station mode which basically scans for nearby WiFi routers
  WiFi.mode(WIFI_STA);
  // Setting WiFi event handlers
  onWiFiConnectedHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
    // This runs when the device connects with WiFi.
    wifiConnected = true;
    Serial.printf("\nDevice has successfully connected to WiFi. Its IP Address is: %s\n",
      WiFi.localIP().toString().c_str());
  });
  onWiFiDisconnectedHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
    // This runs when the device disconnects with WiFi.
    wifiConnected = false;
    Serial.println("Device is disconnected from WiFi.");
  });
  // Begin connecting to WiFi
  WiFi.begin(ssid, passphrase);
  Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid.c_str(), passphrase.c_str());
}

void setup() {
  Serial.begin(9600);
  // This sets up the device WiFi
  setupWiFi();
  // This initializes the SDK's configurations and returns a new object of ApolloDevice class.
  device = apollo.init(deviceID, apiKey, token, []() {
    return wifiConnected;
  });
}

void loop() {
  if(device.getState() == APOLLO_CONNECTED) {
    if(millis() - current >= 5000) {
      // This if-condition makes sure that the code inside this block runs only after
      // every five seconds.
      
      // Requests the cloud for device's summary.
      device.getSummary([](JSONObject payload) {
        if(payload["code"] == "DEVICE-SUMMARY-FETCHED") {
          // If there were no problems in fetching the device's summary.
          Serial.printf("voltage: %d, current: %d\n",
              (int) payload["deviceSummary"]["voltage"],
              (int) payload["deviceSummary"]["current"])
          ;

          /* You can set some pins or trigger events that depend on
          ** device's summary here.
          */

          return;
        }
        // If for some reason, summary could not be fetched.
        Serial.println("Failed to Fetch Summary");
        return;
      });
      
      // Gets the device's parms from the Cloud
      device.getParms([](JSONObject payload) {
        if(payload["code"] == "DEVICE-PARMS-FETCHED") {
          Serial.printf("state: %d\n", (int) payload["deviceParms"]["state"]);

          /* You can set some pins or trigger events that depend on
          ** device's parms here.
          */
          return;
        }
        Serial.println("Failed to Fetch Parms");
        return;
      });

      // This updates the millis counter for
      // the five seconds scheduler.
      current = millis();
    }
  }
  // This synchronizes the SDK with the cloud.
  device.update();
}