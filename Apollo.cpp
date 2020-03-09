#include "Apollo.h"
#include <iostream>

/* STATIC VARIABLES */
char ApolloHandler::_deviceIP[] = "";
int ApolloHandler::_state = 0;
Receiver ApolloHandler::DuplexHandler::handlers[] = {};
EventTable ApolloHandler::eventsTable;
Config* config;

/* GLOBAL VARIABLES */
// DuplexClient object
DuplexClient client;
// Ticker object for ping
Ticker ticker;

/* EVENT HANDLER FUNCTIONS */
WiFiEventHandler onWiFiConnectedHandler;
WiFiEventHandler onWiFiDisconnectedHandler;

void ApolloHandler::WiFiHandler::onWiFiConnected(const WiFiEventStationModeConnected& event) {
    // Method called when WiFI gets connected
    // Updating Apollo state
    _state = WIFI_CONNECTED;
    Serial.println("WIFI CONNECTED");
}

void ApolloHandler::WiFiHandler::onWiFiDisconnected(const WiFiEventStationModeDisconnected& event) {
    // Method called when WiFI gets disconnected
    // Updating Apollo state
    _state = WIFI_NOT_CONNECTED;
    Serial.println("WIFI DISCONNECTED");
}

void onDuplexConnected(uint8_t* message) {
    Serial.println("DUPLEX CONNECTED");
}

void onDuplexDisconnected(uint8_t* message) {
    Serial.println("DUPLEX DISCONNECTED");
}

void ApolloHandler::DuplexHandler::eventHandler(WStype_t eventType, uint8_t* packet, size_t length) {
    // Printing payload character by character
    switch(eventType) {
        case WStype_CONNECTED:
            // When duplex connection opens
            _state = DUPLEX_CONNECTED;
            handlers[ONCONNECTED](packet);
            break;

        case WStype_DISCONNECTED:
            // When duplex connection closes
            _state = WIFI_CONNECTED;
            handlers[ONDISCONNECTED](packet);
            break;

        case WStype_TEXT:
            Serial.printf("Received packet is: %s\n", packet);
            // When a duplex message is received
            // Fetching task and id from the message packet
            
            char* headerKeys[] = {"task", "id"};
            char* headerValues[2];
            Payload* header = new Payload(2, headerKeys, headerValues);
            if(apollo.format.getPayload(packet, header, 20) == 0) {
                // Fetching event callback function from the events Table
                eventsTable.print();
                EventData eventCallback = eventsTable.findAndRemove(header->values[0], (EventID) strtoul(header->values[1], NULL, 0));
                eventsTable.print();
                if(eventCallback != NULL) {
                    char* payloadKeys[1];
                    char* payloadValues[1];
                    // If event callback is found
                    if(apollo.format.getPayloadKeys(packet, payloadKeys, 1, 20) == 0) {
                        Payload* payload = new Payload(6, payloadKeys, payloadValues);
                        if(apollo.format.getPayload(packet, payload, 50) == 0) {
                            // Calling the callback function
                            eventCallback(payload);
                        }
                        delete payload;
                    }
                }
            }
            delete header;
            
            break;
    }
}

void ApolloHandler::DuplexHandler::onConnected(Receiver receiver) {
    handlers[ONCONNECTED] = receiver;
}

void ApolloHandler::DuplexHandler::onDisconnected(Receiver receiver) {
    handlers[ONDISCONNECTED] = receiver;
}

/* INITIALIZERS */
void ApolloHandler::init(char* apiKey, char* token, char* ssid, char* passphrase) {
    // Setting Apollo config
    config = new Config(apiKey, token, ssid, passphrase);
    // Initializing wifi
    wifi.init();
    // Initializing duplex connection
    duplex.init();
    // Setting up connection event handlers
    duplex.onConnected(onDuplexConnected);
    duplex.onDisconnected(onDuplexDisconnected);
}

void ApolloHandler::WiFiHandler::init(void) {
    // Disconnect WiFi if it"s already connected
    WiFi.disconnect();
    // Set WiFi mode to Station
    WiFi.mode(WIFI_STA);
    // Begin connecting to WiFi
    Serial.printf("Connecting to WiFi %s using passphrase %s\n", config->ssid, config->passphrase);
    WiFi.begin(config->ssid, config->passphrase);
    // Setting WiFi event handlers
    onWiFiConnectedHandler = WiFi.onStationModeConnected(&onWiFiConnected);
    onWiFiDisconnectedHandler = WiFi.onStationModeDisconnected(&onWiFiDisconnected);
}

