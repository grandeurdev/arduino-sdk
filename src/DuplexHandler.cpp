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
#include <regex>

/* VARIABLE INITIALIZATIONS */
const char* routes[10] =
  {"ping", "/topic/subscribe", "/topic/unsubscribe", "/topics/unsubscribe",
  "/device/data/get", "/device/data/set", "/datastore/insert", "/datastore/delete",
  "/datastore/update", "/datastore/pipeline"};

//WebSocketsClient client;
unsigned long millisCounterForPing = 0;
size_t sendQueueSize = 0;
SendData* sendQueue[SENDQUEUE_SIZE] = {};

void unsubscribeTopic(EventEmitter<Var>* subs, DuplexHandler* duplex, const char* event, String path, const char* payload) {
  subs->off(String(event) + "." + path);
  duplex->send("/topic/unsubscribe", payload, [](Var payload) {});
}

DuplexHandler::DuplexHandler() : _query("/?type=device"), _token(""), _status(DISCONNECTED),
  _connectionCallback([](bool status) {}) {}

void DuplexHandler::onConnectionEvent(void connectionCallback(bool)) {
  _connectionCallback = connectionCallback;
}

void DuplexHandler::clearConnectionCallback(void) {
  _connectionCallback = [](bool status) {};
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
    _client.loop();
  }
}

void DuplexHandler::send(const char* task, const char* payload, Callback callback) {
  if(_status != CONNECTED) {
    sendQueue[sendQueueSize++] = new SendData(task, payload, callback);
    return ;
  }
  
  char packet[PACKET_SIZE];
  gId packetID = gid();
  // Attaching an event listener
  _tasks.once(String(packetID), callback);
  // Formatting the packet
  snprintf(packet, PACKET_SIZE, "{\"header\": {\"id\": \"%ul\", \"task\": \"%s\"}, \"payload\": %s}", packetID, task, payload);  
  // Sending to server
  _client.sendTXT(packet);
}

std::function<void(void)> DuplexHandler::subscribe(const char* event, String path, const char* payload, Callback updateHandler) {
  // Saving updateHandler callback to subscriptions Array
  String topic = String(event) + "/" + path;
  _subscriptions.on(topic, updateHandler);
  send("/topic/subscribe", payload, [](Var payload) {});

  return std::bind(unsubscribeTopic, &_subscriptions, this, event, path, payload);
}

/** This function pings the cloud to keep the connection alive.
*/
void DuplexHandler::ping() {
  if(_status == CONNECTED) {
    char packet[PING_PACKET_SIZE];
    gId packetID = gid();
    // Saving callback to eventsTable
    _tasks.once(String(packetID), [](Var payload) {});
    // Formatting the packet
    snprintf(packet, PING_PACKET_SIZE, "{\"header\": {\"id\": \"%lu\", \"task\": \"ping\"}}", packetID);
    // Sending to server
    _client.sendTXT(packet);
  }
}

/** This function handles all the duplex events.
 * @param eventType: The type of event that has occurred.
 * @param packet: The packet corresponding to the event.
 * @param length: The size of the @param packet.
*/
void DuplexHandler::duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length) {
  switch(eventType) {
    case WStype_CONNECTED:
      // When duplex connection opens
      _status = CONNECTED;
      _connectionCallback(_status);
      // Resetting ping millis counter
      millisCounterForPing = millis();
      // Sending all queued messages
      for(int i = 0; i < sendQueueSize; i++) {
        send(sendQueue[i]->task, sendQueue[i]->payload, sendQueue[i]->callback);
      }
      return ;

    case WStype_DISCONNECTED:
      // When duplex connection closes
      _status = DISCONNECTED;
      _connectionCallback(_status);
      return ;

    case WStype_TEXT:
      // When a duplex message is received.
      Var messageObject = JSON.parse((char*) packet);
      if (JSON.typeof(messageObject) == "undefined") {
        // Just for internal errors of Arduino_JSON
        // if the parsing fails.
        DEBUG_GRANDEUR("Parsing input failed!");
        return;
      }
      String task = String((const char*) messageObject["header"]["task"]);
      if(task == "update") {
        for(int i = 0; i < NUMBER_OF_TOPICS; i++) {
          if(_events[i] == messageObject["payload"]["event"]) {
            String topic = messageObject["payload"]["event"] + "." +
                                  messageObject["payload"]["path"];

            String* events = _subscriptions.getEventIds();
            unsigned int nEvents = _subscriptions.getNListeners();

            if(messageObject["payload"]["event"] == "data") {
              for(int i = 0; i < nEvents; i++) {
                if(std::regex_match(topic.c_str(), std::regex(events[i].c_str())))
                  _subscriptions.emit(events[i], messageObject["payload"]["update"]);
              }
              return;
            }

            for(int i = 0; i < nEvents; i++) {
              if(topic == events[i]) {
                _subscriptions.emit(events[i], messageObject["payload"]["update"]);
              }
            }
            return;
          }
        }
      }
      if(task == "unpair") return;
      _tasks.emit(task, messageObject["payload"]);
      return;
  }
}

void DuplexHandler::init(Config config) {
  _query = "/?type=device&apiKey=" + config.apiKey;
  _token = config.token;
  // Setting up event handler
  _client.onEvent([=](WStype_t eventType, uint8_t* packet, size_t length) {
    duplexEventHandler(eventType, packet, length);
  });
  // Scheduling reconnect every 5 seconds if it disconnects
  _client.setReconnectInterval(5000);
  // Opening up the connection
  _client.beginSSL(GRANDEUR_URL, GRANDEUR_PORT, _query, GRANDEUR_FINGERPRINT, "node");
  char tokenArray[_token.length() + 1];
  _token.toCharArray(tokenArray, _token.length() + 1);
  _client.setAuthorization(tokenArray);
}