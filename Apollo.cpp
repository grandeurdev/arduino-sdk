#include "Apollo.h"
#include <iostream>

/* GLOBAL VARIABLES */
DuplexClient duplexClient;
unsigned long pingSchedularVariable = 0;
short ApolloDevice::_state = 0;
Callback ApolloDevice::_handlers[16] = {};
EventTable ApolloDevice::_eventsTable;
Config* config;
ApolloDevice::PayloadHandler ApolloDevice::_format;
char ApolloDevice::_deviceIP[IP_SIZE] = "";


/* EVENT HANDLER FUNCTIONS */
WiFiEventHandler onWiFiConnectedHandler;
WiFiEventHandler onWiFiDisconnectedHandler;

ApolloDevice* Apollo::init(char* deviceID, char* apiKey, char* token, char* ssid, char* passphrase) {
    // Setting Apollo config
    config = new Config(deviceID, apiKey, token, ssid, passphrase);
    return new ApolloDevice();
}

ApolloDevice::ApolloDevice() {
    // Initializing wifi
    initializeWiFi();
    
    // Initializing duplex connection
    duplexClient.begin(APOLLO_URL, APOLLO_PORT, "/?type=device&apiKey=" + String(config->apiKey) + "&token=" + String(config->token), "node");
    //client.beginSSL(APOLLO_URL, APOLLO_PORT, "/?type=device&apiKey=" + String(config->apiKey) + "&token=" + String(config->token), APOLLO_FINGERPRINT, "node");
    // Setting up event handler
    duplexClient.onEvent(&apolloEventHandler);
    // Scheduling reconnect every 5 seconds if it disconnects
    duplexClient.setReconnectInterval(5000);

    // Setting up device connection event handlers
    onApolloConnected([](uint8_t* message) {
        Serial.println("\n*** APOLLO CONNECTED ***\n");
    });
    onApolloDisconnected([](uint8_t* message) {
        Serial.println("\n*** APOLLO DISCONNECTED ***\n");
    });
}


void ApolloDevice::getSummary(char* payload, Callback callback) {
    if(_state == APOLLO_CONNECTED) {
        char* packet= "";
        unsigned long packetID = millis();
        // Saving callback to eventsTable
        _eventsTable.insert("getDeviceSummary", packetID, callback);
        // Formatting the packet
        sprintf(packet, "{\"header\": {\"id\": %lu, \"task\": \"getDeviceSummary\"}, \"payload\": %s}", packetID, payload);
        // Sending to server
        duplexClient.sendTXT(packet);
    }
}

void ApolloDevice::getParms(char* payload, Callback callback) {
    if(_state == APOLLO_CONNECTED) {
        char* packet= "";
        unsigned long packetID = millis();
        // Saving callback to eventsTable
        _eventsTable.insert("getDeviceParms", packetID, callback);
        // Formatting the packet
        sprintf(packet, "{\"header\": {\"id\": %lu, \"task\": \"getDeviceParms\"}, \"payload\": %s}", packetID, payload);
        // Sending to server
        duplexClient.sendTXT(packet);
    }
}

void ApolloDevice::setSummary(char* payload, Callback callback) {
    if(_state == APOLLO_CONNECTED) {
        char* packet= "";
        unsigned long packetID = millis();
        // Saving callback to eventsTable
        _eventsTable.insert("setDeviceSummary", packetID, callback);
        // Formatting the packet // TODO: How to add variable number of summary keys
        sprintf(packet, "{\"header\": {\"id\": %lu, \"task\": \"setDeviceSummary\"}, \"payload\": %s}", packetID, payload);
        // Sending to server
        duplexClient.sendTXT(packet);
    }
}

void ApolloDevice::setParms(char* payload, Callback callback) {
    if(_state == APOLLO_CONNECTED) {
        char* packet= "";
        unsigned long packetID = millis();
        // Saving callback to eventsTable
        _eventsTable.insert("setDeviceParms", packetID, callback);
        // Formatting the packet // TODO: How to add variable number of summary keys
        sprintf(packet, "{\"header\": {\"id\": %lu, \"task\": \"setDeviceParms\"}, \"payload\": %s}", packetID, payload);
        // Sending to server
        duplexClient.sendTXT(packet);
    }
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
            return "WIFI_NOT_CONNECTED";
        case 1:
            return "WIFI_CONNECTED";
        case 2:
            return "APOLLO_CONNECTED";
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

void ApolloDevice::apolloEventHandler(WStype_t eventType, uint8_t* packet, size_t length) {
    // Printing payload character by character
    switch(eventType) {
        case WStype_CONNECTED:
            // When duplex connection opens
            _state = APOLLO_CONNECTED;
            _handlers[ONCONNECTED](packet);
            break;

        case WStype_DISCONNECTED:
            // When duplex connection closes
            _state = WIFI_CONNECTED;
            _handlers[ONDISCONNECTED](packet);
            break;

        case WStype_TEXT:
            // When a duplex message is received
            // Fetching task and id from the message packet
            char* headerKeys[] = {"task", "id"};
            char* headerValues[2];
            Feed* header = new Feed(2, headerKeys, headerValues);
            if(_format.getFeed(packet, header, 20) == 0) {
                // Fetching event callback function from the events Table
                Callback callback = _eventsTable.findAndRemove(header->values[0], (EventID) strtoul(header->values[1], NULL, 0));
                if(callback != NULL) {
                    char* data = strstr((char*) packet, "payload");
                    if(data != NULL) {
                        char* payload = strchr(data, '{');
                        payload[strlen(payload) - 1] = '\0';
                        callback((unsigned char*) payload);
                    }
                }
            }
            delete header;
            break;
    }
}

void ApolloDevice::onApolloConnected(Callback receiver) {
    _handlers[ONCONNECTED] = receiver;
}

void ApolloDevice::onApolloDisconnected(Callback receiver) {
    _handlers[ONDISCONNECTED] = receiver;
}

void ApolloDevice::initializeWiFi(void) {
    // Disconnect WiFi if it"s already connected
    WiFi.disconnect();
    // Set WiFi mode to Station
    WiFi.mode(WIFI_STA);
    // Begin connecting to WiFi
    Serial.printf("\n*** Connecting to WiFi %s using passphrase %s ***\n", config->ssid, config->passphrase);
    WiFi.begin(config->ssid, config->passphrase);
    // Setting WiFi event handlers
    onWiFiConnectedHandler = WiFi.onStationModeConnected(&onWiFiConnected);
    onWiFiDisconnectedHandler = WiFi.onStationModeDisconnected(&onWiFiDisconnected);
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
        char* payload= "";
        unsigned long packetID = millis();
        // Saving callback to eventsTable
        _eventsTable.insert("pong", packetID, [](unsigned char* feed) {});
        // Formatting the packet
        sprintf(payload, "{\"header\": {\"id\": %lu, \"task\": \"ping\"}}", packetID);
        // Sending to server
        duplexClient.sendTXT(payload);
    }
}


