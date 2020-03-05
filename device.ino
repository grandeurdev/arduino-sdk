#include "Apollo.h"
#include "Ticker.h"

char* deviceID = "ck73ngond000338tkbzhlfx4r";

void getSummary() {
    char* keys[] = {"deviceID"};
    char* values[] = {deviceID};

    Payload* payload = new Payload(1, keys, values);
    // Requesting for summary of the device
    apollo.device.getSummary(payload, [](Payload* payload) {
        for(int i = 0; i < payload->numberOfKeys; i++) {
            Serial.printf("%s: %s\n", payload->keys[i], payload->values[i]);
        }
    });
    delete payload;
}

void getParms() {
    char* keys[] = {"deviceID"};
    char* values[] = {deviceID};

    Payload* payload = new Payload(1, keys, values);
    // Requesting for parms of the device
    apollo.device.getParms(payload, [](Payload* payload) {
        for(int i = 0; i < payload->numberOfKeys; i++) {
            Serial.printf("%s: %s\n", payload->keys[i], payload->values[i]);
        }
    });
}

void setSummary() {
    // Setting summary of the device
    char* keys[] = {"deviceID","voltage", "current"};
    char* values[] = {deviceID, "100", "15"};

    Payload* payload = new Payload(3, keys, values);
    // Requesting for parms of the device
    apollo.device.setSummary(payload, [](Payload* payload) {
        for(int i = 0; i < payload->numberOfKeys; i++) {
            Serial.printf("%s: %s\n", payload->keys[i], payload->values[i]);
        }
    });
    delete payload;
}

void setup() {
    // Begining serial communication
    Serial.begin(9600);
    // Begining apollo
    apollo.init(Config {"ck412ssij0007xr239uos8jfk", "eyJ0b2tlbiI6ImV5SmhiR2NpT2lKSVV6STFOaUlzSW5SNWNDSTZJa3BYVkNKOS5leUpwWkNJNkltTnJOMlJ1ZURJMVp6QXdNRFUwT0hSclptRm5jVGx6Y0RFaUxDSjBlWEJsSWpvaVpHVjJhV05sSWl3aWFXRjBJam94TlRnek16UTJPVGt5ZlEucEk0czBoWHhLaGVOcGl0cGxQQno5ZHpOaUZ1XzBsYXJ1YmptdGxQQzB2WSJ9", "planx", "PlanX@12345"});
    ticker.attach(5, setSummary);
}

void loop() {
    if(apollo.getState() == "WIFI_NOT_CONNECTED") {
        // If WiFI is not connected
        Serial.println("WIFI IS NOT CONNECTED");
        // Wait for 1 second
        delay(1000);
    }
    else {
        // If WiFi is connected, listen for messages
        apollo.duplex.update();
    }
}