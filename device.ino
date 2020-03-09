#include "Apollo.h"
#include "Ticker.h"

char* deviceID = "ck73ngond000338tkbzhlfx4r";
char* apiKey = "ck412ssij0007xr239uos8jfk";
char* token = "eyJ0b2tlbiI6ImV5SmhiR2NpT2lKSVV6STFOaUlzSW5SNWNDSTZJa3BYVkNKOS5leUpwWkNJNkltTnJOMnRqT0dOaE1EQXdNV0V3TVhwa05EVmhhemxvTmpJaUxDSjBlWEJsSWpvaVpHVjJhV05sSWl3aWFXRjBJam94TlRnek56VXdOakkyZlEuOC1BM3VjYWVubmxuaktleGFRc21EbFBQVGpvemhIa0ZEenZ2elhoMWQtQSJ9";
char* ssid = "planx";
char* passphrase = "PlanX@12345";

void getSummary() {
    // Requesting for summary of the device
    char* keys[] = {"voltage"};
    char* values[1];
    Payload* feedOut = new Payload(1, keys, values);
    apollo.device.getSummary(feedOut, [](Payload* payload) {
        for(int i = 0; i < payload->numberOfKeys; i++) {
            Serial.printf("%s: %s\n", payload->keys[i], payload->values[i]);
        }
    });
    delete feedOut;
}

void getParms() {
    // Requesting for parms of the device
    char* keys[] = {"voltage"};
    char* values[1];
    Payload* feedOut = new Payload(1, keys, values);
    apollo.device.getParms(feedOut, [](Payload* payload) {
        for(int i = 0; i < payload->numberOfKeys; i++) {
            Serial.printf("%s: %s\n", payload->keys[i], payload->values[i]);
        }
    });
    delete feedOut;
}

void setSummary() {
    // Setting summary of the device
    char* keys[] = {"deviceID","voltage", "current"};
    char* values[] = {deviceID, "100", "15"};
    Payload* summary = new Payload(3, keys, values);
    
    char* keys[] = {"code"};
    char* values[1];
    Payload* feedOut = new Payload(1, keys, values);
    // Requesting for parms of the device
    apollo.device.setSummary(summary, feedOut, [](Payload* payload) {
        for(int i = 0; i < payload->numberOfKeys; i++) {
            Serial.printf("%s: %s\n", payload->keys[i], payload->values[i]);
        }
    });
    delete summary;
}

void setup() {
    // Begining serial communication
    Serial.begin(9600);
    // Begining apollo
    apollo.init(deviceID, apiKey, token, ssid, passphrase);
    //ticker.attach(5, setSummary);
}

void loop() {
    if(apollo.getState() == "WIFI_NOT_CONNECTED") {
        // If WiFI is not connected
        Serial.println("WIFI IS NOT CONNECTED");
    }
    // If WiFi is connected, listen for messages
    apollo.update();
}