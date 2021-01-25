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

/* Create client */
WebSocketsClient client;

// Init ping counter
unsigned long millisCounterForPing = 0;

// Define queue size
size_t sendQueueSize = 0;

// Create an array equal to max send queue size
SendData* sendQueue[SENDQUEUE_SIZE] = {};

// Init status handler and variable
short DuplexHandler::_status = DISCONNECTED;
void (*DuplexHandler::_connectionCallback)(bool) = [](bool connectionEventHandler) {};

// Create a new event table and subscriptions
EventTable DuplexHandler::_eventsTable;
EventTable DuplexHandler::_subscriptions;

/* Deifne event handler */
void duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length);

DuplexHandler::DuplexHandler(Config config) {
  // Constructor
  _query = "/?type=device&apiKey=" + config.apiKey;
  _token = config.token;
}

DuplexHandler::DuplexHandler() {
  // Overload the contructor
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

  // Set auth header
  char tokenArray[_token.length() + 1];
  _token.toCharArray(tokenArray, _token.length() + 1);
  client.setAuthorization(tokenArray);
}

void DuplexHandler::onConnectionEvent(void connectionEventHandler(bool)) {
  // Event handler for connection
  _connectionCallback = connectionEventHandler;
}

short DuplexHandler::getStatus() {
  // Return status
  return _status;
}

void DuplexHandler::loop(bool valve) {
  // Give duplex time to execute
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

void DuplexHandler::send(const char* task, const char* payload, Callback<JSONObject> callback) {
  // Check connection status
  if(_status != CONNECTED) {
    // Add to queue if not connected
    sendQueue[sendQueueSize++] = new SendData(task, payload, callback);
    return ;
  }

  // Create packet
  char packet[PACKET_SIZE];

  // Generate packet id
  GrandeurID packetID = millis();

  // Saving callback to eventsTable
  _eventsTable.insert(task, packetID, callback);
  _eventsTable.print();
  _subscriptions.print();

  // Formatting the packet
  snprintf(packet, PACKET_SIZE, "{\"header\": {\"id\": %lu, \"task\": \"%s\"}, \"payload\": %s}", packetID, task, payload);
  
  // Sending to server
  client.sendTXT(packet);
}

void DuplexHandler::subscribe(const char* event, const char* payload, Callback<JSONObject> updateHandler) {
  // Generate an id
  GrandeurID eventID = millis();

  // Saving callback to eventsTable
  _subscriptions.insert(event, eventID, updateHandler);
  _eventsTable.print();
  _subscriptions.print();

  // Saving callback in event table with key
  send("/topic/subscribe", payload, NULL);
}


void DuplexHandler::ping() {
  // Ping handler
  if(_status == CONNECTED) {
    // Generate packet
    char packet[PING_PACKET_SIZE];

    // Create id
    GrandeurID packetID = millis();

    // Saving callback to eventsTable
    _eventsTable.insert("ping", packetID, NULL);

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

  // Switch over event type
  switch(eventType) {
    case WStype_CONNECTED:
      // When duplex connection opens
      DuplexHandler::_status = CONNECTED;

      // Generate callback
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

      // Clear event table
      DuplexHandler::_eventsTable.clear();

      // Generate connection event
      return DuplexHandler::_connectionCallback(DuplexHandler::_status);

    case WStype_TEXT:
      Serial.printf("%s\n", packet);

      // When a duplex message is received.
      JSONObject messageObject = JSON.parse((char*) packet);

      // Handle parsing errors
      if (JSON.typeof(messageObject) == "undefined") {
        // Just for internal errors of Arduino_JSON
        // if the parsing fails.
        DEBUG_GRANDEUR("Parsing input failed!");
        return;
      }

      // If it is an update instead of response to a task
      if(messageObject["header"]["task"] == "update") {
        // Formulate the key
        std::string event((const char*) messageObject["payload"]["event"]);
        std::string path((const char*) messageObject["payload"]["path"]);

        // Emit the event 
        DuplexHandler::_subscriptions.emit(event + "/" + path, messageObject["payload"]);

        // Return
        return;
      }
      // Just return if it is unpair event
      else if(messageObject["header"]["task"] == "unpair") {
        return ;
      }

      // Fetching event callback function from the events Table
      Callback<JSONObject> callback = DuplexHandler::_eventsTable.findAndRemove(
        (const char*) messageObject["header"]["task"],
        (GrandeurID) messageObject["header"]["id"]
      );

      DuplexHandler::_eventsTable.print();
      DuplexHandler::_subscriptions.print();

      // If not found then simply return
      if(!callback) return;

      // Or otherwise resolve the event
      return callback(messageObject["payload"]);
  }
}