void ApolloHandler::DuplexHandler::init() {
    // Starting duplex client
    client.begin(APOLLO_URL, APOLLO_PORT, "/?type=device&apiKey=" + String(config->apiKey) + "&token=" + String(config->token), "node");
    //client.beginSSL(APOLLO_URL, APOLLO_PORT, "/?type=device&apiKey=" + String(config->apiKey) + "&token=" + String(config->token), APOLLO_FINGERPRINT, "node");
    client.onEvent(&eventHandler);
    // Reconnecting every 5 seconds if disconnects
    client.setReconnectInterval(5000);
    // Attaching ticker ot ping function to call it every 25 seconds
    ticker.attach(PING_INTERVAL, ping);
}

/*
Config ApolloHandler::WiFiHandler::quickConfigure(void) {
    int configTries = 1;
    Serial.println("\n\n Starting Quick Configuration\n");
    // Starting Smart Configuration
    WiFi.beginSmartConfig();
    for(int configTries; !WiFi.smartConfigDone() && configTries <= MAX_CONFIG_TRIES; configTries++) {
        // Waiting for smart configuration to complete
        // Showing visual output, meanwhile
        Serial.print(".");
        delay(1000);
    }
    
    if(WiFi.smartConfigDone()) {
        // Fetching SSID and passphrase from smart comfiguration
        WiFi.SSID().toCharArray(_config.ssid, SSID_SIZE);
        WiFi.psk().toCharArray(_config.passphrase, PASSPHRASE_SIZE);
        
        Serial.println("\n Quick configuration is successfully completed. Config is updated.");
        // Returning new Apollo configuration
        return _config;
    }
    
    // Stopping Smart Configuration
    WiFi.stopSmartConfig();
    Serial.println("\n Quick configuration timed out.");
}
*/

/* GETTER METHODS */
char* ApolloHandler::getState(void) {
    switch(_state) {
        case 0:
            return "WIFI_NOT_CONNECTED";
        case 1:
            return "WIFI_CONNECTED";
        case 2:
            return "DUPLEX_CONNECTED";
    }
}

char* ApolloHandler::WiFiHandler::getSSID(void) {
    return config->ssid;
}

char* ApolloHandler::WiFiHandler::getPassphrase(void) {
    return config->passphrase;
}

char* ApolloHandler::WiFiHandler::getDeviceIP(void) {
    // TODO: Fetch IP only once and store it in _deviceIP.
    WiFi.localIP().toString().toCharArray(_deviceIP, IP_SIZE);
    return _deviceIP;
}

// DuplexHandler functions
char* ApolloHandler::DuplexHandler::getApiKey(void) {
    return config->apiKey;
}

char* ApolloHandler::DuplexHandler::getToken(void) {
    return config->token;
}

unsigned long previousMillis = 0;
void ApolloHandler::update(void) {
    if(apollo.getState() != "WIFI_NOT_CONNECTED") {
        // If WiFi is connected
        // Running duplex loop
        Serial.print(".");
        client.loop();
    }
}


void ApolloHandler::DuplexHandler::ping() {
    if(apollo.getState() != "WIFI_NOT_CONNECTED") {
        char* payload= "";
        unsigned long packetID = millis();
        // Saving callback to eventsTable
        eventsTable.insert("pong", packetID, [](Payload* payload) {});
        // Formatting the packet
        sprintf(payload, "{\"header\": {\"id\": %lu, \"task\": \"ping\"}}", packetID);
        // Sending to server
        client.sendTXT(payload);
    }
}


