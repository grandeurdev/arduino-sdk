/**
 * @file Apollo.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

#include "Apollo.h"

/* VARIABLE INITIALIZATIONS */
DuplexClient duplexClient;
unsigned long pingSchedularVariable = 0;
const char* subscriptionTopics[] = {"setDeviceSummary", "setDeviceParms"};
size_t sendQueueSize = 0;
SendData* sendQueue[SENDQUEUE_SIZE] = {};
Config* config;
ApolloDevice apolloDevice;
Apollo apollo;

EventTable ApolloDevice::_eventsTable;
Callback ApolloDevice::_connectionCallback = [](JSONObject updateObject) {};
Callback ApolloDevice::_subscriptions[4] = {};
short ApolloDevice::_state = 0;

void initializeDuplex(void);

/* EVENT HANDLER FUNCTIONS */
void apolloEventHandler(WStype_t eventType, uint8_t* packet, size_t length);

ApolloDevice Apollo::init(String deviceID, String apiKey, String token) {
  // Setting Apollo config
  config = new Config(deviceID, apiKey, token);
  // Initializing Duplex connection
  initializeDuplex();

  return apolloDevice;
}

ApolloDevice::ApolloDevice() {}

void ApolloDevice::_send(const char* task, const char* payload, Callback callback) {
  if(_state != CONNECTED) {
    sendQueue[sendQueueSize++] = new SendData(task, payload, callback);
    return ;
  }
  char packet[PACKET_SIZE];
  ApolloID packetID = millis();
  // Saving callback to eventsTable
  _eventsTable.insert(task, packetID, callback);
  // Formatting the packet
  snprintf(packet, PACKET_SIZE, "{\"header\": {\"id\": %lu, \"task\": \"%s\"}, \"payload\": %s}", packetID, task, payload);
  // Sending to server
  duplexClient.sendTXT(packet);
}

void ApolloDevice::_subscribe(short event, const char* payload, Callback updateHandler) {
  // Saving updateHandler callback to subscriptions Array
  _subscriptions[event] = updateHandler;
  _send("subscribeTopic", payload, [](JSONObject payload) {});
}

void ApolloDevice::getSummary(Callback callback) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = config->deviceID;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _send("getDeviceSummary", jsonString, callback);
}

void ApolloDevice::getParms(Callback callback) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = config->deviceID;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _send("getDeviceParms", jsonString, callback);
}

void ApolloDevice::setSummary(JSONObject summary, Callback callback) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = config->deviceID;
  jsonObject["summary"] = summary;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _send("setDeviceSummary", jsonString, callback);
}

void ApolloDevice::setParms(JSONObject parms, Callback callback) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = config->deviceID;
  jsonObject["parms"] = parms;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _send("setDeviceParms", jsonString, callback);
}

Config ApolloDevice::getConfig() {
  Config apolloConfig(config->deviceID, config->apiKey, config->token);
  return apolloConfig;
}

String ApolloDevice::getDeviceID(void) {
  return config->deviceID;
}

String ApolloDevice::getApiKey(void) {
  return config->apiKey;
}

short ApolloDevice::getState(void) {
  return _state;
}

String ApolloDevice::getToken(void) {
  return config->token;
}

void ApolloDevice::onConnection(Callback connectionHandler) {
  _connectionCallback = connectionHandler;
}

void ApolloDevice::onSummaryUpdated(Callback updateHandler) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = config->deviceID;
  jsonObject["event"] = "setDeviceSummary";
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _subscribe(SUMMARYUPDATE, jsonString, updateHandler);
}

void ApolloDevice::onParmsUpdated(Callback updateHandler) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = config->deviceID;
  jsonObject["event"] = "setDeviceParms";
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _subscribe(PARMSUPDATE, jsonString, updateHandler);
}

