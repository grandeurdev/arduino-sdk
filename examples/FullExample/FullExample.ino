/**
 * @file FullExample.ino
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Hardware SDK for Grandeur Cloud.
 *
 * Apollo.h is used for device's communication to Grandeur Cloud.
 * Arduino_JSON.h is for parsing and serializing JSON packets.
 */

#include <Apollo.h>

const char* deviceID = "YOUR-DEVICE-ID";
const char* apiKey = "YOUR-PROJECT-APIKEY";
const char* token = "YOUR-ACCESS-TOKEN";
const char* ssid = "YOUR-WIFI-SSID";
const char* passphrase = "YOUR-WIFI-PASSWORD";

unsigned long current;
short numberOfTimes = 2;
ApolloDevice device;

void setup() {
    Serial.begin(9600);
    // Initialize apollo with your configurations.
    device = apollo.init(deviceID, apiKey, token, ssid, passphrase);

    device.onApolloConnected([](JSONObject message) {
        // This function runs when the device connects to
        // Grandeur Cloud.
        // Initializing the millis counter for five
        // seconds schedular below.
        current = millis();
        Serial.println("BOOOOOOOOOOOTING UP");

        for(int i = numberOfTimes; i > 0; i--) {
            device.onSummaryUpdated([](JSONObject updatedSummary) {
                Serial.printf("Updated Summary is: %s\n", (const char*) updatedSummary["voltage"]);
                /*JSONVar summary = JSON.parse((char* )updatedSummary);
                if (JSON.typeof(summary) == "undefined") {
                    Serial.println("Parsing input failed!");
                    return;
                }
    
                Serial.printf("voltage: %d, current: %d\n", (int) summary["voltage"], (int) summary["current"]);
    
                /* Here you can set some pins or trigger events that depend on
                ** device's summary update.
                */
            });
    
            device.onParmsUpdated([](JSONObject updatedParms) {
                Serial.printf("Updated Parms is: %s\n", (const char*) updatedParms["state"]);
                /*JSONVar parms = JSON.parse((char* )updatedParms);
                if (JSON.typeof(parms) == "undefined") {
                    Serial.println("Parsing input failed!");
                    return;
                }
    
                Serial.printf("state: %d\n", (int) parms["state"]);
    
                /* Here you can set some pins or trigger events that depend on
                ** device's parms update.
                */
            });
        }
    });
}

void loop() {
    if(device.getState() == APOLLO_CONNECTED) {
        if(millis() - current >= 5000) {
            // This if-condition makes sure that the device sends the below requests to Grandeur Cloud
            // every five seconds.
            
            // Requests the cloud for device's summary.
            device.getSummary([](JSONObject payload) {
                if(payload["code"] == "DEVICE-SUMMARY-FETCHED") {
                    // If there were no problems in fetching the device's summary.
                    Serial.printf("voltage: %d, current: %d\n", (int) payload["deviceSummary"]["voltage"], (int) payload["deviceSummary"]["current"]);
                    
                    /* You can set some pins or trigger events that depend on
                    ** device's summary here.
                    */

                    return;
                }
                // If for some reason, summary could not be fetched.
                Serial.println("Failed to Fetch Summary");
                return;
            });
            
            // Requests the cloud for device's parms.
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

            // Since the data object already has the deviceID since
            // we used it above we only add the summary into it.
            // You can read a values from pins or sensors here.
            JSONObject summary;
            summary["voltage"] = millis();
            summary["current"] = millis();
            // Requests the cloud for setting device's summary
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

            // This removes summary from the data object and
            // adds parms.
            // You can read a values from pins or sensors here.
            JSONObject parms;
            parms["state"] = millis();
            // Requests the cloud for setting device's parms
            device.setParms(parms, [](JSONObject payload) {
                if(payload["code"] == "DEVICE-PARMS-UPDATED") {
                    Serial.printf("State is updated to: %d\n", (int) payload["update"]["state"]);

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