int ApolloHandler::PayloadHandler::getValue(uint8_t* payload, char* key, char* value) {
    if(*key == '\0') {
        // If key is empty
        value[0] = '\0';
        return 0;
    }
    // Converting uint8_t to char*
    char* data = (char*) payload;
    
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

int ApolloHandler::PayloadHandler::getValues(uint8_t* payload, char** keys, uint_fast16_t numberOfKeys, char** values, uint_fast16_t maxSizeOfValue) {
    for(int i = 0; i < numberOfKeys; i++) {
        char* value = new char[maxSizeOfValue];
        if(getValue(payload, keys[i], value) == 0) {
            values[i] = new char[maxSizeOfValue];
            memcpy(values[i], value, sizeof(char) * maxSizeOfValue);
        }
        else {
            return -1;
        }
    }
    return 0;
}

int ApolloHandler::PayloadHandler::getPayloadKeys(uint8_t* packet, char** keys, size_t numberOfKeys, size_t sizeOfKey) {
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

int ApolloHandler::PayloadHandler::getPayload(uint8_t* packet, Payload* payload, uint_fast16_t maxSizeOfValue = 10) {
    char** values = payload->values;
    if(getValues(packet, payload->keys, payload->numberOfKeys, payload->values, maxSizeOfValue) == 0) {
        return 0;
    }
    else {
        return -1;
    }
}


int ApolloHandler::DeviceHandler::getSummary(Payload* feedOut, Callback callback) {
    if(apollo.getState() == "DUPLEX_CONNECTED") {
        char* packet= "";
        unsigned long packetID = millis();
        // Saving callback to eventsTable
        eventsTable.insert("getDeviceSummary", packetID, callback);
        // Formatting the packet
        sprintf(packet, "{\"header\": {\"id\": %lu, \"task\": \"getDeviceSummary\"}, \"payload\": {\"deviceID\": \"%s\"}}", packetID, config->deviceID);
        // Sending to server
        client.sendTXT(packet);
        return 0;
    }
}

int ApolloHandler::DeviceHandler::getParms(Payload* feedOut, Callback callback) {
    if(apollo.getState() == "DUPLEX_CONNECTED") {
        if(payload->numberOfKeys > 1 || payload->keys[0] != "deviceID") {
            return -1;
        }
        char* packet= "";
        unsigned long packetID = millis();
        // Saving callback to eventsTable
        eventsTable.insert("getDeviceParms", packetID, callback);
        // Formatting the packet
        sprintf(packet, "{\"header\": {\"id\": %lu, \"task\": \"getDeviceParms\"}, \"payload\": {\"%s\": \"%s\"}}", packetID, payload->keys[0], payload->values[0]);
        // Sending to server
        client.sendTXT(packet);
        return 0;
    }
}

int ApolloHandler::DeviceHandler::setSummary(Payload* summary, Payload* feedOut, Callback callback) {
    if(apollo.getState() == "DUPLEX_CONNECTED") {
        if(payload->numberOfKeys > 1 || payload->keys[0] != "deviceID") {
            return -1;
        }
        char* packet= "";
        unsigned long packetID = millis();
        // Saving callback to eventsTable
        eventsTable.insert("setDeviceSummary", packetID, callback);
        // Formatting the packet
        sprintf(packet, "{\"header\": {\"id\": %lu, \"task\": \"setDeviceSummary\"}, \"payload\": {\"%s\": \"%s\", \"summary\": {\"%s\": \"%s\", \"%s\": \"%s\"}}}", packetID, payload->keys[0], payload->values[0], payload->keys[1], payload->values[1], payload->keys[2], payload->values[2]);
        // Sending to server
        client.sendTXT(packet);
        return 0;
    }
}

int ApolloHandler::DeviceHandler::setParms(Payload* parms, Payload* feedOut, Callback callback) {
    if(apollo.getState() == "DUPLEX_CONNECTED") {
        if(payload->numberOfKeys > 1 || payload->keys[0] != "deviceID") {
            return -1;
        }
        char* packet= "";
        unsigned long packetID = millis();
        // Saving callback to eventsTable
        eventsTable.insert("setDeviceParms", packetID, callback);
        // Formatting the packet
        sprintf(packet, "{\"header\": {\"id\": %lu, \"task\": \"setDeviceParms\"}, \"payload\": {\"%s\": \"%s\", \"parms\": {\"%s\": \"%s\"}}}", packetID, payload->keys[0], payload->values[0], payload->keys[1], payload->values[1]);
        // Sending to server
        client.sendTXT(packet);
        return 0;
    }
}