int ApolloDevice::PayloadHandler::getValue(uint8_t* packet, char* key, char* value) {
    if(*key == '\0') {
        // If key is empty
        value[0] = '\0';
        return 0;
    }
    // Converting uint8_t to char*
    char* data = (char*) packet;
    
    // Finding key in the payload
    char *keyPtr = strstr(data, key);
    if (keyPtr != NULL) {
        // If key is found in the payload
        int start = false;
        
        // Traversing it to find the value
        for (int i = 0, j = 0; i < strlen(keyPtr); i++) {
            if (keyPtr[i] == ',' || keyPtr[i] == '}') {
                // Ending copying condition
                if(keyPtr[i - 1] == '"')
                    j--;
                value[j] = '\0';
                break;
            }

            if (start) {
                // Copying the value
                value[j++] = keyPtr[i];
            }

            if (keyPtr[i] == ':') {
                while(keyPtr[i + 1] == ' ')
                    i++;
                if(keyPtr[i + 1] == '"')
                    i++;
                // Start copying condition
                start = true;
            }
        }
        // If value is copied
        return 0;
    }

    // If value is not found
    return -1;
}

int ApolloDevice::PayloadHandler::getValues(uint8_t* packet, char** keys, uint_fast16_t numberOfKeys, char** values, uint_fast16_t maxSizeOfValue) {
    for(int i = 0; i < numberOfKeys; i++) {
        char* value = new char[maxSizeOfValue];
        if(getValue(packet, keys[i], value) == 0) {
            values[i] = new char[maxSizeOfValue];
            memcpy(values[i], value, sizeof(char) * maxSizeOfValue);
        }
        else {
            return -1;
        }
    }
    return 0;
}

int ApolloDevice::PayloadHandler::getPayloadKeys(uint8_t* packet, char** keys, size_t numberOfKeys, size_t sizeOfKey) {
    // Traversing to the starting bracket of the payload
    char* data = strstr((char*) packet, "payload");
    if(data == NULL) {
        // If no payload is present in the packet
        for(int i = 0; i < numberOfKeys; i++) {
            keys[i] = new char[sizeOfKey];
            *keys[i] = '\0';
        }
        return 0;
    }
    data = strchr(data, '{');
    int brackets = 1;
    int start = false;
    for(int i = 0, j = 0, c = 0; i < strlen(data); i++) {
        // Traversing the payload
        char* temp;
        if(data[i] == '}') {
            if(--brackets == 0) {
                // If closing bracket of payload is reached
                while(j < numberOfKeys) {
                    keys[j] = new char[sizeOfKey];
                    *keys[j++] = '\0';
                }
                break;
            }
        }
        if(!start && data[i] == ':') {
            // Ignoring value in key-value pair
            while(data[i] != ',') {
                if(data[i] == '{') {
                    brackets++;
                    j--;
                    break;
                }
                if(data[i] == '}') {
                    brackets--;
                    break;
                }
                i++;
            }
        }

        if (start && data[i] == '"') {
            // Ending copying condition
            temp[c] = '\0';
            start = false;
            keys[j++] = temp;
            if(j >= numberOfKeys)
                // If limit on number of keys is reached, breaking out of loop
                break;
            // Going to the start of the loop
            continue;
        }

        if (start) {
            // Copying the value
            temp[c++] = data[i];
        }

        if (!start && data[i] == '"' && data[i + 1] != ':') {
            // Start copying condition
            start = true;
            temp = new char[sizeOfKey];
            c = 0;
        }
    }
    if(start) {
        // If closing condition was not satisfied
        return -1;
    }
    return 0;
}

int ApolloDevice::PayloadHandler::getFeed(uint8_t* packet, Feed* feed, uint_fast16_t maxSizeOfValue = 10) {
    if(getValues(packet, feed->keys, feed->numberOfKeys, feed->values, maxSizeOfValue) == 0) {
        return 0;
    }
    else {
        return -1;
    }
}