void initializeDuplex(void) {
  // Setting up event handler
  duplexClient.onEvent(&apolloEventHandler);
  // Scheduling reconnect every 5 seconds if it disconnects
  duplexClient.setReconnectInterval(5000);

  // Setting up device cloud connection event handler
  apolloDevice.onConnection([](JSONObject updateObject) {
    switch((int) updateObject["event"]) {
      case CONNECTED:
        DEBUG_APOLLO("\n*** DEVICE CONNECTED WITH CLOUD ***\n");
        break;
      case DISCONNECTED:
        DEBUG_APOLLO("\n*** DEVICE DISCONNECTED FROM CLOUD ***\n");
        break;
    }
  });

  // Opening up the connection
  duplexClient.beginSSL(APOLLO_URL, APOLLO_PORT, "/?type=device&apiKey=" + config->apiKey, APOLLO_FINGERPRINT, "node");
  char token[config->token.length() + 1];
  config->token.toCharArray(token, config->token.length() + 1);
  duplexClient.setAuthorization(token);
}

void ApolloDevice::loop(bool valve) {
  if(valve) {
    // If valve is true => valve is open
    if(millis() - pingSchedularVariable >= PING_INTERVAL) {
        // Ping Apollo if PING_INTERVAL milliseconds have passed
        pingSchedularVariable += PING_INTERVAL;
        ping();
    }
    // Running duplex loop
    duplexClient.loop();
  }
}

/** This function pings the cloud to keep the connection alive.
*/
void ApolloDevice::ping() {
  if(_state == CONNECTED) {
    char packet[PING_PACKET_SIZE];
    ApolloID packetID = millis();
    // Saving callback to eventsTable
    _eventsTable.insert("ping", packetID, [](JSONObject feed) {});
    // Formatting the packet
    snprintf(packet, PING_PACKET_SIZE, "{\"header\": {\"id\": %lu, \"task\": \"ping\"}}", packetID);
    // Sending to server
    duplexClient.sendTXT(packet);
  }
}

/** This function handles all the cloud events.
 * @param eventType: The type of event that has occurred.
 * @param packet: The packet corresponding to the event.
 * @param length: The size of the @param packet.
*/
void apolloEventHandler(WStype_t eventType, uint8_t* packet, size_t length) {
  JSONObject updateObject;
  switch(eventType) {
    case WStype_CONNECTED:
      // When duplex connection opens
      apolloDevice._state = CONNECTED;
      updateObject["event"] = CONNECTED;
      apolloDevice._connectionCallback(updateObject);
      // Sending all queued messages
      for(int i = 0; i < sendQueueSize; i++) {
        apolloDevice._send(sendQueue[i]->task, sendQueue[i]->payload, sendQueue[i]->callback);
      }
      return ;

    case WStype_DISCONNECTED:
      // When duplex connection closes
      apolloDevice._state = DISCONNECTED;
      updateObject["event"] = DISCONNECTED;
      return apolloDevice._connectionCallback(updateObject);

    case WStype_TEXT:
      // When a duplex message is received.
      // Fetching task and id from the message packet
      JSONObject messageObject = JSON.parse((char*) packet);
      if (JSON.typeof(messageObject) == "undefined") {
        // Just for internal errors of Arduino_JSON
        // if the parsing fails.
        DEBUG_APOLLO("Parsing input failed!");
        return;
      }
      if(messageObject["header"]["task"] == "update") {
        for(int i = 0; i < NUMBER_OF_TOPICS; i++) {
          if(messageObject["payload"]["event"] == subscriptionTopics[i]) {
            apolloDevice._subscriptions[i](messageObject["payload"]["update"]);
            return ;
          }
        }
      }
      // Fetching event callback function from the events Table
      Callback callback = apolloDevice._eventsTable.findAndRemove(
        (const char*) messageObject["header"]["task"],
        (ApolloID) messageObject["header"]["id"]
      );
      if(!callback) {
        return;
      }
      return callback(messageObject["payload"]);
  }
}