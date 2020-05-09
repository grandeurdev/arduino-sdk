/**
 * @file Apollo.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

#include "Apollo.h"
#include <iostream>

/* VARIABLE INITIALIZATIONS */
DuplexClient duplexClient;
unsigned long pingSchedularVariable = 0;
const char* subscriptionTopics[] = {"setDeviceSummary", "setDeviceParms"};
size_t sendQueueSize = 0;
SendData* sendQueue[SENDQUEUE_SIZE] = {};
short ApolloDevice::_state = 0;
Callback ApolloDevice::_handlers[4] = {};
Callback ApolloDevice::_subscriptions[4] = {};
EventTable ApolloDevice::_eventsTable;
Config* config;
char ApolloDevice::_deviceIP[IP_SIZE] = "";
ApolloDevice apolloDevice;
Apollo apollo;

void initializeWiFi(void);
void initializeDuplex(void);

/* EVENT HANDLER FUNCTIONS */
WiFiEventHandler onWiFiConnectedHandler;
WiFiEventHandler onWiFiDisconnectedHandler;

ApolloDevice Apollo::init(const char* deviceID, const char* apiKey, const char* token, const char* ssid, const char* passphrase) {
    // Setting Apollo config
    config = new Config(deviceID, apiKey, token, ssid, passphrase);
    // Initializing wifi
    initializeWiFi();
    // Initializing Duplex connection
    initializeDuplex();
    return apolloDevice;
}

ApolloDevice Apollo::init(String deviceID, String apiKey, String token, String ssid, String passphrase) {
    // Converting all Strings to Char Arrays
    char deviceIDArray[DEVICEID_SIZE], apiKeyArray[APIKEY_SIZE], tokenArray[TOKEN_SIZE];
    char ssidArray[SSID_SIZE], passphraseArray[PASSPHRASE_SIZE];
    deviceID.toCharArray(deviceIDArray, DEVICEID_SIZE);
    apiKey.toCharArray(apiKeyArray, APIKEY_SIZE);
    token.toCharArray(tokenArray, TOKEN_SIZE);
    ssid.toCharArray(ssidArray, SSID_SIZE);
    passphrase.toCharArray(passphraseArray, PASSPHRASE_SIZE);
    
    // Initializing Apollo
    init(deviceIDArray, apiKeyArray, tokenArray, ssidArray, passphraseArray);
}

ApolloDevice::ApolloDevice() {}

void ApolloDevice::_send(const char* task, const char* payload, Callback callback) {
    if(_state != APOLLO_CONNECTED) {
        sendQueue[sendQueueSize++] = new SendData(task, payload, callback);
        return ;
    }
    char packet[PACKET_SIZE];
    ApolloID packetID = millis();
    // Saving callback to eventsTable
    _eventsTable.insert(task, packetID, callback);
    // Formatting the packet
    snprintf(packet, PACKET_SIZE, "{\"header\": {\"id\": %lu, \"task\": \"%s\"}, \"payload\": %s}", packetID, task, payload);
    // Sending to server
    duplexClient.sendTXT(packet);
}

void ApolloDevice::_subscribe(short event, const char* payload, Callback updateHandler) {
    // Saving updateHandler callback to subscriptions Array
    _subscriptions[event] = updateHandler;
    _send("subscribeTopic", payload, [](JSONObject payload) {});
}

void ApolloDevice::getSummary(Callback callback) {
    JSONObject jsonObject;
    char jsonString[PACKET_SIZE];
    jsonObject["deviceID"] = config->deviceID;
    JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
    _send("getDeviceSummary", jsonString, callback);
}

void ApolloDevice::getParms(Callback callback) {
    JSONObject jsonObject;
    char jsonString[PACKET_SIZE];
    jsonObject["deviceID"] = config->deviceID;
    JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
    _send("getDeviceParms", jsonString, callback);
}

void ApolloDevice::setSummary(JSONObject summary, Callback callback) {
    JSONObject jsonObject;
    char jsonString[PACKET_SIZE];
    jsonObject["deviceID"] = config->deviceID;
    jsonObject["summary"] = summary;
    JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
    _send("setDeviceSummary", jsonString, callback);
}

void ApolloDevice::setParms(JSONObject parms, Callback callback) {
    JSONObject jsonObject;
    char jsonString[PACKET_SIZE];
    jsonObject["deviceID"] = config->deviceID;
    jsonObject["parms"] = parms;
    JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
    _send("setDeviceParms", jsonString, callback);
}

char* ApolloDevice::getSSID(void) {
    return config->ssid;
}

char* ApolloDevice::getPassphrase(void) {
    return config->passphrase;
}

char* ApolloDevice::getDeviceIP(void) {
    if(_state != WIFI_NOT_CONNECTED) {
        WiFi.localIP().toString().toCharArray(_deviceIP, IP_SIZE);
    }
    return _deviceIP;
}

char* ApolloDevice::getDeviceID(void) {
    return config->deviceID;
}

char* ApolloDevice::getApiKey(void) {
    return config->apiKey;
}

char* ApolloDevice::getToken(void) {
    return config->token;
}

short ApolloDevice::getState(void) {
    return _state;
}

char* ApolloDevice::getStringifiedState(void) {
    switch(_state) {
        case 0:
            return (char*)"WIFI_NOT_CONNECTED";
        case 1:
            return (char*)"WIFI_CONNECTED";
        case 2:
            return (char*)"APOLLO_CONNECTED";
    }
}

