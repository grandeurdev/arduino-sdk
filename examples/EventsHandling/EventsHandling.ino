/**
 * @file EventsHandling.ino
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Hardware SDK for Grandeur Cloud.
 *
 * Apollo.h is used for device's communication to Grandeur Cloud.
 * 
 * In this example, we set listeners for device's connection and disconnection with the cloud.
 * We also set update handlers for device's Summary and Parms. We see when the device's
 * summary or parms are updated, by the setSummary and setParms functions, these handlers
 * are called with the updated values.
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

    // This sets a callback function to be called when the device makes a successful
    // connection with the Cloud.
    device.onApolloConnected([](JSONObject message) {
      Serial.println("Device is connected to the cloud");
      // Initializing the millis counter for the five
      // seconds timer. So that the timer starts after the device is connected
      // to the cloud.
      current = millis();
    });

    // This sets a callback function to be called when the device's connection with the
    // cloud breaks.
    device.onApolloDisconnected([](JSONObject message) {
      Serial.println("Device is disconnected from the cloud");
    });

    // This sets a callback function to be called when someone changes device's
    // summary on the cloud.
    device.onSummaryUpdated([](JSONObject updatedSummary) {
      Serial.printf("Updated Voltage is: %d\n", (int) updatedSummary["voltage"]);
      Serial.printf("Updated Current is: %d\n", (int) updatedSummary["current"]);
    });

    // This sets a callback function to be called when someone changes device's
    // parms on the cloud.
    device.onParmsUpdated([](JSONObject updatedParms) {
      Serial.printf("Updated State is: %d\n", (int) updatedParms["state"]);  
    });
}

void loop() {
  if(device.getState() == APOLLO_CONNECTED) {
    if(millis() - current >= 5000) {
      // This if-condition makes sure that the code inside this block runs only after
      // every five seconds.

      // This updates the device's summary on the Cloud
      JSONObject summary;
      summary["voltage"] = 3.3;
      summary["current"] = 0.01;
      device.setSummary(summary, [](JSONObject payload) {
        if(payload["code"] == "DEVICE-SUMMARY-UPDATED") {
          Serial.printf("Voltage is updated to: %d\n", (int) payload["update"]["voltage"]);
          Serial.printf("Current is updated to: %d\n", (int) payload["update"]["current"]);
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
          return;
        }

        Serial.println("Failed to Update Parms");
      });

      // This updates the millis counter for
      // the five seconds scheduler.
      current = millis();
    }
  }
  // Keep updating the TCP buffer
  device.update();
}
