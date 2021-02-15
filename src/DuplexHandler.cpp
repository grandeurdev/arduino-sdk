/**
 * @file DuplexHandler.cpp
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */


#include <regex>
#include "DuplexHandler.h"

// Init ping counter
unsigned long millisCounterForPing = 0;

DuplexHandler::DuplexHandler() : _query("/?type=device"), _token(""), _status(DISCONNECTED),
  _connectionHandler([](bool status) {}) {}

void DuplexHandler::init(Config config) {
  _query = _query + "&apiKey=" + config.apiKey;
  _token = config.token;
  // Setting up event handler
  _client.onEvent([=](WStype_t eventType, uint8_t* packet, size_t length) {
    duplexEventHandler(eventType, packet, length);
  });

  // Scheduling reconnect every 5 seconds if it disconnects
  _client.setReconnectInterval(5000);

  DEBUG_GRANDEUR("Initializing duplex channel.");

  // Opening up the connection.
  _client.beginSSL(GRANDEUR_URL, GRANDEUR_PORT, _query, GRANDEUR_FINGERPRINT, "node");

  // Setting auth header.
  char tokenArray[_token.length() + 1];
  _token.toCharArray(tokenArray, _token.length() + 1);
  _client.setAuthorization(tokenArray);
}

void DuplexHandler::onConnectionEvent(void connectionCallback(bool)) {
  DEBUG_GRANDEUR("Setting up connection handler.");
  _connectionHandler = connectionCallback;
}

void DuplexHandler::clearConnectionCallback(void) {
  DEBUG_GRANDEUR("Clearing connection handler.");
  // Setting connection handler to empty function.
  _connectionHandler = [](bool status) {};
}

