/**
 * @file DuplexHandler.cpp
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "DuplexHandler.h"
#include "arduinoWebSockets/WebSockets.h"

/* VARIABLE INITIALIZATIONS */
WebSocketsClient client;
unsigned long millisCounterForPing = 0;
const char* subscriptionTopics[] = {"deviceSummary", "deviceParms"};
size_t sendQueueSize = 0;
SendData* sendQueue[SENDQUEUE_SIZE] = {};
short DuplexHandler::_status = DISCONNECTED;
void (*DuplexHandler::_connectionCallback)(bool) = [](bool connectionEventHandler) {};

EventTable DuplexHandler::_eventsTable;
Callback DuplexHandler::_subscriptions[4] = {};

/* EVENT HANDLER FUNCTIONS */
void duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length);

DuplexHandler::DuplexHandler(Config config) {
  _query = "/?type=device&apiKey=" + config.apiKey;
  _token = config.token;
}

DuplexHandler::DuplexHandler() {
  _query = "/?type=device";
  _token = "";
}

void DuplexHandler::init(void) {
  // Setting up event handler
  client.onEvent(&duplexEventHandler);
  // Scheduling reconnect every 5 seconds if it disconnects
  client.setReconnectInterval(5000);
  // Opening up the connection
  client.beginSSL(GRANDEUR_URL, GRANDEUR_PORT, _query, GRANDEUR_FINGERPRINT, "node");
  char tokenArray[_token.length() + 1];
  _token.toCharArray(tokenArray, _token.length() + 1);
  client.setAuthorization(tokenArray);
}

void DuplexHandler::onConnectionEvent(void connectionEventHandler(bool)) {
  _connectionCallback = connectionEventHandler;
}

short DuplexHandler::getStatus() {
  return _status;
}

void DuplexHandler::loop(bool valve) {
  if(valve) {
    // If valve is true => valve is open
    if(millis() - millisCounterForPing >= PING_INTERVAL) {
        // Ping Grandeur if PING_INTERVAL milliseconds have passed
        millisCounterForPing = millis();
        ping();
    }
    // Running duplex loop
    client.loop();
  }
}

void DuplexHandler::send(const char* task, const char* payload, Callback callback) {
  if(_status != CONNECTED) {
    sendQueue[sendQueueSize++] = new SendData(task, payload, callback);
    return ;
  }
  char packet[PACKET_SIZE];
  GrandeurID packetID = millis();
  // Saving callback to eventsTable
  _eventsTable.insert(task, packetID, callback);
  // Formatting the packet
  snprintf(packet, PACKET_SIZE, "{\"header\": {\"id\": %lu, \"task\": \"%s\"}, \"payload\": %s}", packetID, task, payload);
  // Sending to server
  client.sendTXT(packet);
}

void DuplexHandler::subscribe(short event, const char* payload, Callback updateHandler) {
  // Saving updateHandler callback to subscriptions Array
  _subscriptions[event] = updateHandler;
  send("/topic/subscribe", payload, [](JSONObject payload) {});
}

/** This function pings the cloud to keep the connection alive.
*/
void DuplexHandler::ping() {
  if(_status == CONNECTED) {
    char packet[PING_PACKET_SIZE];
    GrandeurID packetID = millis();
    // Saving callback to eventsTable
    _eventsTable.insert("ping", packetID, [](JSONObject feed) {});
    // Formatting the packet
    snprintf(packet, PING_PACKET_SIZE, "{\"header\": {\"id\": %lu, \"task\": \"ping\"}}", packetID);
    // Sending to server
    client.sendTXT(packet);
  }
}

/** This function handles all the cloud events.
 * @param eventType: The type of event that has occurred.
 * @param packet: The packet corresponding to the event.
 * @param length: The size of the @param packet.
*/
void duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length) {
  JSONObject updateObject;
  switch(eventType) {
    case WStype_CONNECTED:
      // When duplex connection opens
      DuplexHandler::_status = CONNECTED;
      DuplexHandler::_connectionCallback(DuplexHandler::_status);
      // Resetting ping millis counter
      millisCounterForPing = millis();
      // Sending all queued messages
      for(int i = 0; i < sendQueueSize; i++) {
        DuplexHandler::send(sendQueue[i]->task, sendQueue[i]->payload, sendQueue[i]->callback);
      }
      return ;

    case WStype_DISCONNECTED:
      // When duplex connection closes
      DuplexHandler::_status = DISCONNECTED;
      return DuplexHandler::_connectionCallback(DuplexHandler::_status);

    case WStype_TEXT:
      // When a duplex message is received.
      JSONObject messageObject = JSON.parse((char*) packet);
      if (JSON.typeof(messageObject) == "undefined") {
        // Just for internal errors of Arduino_JSON
        // if the parsing fails.
        DEBUG_GRANDEUR("Parsing input failed!");
        return;
      }
      if(messageObject["header"]["task"] == "update") {
        for(int i = 0; i < NUMBER_OF_TOPICS; i++) {
          if(messageObject["payload"]["event"] == subscriptionTopics[i]) {
            DuplexHandler::_subscriptions[i](messageObject["payload"]["update"]);
            return ;
          }
        }
      }
      else if(messageObject["header"]["task"] == "unpair") {
        return ;
      }
      // Fetching event callback function from the events Table
      Callback callback = DuplexHandler::_eventsTable.findAndRemove(
        (const char*) messageObject["header"]["task"],
        (GrandeurID) messageObject["header"]["id"]
      );
      if(!callback) {
        return;
      }
      return callback(messageObject["payload"]);
  }
}

