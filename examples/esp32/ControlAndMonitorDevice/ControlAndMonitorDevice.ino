/**
 * @file ControlAndMonitorDevice.ino
 * @date 21.02.2021
 * @author Grandeur Technologies
 *
 * Copyright (c) 2021 Grandeur Technologies Inc. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 * Grandeur.h is used for device's communication with Grandeur.
 * WiFi.h is used for handling device's WiFi.
 *
 * This example illustrates pretty much every basic thing you'd need in order to monitor /
 * control your device through Grandeur. Here are some of those:
 * 1. Listen to Grandeur for updates in device variables.
 * 2. Publish updates in variables to Grandeur every 5 seconds.
 * 3. Running the SDK only when a certain condition is true; in our case, if the WiFi is connected.
 * 
 * After uploading this sketch to your ESP, go to your device's canvas and add a button and
 * a display to control state and monitor voltage variable, respectively.
*/

#include <Grandeur.h>
#include <WiFi.h>

// Device's connection configurations:
String apiKey = "YOUR-PROJECT-APIKEY"; // Copy from https://console.grandeur.dev/access
String deviceID = "YOUR-DEVICE-ID"; // Copy your device ID from https://console.grandeur.dev
String token = "YOUR-DEVICE-TOKEN"; // Copy when you register a new device
const char *ssid = "YOUR-WIFI-SSID";
const char *passphrase = "YOUR-WIFI-PASSWORD";

// Handles our 5 second timer in loop().
unsigned long currentTime = millis();
// Object of Grandeur project.
Grandeur::Project project;
// Device data object to get/set/subscribe to device variables.
Grandeur::Project::Device::Data data;
// State and voltage pins to set.
int statePin = 4;
int voltagePin = 2;

// FUNCTION PROTOTYPES:
// Handles WiFi connection/disconnection events.
void WiFiEventCallback(WiFiEvent_t event);
// Starts the device WiFi.
void startWiFi(void);
// Handles Grandeur connection/disconnection events.
void GrandeurConnectionCallback(bool state);
// Data get/set/update callback functions:
void initializeStatePin(const char *code, bool state);
void setStatePinToNewValue(const char *path, bool state);
void afterVoltageIsUpdated(const char *code, int voltage);

void setup()
{
  Serial.begin(9600);
  startWiFi();
  // This initializes the SDK's configurations and returns reference to your project.
  project = grandeur.init(apiKey, token);
  // Getting object of your device data.
  data = project.device(deviceID).data();
  // This schedules the GrandeurConnectionCallback() function to be called when connection with Grandeur
  // is made/broken.
  project.onConnection(GrandeurConnectionCallback);
  // This schedules setStatePinToNewValue() function to be called when a change in device state occurs
  // on Grandeur.
  data.on("state", setStatePinToNewValue);
  Serial.println("Listening for state update from Grandeur...");
}

void loop()
{
  // In this loop() function, after every five seconds, we send the updated values of our
  // device's voltage to Grandeur.
  if (project.isConnected())
  {
    if (millis() - currentTime >= 5000)
    {
      // This if-condition makes sure that the code inside this block runs only after
      // every five seconds.

      Serial.println("Setting Voltage");
      int voltage = analogRead(voltagePin);
      // This updates the voltage of our device on Grandeur and schedules afterVoltageIsUpdated()
      // function to be called when Grandeur responds with the DATA UPDATED message.
      data.set("voltage", voltage, afterVoltageIsUpdated);

      // This updates the current time for the five seconds timer.
      currentTime = millis();
    }
  }

  // This runs the SDK only when the WiFi is connected.
  if (WiFi.status() == WL_CONNECTED)
    project.loop();
}

void WiFiEventCallback(WiFiEvent_t event)
{
  switch (event)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
    // This runs when the device connects with WiFi.
    Serial.printf("\nDevice has successfully connected to WiFi. Its IP Address is: %s\n",
                  WiFi.localIP().toString().c_str());
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    // This runs when the device disconnects with WiFi.
    Serial.println("Device is disconnected from WiFi.");
    break;
  default:
    break;
  }
}

void startWiFi(void)
{
  // Disconnecting WiFi if it"s already connected
  WiFi.disconnect();
  // Setting it to Station mode which basically scans for nearby WiFi routers
  WiFi.mode(WIFI_STA);
  // Setting WiFi event handler
  WiFi.onEvent(WiFiEventCallback);
  // Begin connecting to WiFi
  WiFi.begin(ssid, passphrase);
  Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid, passphrase);
}

void GrandeurConnectionCallback(bool status)
{
  switch (status)
  {
  case CONNECTED: // Expands to true.
    Serial.println("Device is connected with Grandeur.");
    // On successful connection with Grandeur, we initialize the device's *state*.
    // To do that, we get device state from Grandeur and set the *state pin* to its
    // value.
    data.get("state", initializeStatePin);

    // Initializing the millis counter for the five
    // seconds timer.
    currentTime = millis();
    break;
  case DISCONNECTED: // Expands to false.
    Serial.println("Device's connection with Grandeur is broken.");
    break;
  }
}

void initializeStatePin(const char *code, bool state)
{
  // This function sets the *state pin* to the *state value* that we received in data
  // from Grandeur.
  if (strcmp(code, "DEVICE-DATA-FETCHED") == 0)
  {
    Serial.printf("State is: %d\n", state);
    digitalWrite(statePin, state);
    return;
  }
  // If the data could not be fetched.
  Serial.println("Failed to Fetch State");
  return;
}

void setStatePinToNewValue(const char *path, bool state)
{
  // This function sets the *state pin* to state value.
  Serial.printf("Updated State is: %d\n", state);
  digitalWrite(statePin, state);
}

void afterVoltageIsUpdated(const char *code, int voltage)
{
  if (strcmp(code, "DEVICE-DATA-UPDATED") == 0)
  {
    Serial.printf("Voltage is updated to: %d\n", voltage);

    /* You can set some pins or trigger events here which depend on successful
    ** voltage update.
    */
    return;
  }
  // If the voltage could not be updated.
  Serial.println("Failed to Update Voltage");
  return;
}