/**
 * @file YourDeviceInAction.ino
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
#include <Arduino_JSON.h>

char* deviceID = "YOUR_DEVICE_ID";
char* apiKey = "YOUR_PROJECT'S_API_KEY";
char* token = "YOUR_DEVICE'S_ACCESS_TOKEN";
char* ssid = "YOUR_WIFI'S_SSID";
char* passphrase = "YOUR_WIFI'S_PASSPHRASE";

unsigned long current;
ApolloDevice device;

void setup() {
    Serial.begin(9600);
    // Initialize apollo with your configurations.
    device = apollo.init(deviceID, apiKey, token, ssid, passphrase);

    device.onApolloConnected([](unsigned char* message) {
        // This function runs when the device connects to
        // Grandeur Cloud.
        // Initializing the millis counter for five
        // seconds schedular below.
        current = millis();
    });
}

void loop() {
    if(device.getState() == APOLLO_CONNECTED) {
        if(millis() - current >= 5000) {
            // This if-condition makes sure that the device sends request to Grandeur Cloud
            // every five seconds.
            JSONVar data;
            data["deviceID"] = deviceID;
            // Serializes data to JSON for sending to the cloud.
            String payload = JSON.stringify(data);
            
            // Requests the cloud for device's summary.
            device.getSummary(payload, [](unsigned char* payload) {
                // Parse the JSON coming from the cloud.
                JSONVar data = JSON.parse((char* )payload);

                if (JSON.typeof(data) == "undefined") {
                    // Just for internal errors of Arduino_JSON
                    // if the parsing fails.
                    Serial.println("Parsing input failed!");
                    return;
                }

                if (data.hasOwnProperty("code")) {
                    // Checks if "code" exists in the parsed JSON.
                    if(strcmp((const char* ) data["code"], "DEVICE-SUMMARY-FETCHED") == 0) {
                        // If there were no problems in fetching the device's summary.
                        Serial.printf("voltage: %d, current: %d\n", (int) data["deviceSummary"]["voltage"], (int) data["deviceSummary"]["current"]);
                        
                        /* You can set some pins or trigger events that depend on
                        ** device's summary here.
                        */
                    }
                    else if(strcmp((const char* ) data["code"], "DEVICE-SUMMARY-FETCHING-FAILED") == 0) {
                        // If for some reason, summary could not be fetched.
                        Serial.println("Failed to Fetch Summary");
                    }
                }
            });
            
            // Requests the cloud for device's parms.
            // Since we only need to send the deviceID for fetching parms,
            // we can use the same old payload again.
            device.getParms(payload, [](unsigned char* payload) {
                JSONVar data = JSON.parse((char* )payload);
                if (JSON.typeof(data) == "undefined") {
                    Serial.println("Parsing input failed!");
                    return;
                }
                if (data.hasOwnProperty("code")) {
                    if(strcmp((const char* ) data["code"], "DEVICE-PARMS-FETCHED") == 0) {
                        Serial.printf("state: %d\n", (int) data["deviceParms"]["state"]);

                        /* You can set some pins or trigger events that depend on
                        ** device's parms here.
                        */
                    }
                    else if(strcmp((const char* ) data["code"], "DEVICE-PARMS-FETCHING-FAILED") == 0) {
                        Serial.println("Failed to Fetch Parms");
                    }
                }
            });

            // Since the data object already has the deviceID since
            // we used it above we only add the summary into it.
            // You can read a values from pins or sensors here.
            data["summary"]["voltage"] = millis();
            data["summary"]["current"] = millis();
            payload = JSON.stringify(data);

            // Requests the cloud for setting device's summary
            device.setSummary(payload, [](unsigned char* payload) {
                JSONVar data = JSON.parse((char* )payload);
                if (JSON.typeof(data) == "undefined") {
                    Serial.println("Parsing input failed!");
                    return;
                }
                if (data.hasOwnProperty("code")) {
                    if(strcmp((const char* ) data["code"], "DEVICE-SUMMARY-UPDATED") == 0) {
                        Serial.printf("Voltage is updated to: %d\n", (int) data["update"]["voltage"]);
                        Serial.printf("Current is updated to: %d\n", (int) data["update"]["current"]);

                        /* You can set some pins or trigger events that depend on successful
                        ** device summary update here.
                        */
                    }
                    else if(strcmp((const char*) data["code"], "DEVICE-SUMMARY-UPDATE-FAILED") == 0) {
                        Serial.println("Failed to Update Summary");
                    }
                }
            });

            // This removes summary from the data object and
            // adds parms.
            // You can read a values from pins or sensors here.
            data["summary"] = undefined;
            data["parms"]["state"] = millis();
            payload = JSON.stringify(data);

            // Requests the cloud for setting device's parms
            device.setParms(payload, [](unsigned char* payload) {
                JSONVar data = JSON.parse((char* )payload);
                if (JSON.typeof(data) == "undefined") {
                    Serial.println("Parsing input failed!");
                    return;
                }
                if (data.hasOwnProperty("code")) {
                    if(strcmp((const char* ) data["code"], "DEVICE-PARMS-UPDATED") == 0) {
                        Serial.printf("State is updated to: %d\n", (int) data["update"]["state"]);

                        /* You can set some pins or trigger events that depend on successful
                        ** device parms update here.
                        */
                    }
                    else if(strcmp((const char* ) data["code"], "DEVICE-PARMS-UPDATE-FAILED") == 0) {
                        Serial.println("Failed to Update Parms");
                    }
                }
            });

            // This updates the millis counter for
            // the five seconds scheduler.
            current = millis();
        }
    }
    // Keep updating the TCP buffer
    device.update();
}