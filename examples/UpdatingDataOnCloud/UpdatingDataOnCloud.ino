/**
 * @file FullExample.ino
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
  if(device.getState() == APOLLO_CONNECTED) {
    if(millis() - current >= 5000) {
      // This if-condition makes sure that the code inside this block runs only after
      // every five seconds.

      // This updates the device's summary on the Cloud
      JSONObject summary;
      summary["voltage"] = 5;
      summary["current"] = 0.1;
      device.setSummary(summary, [](JSONObject payload) {
        if(payload["code"] == "DEVICE-SUMMARY-UPDATED") {
          Serial.printf("Voltage is updated to: %d\n", (int) payload["update"]["voltage"]);
          Serial.printf("Current is updated to: %d\n", (int) payload["update"]["current"]);

          /* You can set some pins or trigger events that depend on successful
          ** device summary update here.
          */
          return;
        }
        Serial.println("Failed to Update Summary");
      });

      // This updates the device's parms to "true" on the Cloud
      JSONObject parms;
      parms["state"] = true;
      device.setParms(parms, [](JSONObject payload) {
        if(payload["code"] == "DEVICE-PARMS-UPDATED") {
          Serial.printf("State is updated to: %d\n", (bool) payload["update"]["state"]);

          /* You can set some pins or trigger events that depend on successful
          ** device parms update here.
          */
          return;
        }
        Serial.println("Failed to Update Parms");
        return;
      });

      // This updates the millis counter for
      // the five seconds scheduler.
      current = millis();
    }
  }
  // Keep updating the TCP buffer
  device.update();
}