void ApolloDevice::onWiFiConnected(const WiFiEventStationModeConnected& event) {
    // Method called when WiFI gets connected
    // Updating Apollo state
    _state = WIFI_CONNECTED;
    Serial.print("\n*** WIFI CONNECTED ***\n");
}

void ApolloDevice::onWiFiDisconnected(const WiFiEventStationModeDisconnected& event) {
    // Method called when WiFI gets disconnected
    // Updating Apollo state
    _state = WIFI_NOT_CONNECTED;
    Serial.print("\n*** WIFI DISCONNECTED ***\n");
}

void ApolloDevice::onApolloConnected(Callback receiver) {
    _handlers[ONCONNECTED] = receiver;
}

void ApolloDevice::onApolloDisconnected(Callback receiver) {
    _handlers[ONDISCONNECTED] = receiver;
}

void ApolloDevice::onSummaryUpdated(Callback updateHandler) {
    JSONObject jsonObject;
    char jsonString[PACKET_SIZE];
    jsonObject["deviceID"] = config->deviceID;
    jsonObject["event"] = "setDeviceSummary";
    JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
    _subscribe(SUMMARYUPDATE, jsonString, updateHandler);
}

void ApolloDevice::onParmsUpdated(Callback updateHandler) {
    JSONObject jsonObject;
    char jsonString[PACKET_SIZE];
    jsonObject["deviceID"] = config->deviceID;
    jsonObject["event"] = "setDeviceParms";
    JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
    _subscribe(PARMSUPDATE, jsonString, updateHandler);
}

void initializeWiFi(void) {
    // Disconnect WiFi if it"s already connected
    WiFi.disconnect();
    // Set WiFi mode to Station
    WiFi.mode(WIFI_STA);
    // Begin connecting to WiFi
    Serial.printf("\n*** Connecting to WiFi %s using passphrase %s ***\n", config->ssid, config->passphrase);
    WiFi.begin(config->ssid, config->passphrase);
    // Setting WiFi event handlers
    onWiFiConnectedHandler = WiFi.onStationModeConnected(&apolloDevice.onWiFiConnected);
    onWiFiDisconnectedHandler = WiFi.onStationModeDisconnected(&apolloDevice.onWiFiDisconnected);
}

void initializeDuplex(void) {
    duplexClient.beginSSL(APOLLO_URL, APOLLO_PORT, "/?type=device&apiKey=" + String(config->apiKey), APOLLO_FINGERPRINT, "node");
    duplexClient.setAuthorization(config->token);
    // Setting up event handler
    duplexClient.onEvent(&apolloDevice.apolloEventHandler);
    // Scheduling reconnect every 5 seconds if it disconnects
    duplexClient.setReconnectInterval(5000);

    // Setting up device connection event handlers
    apolloDevice.onApolloConnected([](JSONObject message) {
        Serial.println("\n*** APOLLO CONNECTED ***\n");
    });
    apolloDevice.onApolloDisconnected([](JSONObject message) {
        Serial.println("\n*** APOLLO DISCONNECTED ***\n");
    });
}

void ApolloDevice::update(void) {
    if(_state != WIFI_NOT_CONNECTED) {
        // If WiFi is connected
        if(millis() - pingSchedularVariable >= PING_INTERVAL) {
            // Ping Apollo if PING_INTERVAL milliseconds have passed
            pingSchedularVariable += PING_INTERVAL;
            ping();
        }
        // Running duplex loop
        duplexClient.loop();
    }
}


void ApolloDevice::ping() {
    if(_state == APOLLO_CONNECTED) {
        char packet[PING_PACKET_SIZE];
        ApolloID packetID = millis();
        // Saving callback to eventsTable
        _eventsTable.insert("ping", packetID, [](JSONObject feed) {});
        // Formatting the packet
        snprintf(packet, PING_PACKET_SIZE, "{\"header\": {\"id\": %lu, \"task\": \"ping\"}}", packetID);
        // Sending to server
        duplexClient.sendTXT(packet);
    }
}

void ApolloDevice::apolloEventHandler(WStype_t eventType, uint8_t* packet, size_t length) {
    // Printing payload character by character
    switch(eventType) {
        case WStype_CONNECTED:
            // When duplex connection opens
            _state = APOLLO_CONNECTED;
            _handlers[ONCONNECTED](packet);
            // Sending all queued messages
            for(int i = 0; i < sendQueueSize; i++) {
                _send(sendQueue[i]->task, sendQueue[i]->payload, sendQueue[i]->callback);
            }
            return ;

        case WStype_DISCONNECTED:
            // When duplex connection closes
            _state = WIFI_CONNECTED;
            return _handlers[ONDISCONNECTED](packet);

        case WStype_TEXT:
            // When a duplex message is received
            // Fetching task and id from the message packet
            JSONObject messageObject = JSON.parse((char*) packet);
            if (JSON.typeof(messageObject) == "undefined") {
                // Just for internal errors of Arduino_JSON
                // if the parsing fails.
                Serial.println("Parsing input failed!");
                return;
            }
            if(messageObject["header"]["task"] == "update") {
                for(int i = 0; i < NUMBER_OF_TOPICS; i++) {
                    if(messageObject["payload"]["event"] == subscriptionTopics[i]) {
                        return _subscriptions[i](messageObject["payload"]["update"]);
                    }
                }
            }
            // Fetching event callback function from the events Table
            Callback callback = _eventsTable.findAndRemove(
                (const char*) messageObject["header"]["task"],
                (ApolloID) messageObject["header"]["id"]
            );
            if(!callback) {
                return;
            }
            return callback(messageObject["payload"]);
    }
}