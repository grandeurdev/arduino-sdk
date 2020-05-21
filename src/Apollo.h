/**
 * @file Apollo.h
 * @date 20.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for ESP8266 module.
 *
 */

// Including headers
#include <ESP8266WiFi.h>
#include "DuplexClient.h"
#include "Duplex.h"
#include "EventTable.h"
#include "apollotypes.h"
#include "apollomacros.h"

#ifndef APOLLO_H_
#define APOLLO_H_

class Apollo;

class ApolloDevice {
  // Class for handling device related functions
  private:
    // Device IP Address
    static char _deviceIP[IP_SIZE];
    
    // Container for apollo connection callback
    static Callback _connectionCallback;
    // Events Table
    static EventTable _eventsTable;

    // Subscription Array for update handler functions
    static Callback _subscriptions[4];
    
    // Apollo state variable
    static short _state;
    
    void ping();
    
    void _send(const char* task, const char* payload, Callback callback);
    void _subscribe(short event, const char* payload, Callback updateHandler);
      
  public:
    // Device constructor
    ApolloDevice();
    // Getter for Apollo state
    short getState(void);
    char* getStringifiedState(void);

    // Getter methods for Apollo configurations
    char* getSSID(void);
    char* getPassphrase(void);
    char* getDeviceIP(void);
    char* getDeviceID(void);
    char* getApiKey(void);
    char* getToken(void);

    // Async methods
    void getSummary(Callback callback);
    void getParms(Callback callback);
    void setSummary(JSONObject summary, Callback callback);
    void setParms(JSONObject parms, Callback callback);

    // Method to update the device's TCP buffer
    void update(void);

    // When device makes/breaks connection with the Cloud
    void onConnection(Callback connectionHandler);

    // Listeners for events from the Cloud
    void onSummaryUpdated(Callback callback);
    void onParmsUpdated(Callback callback);

    
    friend void apolloEventHandler(WStype_t eventType, uint8_t* packet, size_t length);
    friend void onWiFiDisconnected(const WiFiEventStationModeDisconnected& event);
    friend void onWiFiConnected(const WiFiEventStationModeConnected& event);
    friend class Apollo;
};

class Apollo {
  private:
  public:
    ApolloDevice init(const char* deviceID, const char* apiKey, const char* token, const char* ssid, const char* passphrase);
    ApolloDevice init(String deviceID, String apiKey, String token, String ssid, String passphrase);
};

extern Apollo apollo;

#endif