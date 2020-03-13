#include "Apollo.h"

char* deviceID = "ck73ngond000338tkbzhlfx4r";
char* apiKey = "ck412ssij0007xr239uos8jfk";
char* token = "eyJ0b2tlbiI6ImV5SmhiR2NpT2lKSVV6STFOaUlzSW5SNWNDSTZJa3BYVkNKOS5leUpwWkNJNkltTnJOMnRqT0dOaE1EQXdNV0V3TVhwa05EVmhhemxvTmpJaUxDSjBlWEJsSWpvaVpHVjJhV05sSWl3aWFXRjBJam94TlRnek56VXdOakkyZlEuOC1BM3VjYWVubmxuaktleGFRc21EbFBQVGpvemhIa0ZEenZ2elhoMWQtQSJ9";
char* ssid = "planx";
char* passphrase = "PlanX@12345";

ApolloDevice* device;

void getSummary() {
    // Requesting for summary of the device
    char payload[128];
    sprintf(payload, "{\"deviceID\": \"%s\"}", deviceID);
    device->getSummary(payload, [](unsigned char* payload) {
        Serial.printf("Response: %s\n", payload);
    });
}

void getParms() {
    // Requesting for parms of the device
    char payload[128];
    sprintf(payload, "{\"deviceID\": \"%s\"}", deviceID);
    device->getParms(payload, [](unsigned char* payload) {
        Serial.printf("Response: %s\n", payload);
    });
}

void setSummary() {
    // Setting summary of the device
    char payload[128];
    sprintf(payload, "{\"deviceID\": \"%s\", \"summary\": {\"voltage\": 10, \"current\": 20}}", deviceID);
    device->setSummary(payload, [](unsigned char* payload) {
        Serial.printf("Response: %s\n", payload);
    });
}

void setParms() {
    // Setting parms of the device
    char payload[128];
    sprintf(payload, "{\"deviceID\": \"%s\", \"parms\": {\"state\": 50}}", deviceID);
    device->setParms(payload, [](unsigned char* payload) {
        Serial.printf("Response: %s\n", payload);
    });
}

unsigned long current;
int startSending = false;

void setup() {
    // Begining serial communication
    Serial.begin(9600);
    // Begining apollo
    device = apollo.init(deviceID, apiKey, token, ssid, passphrase);

    device->onApolloConnected([](unsigned char* message) {
        current = millis();
    });
}

void loop() {
    if(device->getState() == APOLLO_CONNECTED) {
        if(millis() - current >= 5000) {
            getSummary();
            getParms();
            setSummary();
            setParms();

            current = millis();
        }
    }
    // If WiFi is connected, listen for messages
    device->update();
}