#include "DuplexClient.h"

// Libraries
#include <ESP8266WiFi.h>

// Duplex Object
DuplexClient duplex;

// Settings
String token = "eyJ0b2tlbiI6ImV5SmhiR2NpT2lKSVV6STFOaUlzSW5SNWNDSTZJa3BYVkNKOS5leUpwWkNJNkltTnJOek5wY1RoNU5UQXdNRFE0T0hScloyaHFlak55WXpraUxDSjBlWEJsSWpvaVpHVjJhV05sSWl3aWFXRjBJam94TlRneU56TXpOakUwZlEueFBrblp6bFhzQkhDMjY1aGp5LWFoQ2c2UlZkVU9nTE5VNVZiNWFHdGNjQSJ9";

void setup() {
    Serial.begin(9600);
    Serial.println("Connecting to WiFi");

    // Connect WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin("planx", "PlanX@12345");

    // Wait for it to happen
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);

        // Show Visual Output
        Serial.print(".");
        digitalWrite(2, !digitalRead(2));
    }

    if (WiFi.status() == WL_CONNECTED) {
        // If connection was Successfull
        // Then Print the IP  
        Serial.println();
        Serial.println("\nWiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.println();

        // Connect to Server
        duplex.begin("10.50.202.122", 3000, "/?type=device&apiKey=ck412ssij0007xr239uos8jfk&token=" + token, "node");

        // try ever 5000 again if connection has failed
        //duplex.setReconnectInterval(5000); 
    }
}

void loop() {
    duplex.loop();
}