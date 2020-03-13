// Including headers
#include <ESP8266WiFi.h>
#include "DuplexClient.h"
#include "EventTable.h"
#include "apollotypes.h"
#include "apollomacros.h"

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

        
        // WiFi initializer
        static void initializeWiFi(void);
        // Event handlers
        static void onWiFiConnected(const WiFiEventStationModeConnected& event);
        static void onWiFiDisconnected(const WiFiEventStationModeDisconnected& event);

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
        static short getState(void);
        static char* getStringifiedState(void);
        
        static void apolloEventHandler(WStype_t eventType, uint8_t* packet, size_t length);

        // Getter Methods
        static char* getSSID(void);
        static char* getPassphrase(void);
        static char* getDeviceIP(void);
        static char* getApiKey(void);
        static char* getToken(void);
        static void getSummary(char* payload, Callback callback);
        static void getParms(char* payload, Callback callback);
        // Setter Methods
        static void setSummary(char* payload, Callback callback);
        static void setParms(char* payload, Callback callback);

        // Method to read/write data from client buffer
        static void update(void);

        // Event handlers
        static void onApolloConnected(Callback receiver);
        static void onApolloDisconnected(Callback receiver);

};

class Apollo {
    private:
    public:
        // Apollo init Method
        ApolloDevice* init(char* deviceID, char* apiKey, char* token, char* ssid, char* passphrase);
};

Apollo apollo;

#endif