bool DuplexHandler::getStatus() {
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

void DuplexHandler::send(const char* task, const char* payload, Callback cb) {
  // Generate message id
  gId messageId = gid();

  // Checking connection status
  if(_status != CONNECTED) {
    // Append the packet to queue
    _queue.push(messageId, task, payload, cb);
    return;
  }

  // Creating new message.
  char message[MESSAGE_SIZE];
  // Adding task.
  _tasks.once(String(messageId), cb);

  // Preparing the message.
  snprintf(message, MESSAGE_SIZE,
        "{\"header\": {\"id\": %lu, \"task\": \"%s\"}, \"payload\": %s}", messageId, task, payload);
  
  // Sending.
  _client.sendTXT(message);
}

gId DuplexHandler::subscribe(const char* topic, const char* payload, Callback updateHandler) {
  // Generate message id
  gId messageId = gid();

  // Saving callback to subscriptions Table
  _subscriptions.on(String(messageId), updateHandler);

  // Append the packet to queue because in case of queue
  // the packet will always be queue either we are connected
  // or disconnected
  // This is being done to handle case where we were connected
  // the subscribed to some events and got disconnected
  _queue.push(messageId, "/topic/subscribe", payload, NULL);
  
  // Check connection status
  if(_status != CONNECTED) {
    // Don't proceed if we aren't
    // but return packet id
    return messageId;
  }

  // Creating a new message.
  char message[MESSAGE_SIZE];

  // Preparing the message.
  snprintf(message, MESSAGE_SIZE,
      "{\"header\": {\"id\": %lu, \"task\": \"%s\"}, \"payload\": %s}", messageId,
      "/topic/subscribe", payload);
  
  // Sending.
  _client.sendTXT(message);

  // Return the message Id
  return messageId;
}

void DuplexHandler::unsubscribe(gId id, const char* payload) {
  // Generate message id
  gId messageId = gid();

  // Removing event from subscriptions
  _subscriptions.off(String(id));

  // Pushing unsub to queue.
  _queue.push(messageId, "/topic/unsubscribe", payload, NULL);
  // and remove subscription from queue.
  _queue.remove(id);
  
  // Check connection status
  if(_status != CONNECTED) {
    // Don't proceed if we aren't
    return;
  }

  // Creating a new message.
  char message[MESSAGE_SIZE];

  // Saving callback to eventsTable
  _tasks.once(String(messageId), NULL);

  // Preparing the message.
  snprintf(message, MESSAGE_SIZE,
        "{\"header\": {\"id\": %lu, \"task\": \"%s\"}, \"payload\": %s}",
        messageId, "/topic/unsubscribe", payload);
  
  // Sending.
  _client.sendTXT(message);
}

// Define the handle function
void DuplexHandler::handle(EventID id, EventKey key, EventPayload payload, Callback callback) {
  // Check connection status
  if(_status != CONNECTED) {
    return ;
  }

  // Creating a new message.
  char message[MESSAGE_SIZE];

  // Scheduling task.
  _tasks.once(String(id), callback);

  // Preparing the message.
  snprintf(message, MESSAGE_SIZE,
      "{\"header\": {\"id\": %lu, \"task\": \"%s\"}, \"payload\": %s}",
      id, key.c_str(), payload.c_str());
  
  // Sending.
  _client.sendTXT(message);
}

void DuplexHandler::ping() {
  if(_status == CONNECTED) {
    DEBUG_GRANDEUR("Pinging Grandeur.");

    char message[PING_MESSAGE_SIZE];
    gId messageId = gid();
    // Formatting the packet
    snprintf(message, PING_MESSAGE_SIZE, "{\"header\": {\"id\": \"%lu\", \"task\": \"ping\"}}", messageId);
    // Sending to server
    _client.sendTXT(message);
  }
}

void DuplexHandler::duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length) {
  // Switch over event type
  switch(eventType) {
    case WStype_CONNECTED:
      DEBUG_GRANDEUR("Duplex channel established.");
      // When duplex connection opens
      _status = CONNECTED;
      // Running connection handler.
      _connectionHandler(_status);
      // Resetting ping millis counter
      millisCounterForPing = millis();

      // Handle the queued events
      _queue.forEach([=](EventID id, EventKey key, EventPayload payload, Callback cb) {
        handle(id, key, payload, cb);
      });

      return ;

    case WStype_DISCONNECTED:
      DEBUG_GRANDEUR("Duplex channel broke.");
      // When duplex connection closes
      _status = DISCONNECTED;
      // Clear all tasks.
      _tasks.offAll();
      // Running connection handler.
      return _connectionHandler(_status);

    case WStype_TEXT:
      // When a duplex message is received.
      Var messageObject = JSON.parse((char*) packet);
      // Handling any parsing errors
      if (JSON.typeof(messageObject) == "undefined") {
        // Just for internal errors of Arduino_JSON
        // if the parsing fails.
        DEBUG_GRANDEUR("Parsing message failed!");
        return;
      }
      
      String task = String((const char*) messageObject["header"]["task"]);

      // We do not need to handle the unpair or ping event in Device SDKs, so simply returning.
      if(task == "unpair") return;
      if(task == "ping") return;
      // If it is an update event rather than a task (response message).
      if(task == "update") {
        DEBUG_GRANDEUR(
          "Data update is received:: path: %s, data: %s.",
          (const char*) messageObject["payload"]["path"],
          (const char*) messageObject["payload"]["data"]
        );
        // Formulate the key
        String event = (const char*) messageObject["payload"]["event"];
        String path = (const char*) messageObject["payload"]["path"];
        String topic = messageObject["payload"]["event"] + "/" +
                                  messageObject["payload"]["path"];

        // Handling the backward compatibility for summary/parms
        if (event == "deviceParms" || event == "deviceSummary") event = "data";

        String* events = _subscriptions.getIds();
        unsigned int nEvents = _subscriptions.getNListeners();

        // If it's update for device data, do pattern matching on topic: event/path.
        if(messageObject["payload"]["event"] == "data") {
          for(int i = 0; i < nEvents; i++) {
            if(std::regex_match(topic.c_str(), std::regex(events[i].c_str())))
              _subscriptions.emit(
                events[i],
                messageObject["payload"]["path"],
                messageObject["payload"]["update"]
              );
          }
          return;
        }
        // Otherwise just emit on the event.
        _subscriptions.emit(event, event.c_str(), messageObject["payload"]["update"]);
        return;
      }
      // Otherwise: It's a response message for a task.
      // Extracting code.
      String code = (const char*) messageObject["payload"]["code"];
      //Removing code and message from message payload. We only emit code and data from response.
      messageObject["payload"]["code"] = undefined;
      messageObject["payload"]["message"] = undefined;

      DEBUG_GRANDEUR(
        "Response message is received:: code: %s, data: %s.",
        code.c_str(),
        JSON.stringify(messageObject["payload"]).c_str()
      );

      // Emit on the Id from the tasks.
      _tasks.emit(
        String((const char*) messageObject["header"]["id"]),
        code.c_str(),
        messageObject["payload"]
      );

      // Remove the packet if it was queued
      // because the ack has been received
      if (task == "/topic/subscribe");
      else {
        // but if it is not of subscribe type
        DuplexHandler::_queue.remove((gId) (int) messageObject["header"]["id"]);
      }
      return;
  }
}

