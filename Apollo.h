// Including headers
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include "DuplexClient.h"
#include "EventTable.h"
#include "apollotypes.h"
#include "apollomacros.h"

#ifndef APOLLO_H_
#define APOLLO_H_

class ApolloHandler {
    private:
        // Handler classes
        class WiFiHandler {
            // Class for handling WiFi of the device
            private:
                // Event handlers
                static void onWiFiConnected(const WiFiEventStationModeConnected& event);
                static void onWiFiDisconnected(const WiFiEventStationModeDisconnected& event);

            public:
                // WiFi initializer
                void init(void);
                // Method to begin quick configuration
                //Config quickConfigure(void);

                // Getter Methods
                char* getSSID(void);
                char* getPassphrase(void);
                char* getDeviceIP(void);
        };

        class DuplexHandler {
            // Class for handling duplex connection of the device to Apollo server
            private:
                // Handlers array for callback functions
                static Receiver handlers[16];
                // Event handler
                static void eventHandler(WStype_t eventType, uint8_t* packet, size_t length);

                static void ping();
            public:
                // Getter Methods for duplex apiKey and Token
                char* getApiKey(void);
                char* getToken(void);

                // Duplex initializer
                void init();
                // Event handlers
                void onConnected(Receiver receiver);
                void onDisconnected(Receiver receiver);
        };

        class PayloadHandler {
            // Class for handling payload format to or from apollo server
            private:
                
            public:
                int getPayloadKeys(uint8_t* packet, char** keys, size_t numberOfKeys, size_t sizeOfKey);
                int getPayload(uint8_t* packet, Payload* payload, uint_fast16_t maxSizeOfValue);
                int getValues(uint8_t* payload, char** keys, uint_fast16_t numberOfKeys, char** values, uint_fast16_t maxSizeOfValues);
                int getValue(uint8_t* payload, char* key, char* value);
        };

        class DeviceHandler {
            // Class for handling device related functions
            private:
            public:
                int getSummary(Payload* feedOut, Callback callback);
                int getParms(Payload* feedOut, Callback callback);
                int setSummary(Payload* summary, Payload* feedOut, Callback callback);
                int setParms(Payload* parms, Payload* feedOut, Callback callback);
        };

        // Apollo state variable
        static int _state;
        // Device IP Address
        static char _deviceIP[IP_SIZE];
        // Events Table
        static EventTable eventsTable;

        DuplexHandler duplex;

    public:
        // Apollo init Method
        void init(char* deviceID, char* apiKey, char* token, char* ssid, char* passphrase);

        // Getter for Apollo state
        static char* getState(void);
        
        // Method to read/write data from client buffer
        void update(void);

        // Handler objects
        WiFiHandler wifi;
        DeviceHandler device;
        PayloadHandler format;
};

ApolloHandler apollo;

#endif