// Including headers
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <cstring>
#include "DuplexClient.h"
#include "EventTable.h"
#include "types.h"

#ifndef APOLLO_H_
#define APOLLO_H_

// Connection macros
#define APOLLO_URL "10.50.202.122"
#define APOLLO_PORT 3000
#define APOLLO_FINGERPRINT ""

// Defining general macros
#define MAX_CONFIG_TRIES 30
#define SSID_SIZE 32
#define PASSPHRASE_SIZE 32
#define APIKEY_SIZE 32
#define TOKEN_SIZE 512
#define IP_SIZE 16
#define FINGERPRINT_SIZE 256

// Ping interval
#define PING_INTERVAL 5

// Defining macros for Apollo states
#define WIFI_NOT_CONNECTED 0
#define WIFI_CONNECTED 1
#define DUPLEX_CONNECTED 2

// Indexes for functions in eventQueue
#define LOGOUT 0

// Indexes for handlers callbacks
#define ONCONNECTED 0
#define ONDISCONNECTED 1
#define ONMESSAGE 2

// Config structure for apollo configurations
typedef struct {
    char apiKey[APIKEY_SIZE];
    char token[TOKEN_SIZE];
    char ssid[SSID_SIZE];
    char passphrase[PASSPHRASE_SIZE];
} Config;

class ApolloHandler {
    private:
        // Config struct
        static Config _config;
        // Apollo state variable
        static int _state;
        // Device IP Address
        static char _deviceIP[IP_SIZE];
        // Events Table
        static EventTable eventsTable;
        
        // Handler classes
        class WiFiHandler {
            // Class for handling WiFi of the device
            private:
                // Method to stop WiFi connection loop
                void stopConnectionLoop(void);

                // Event handlers
                static void onWiFiConnected(const WiFiEventStationModeConnected& event);
                static void onWiFiDisconnected(const WiFiEventStationModeDisconnected& event);

            public:
                // WiFi initializer
                void init(void);
                // Method to begin connecting to WiFI
                void connect(void);
                // Method to begin quick configuration
                Config quickConfigure(void);

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
                void init(String apiKey, String token);
                // Method to read/write data from client buffer
                void update(void);
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
                int getSummary(Payload* payload, Callback callback);
                int getParms(Payload* payload, Callback callback);
                int setSummary(Payload* payload, Callback callback);
                int setParms(Payload* payload, Callback callback);
                // Method to subscribe to a topic
                void subscribe(void);
                // Method to unsubscribe from a topic
                void unsubscribe(void);
        };


    public:
        // Apollo init Method
        void init(Config configuration);

        // Getter for Apollo state
        static char* getState(void);

        // Handler objects
        WiFiHandler wifi;
        DuplexHandler duplex;
        DeviceHandler device;
        PayloadHandler format;
};

ApolloHandler apollo;

#endif