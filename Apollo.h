/**
 * @file Apollo.h
 * @date 20.03.2020
 * @author Muhammad Abdullah
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Hardware SDK for ESP8266 module.
 *
 */

// Including headers
#include <ESP8266WiFi.h>
#include <DuplexClient.h>
#include <Duplex.h>
#include <EventTable.h>
#include <apollotypes.h>
#include <apollomacros.h>

#ifndef APOLLO_H_
#define APOLLO_H_

class ApolloDevice {
    // Class for handling device related functions
    private:
        class PayloadHandler {
            // Class for handling payload format to or from apollo server
            private:
                
            public:
                int getPayloadKeys(uint8_t* packet, char** keys, size_t numberOfKeys, size_t sizeOfKey);
                int getFeed(uint8_t* packet, Feed* feed, uint_fast16_t maxSizeOfValue);
                int getValues(uint8_t* packet, char** keys, uint_fast16_t numberOfKeys, char** values, uint_fast16_t maxSizeOfValues);
                int getValue(uint8_t* packet, char* key, char* value);
        };

        // Device IP Address
        static char _deviceIP[IP_SIZE];
        
        // Handlers array for callback functions
        static Callback _handlers[16];
        // Events Table
        static EventTable _eventsTable;
        
        // Apollo state variable
        static short _state;
        
        static void ping();
        
        static PayloadHandler _format;
    public:
        // Device constructor
        ApolloDevice();
        // Getter for Apollo state
        short getState(void);
        char* getStringifiedState(void);
        
        static void apolloEventHandler(WStype_t eventType, uint8_t* packet, size_t length);

        // Getter Methods
        char* getSSID(void);
        char* getPassphrase(void);
        char* getDeviceIP(void);
        char* getApiKey(void);
        char* getToken(void);
        void getSummary(char* payload, Callback callback);
        void getParms(char* payload, Callback callback);
        // Setter Methods
        void setSummary(char* payload, Callback callback);
        void setParms(char* payload, Callback callback);

        // Method to read/write data from client buffer
        void update(void);

        // Event handlers
        static void onWiFiConnected(const WiFiEventStationModeConnected& event);
        static void onWiFiDisconnected(const WiFiEventStationModeDisconnected& event);
        void onApolloConnected(Callback receiver);
        void onApolloDisconnected(Callback receiver);

};

class Apollo {
    private:
    public:
        // Apollo init Method
        ApolloDevice init(char* deviceID, char* apiKey, char* token, char* ssid, char* passphrase);
};

extern Apollo apollo;

#endif