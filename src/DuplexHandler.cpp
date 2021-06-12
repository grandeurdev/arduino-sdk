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
unsigned long timeSinceLastMessage = 0;

DuplexHandler::DuplexHandler() : _query("/?type=device"), _token(""), _status(DISCONNECTED),
  _connectionHandler([](bool status) {}) {}

void DuplexHandler::init(Config config) {
  _query = _query + "&apiKey=" + config.apiKey;
  _token = config.token;
  // Setting up event handler
  _client.onEvent([=](WStype_t eventType, uint8_t* message, size_t length) {
    duplexEventHandler(eventType, message, length);
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

void DuplexHandler::loop(bool valve) {
  if(valve) {
    // If valve is true => valve is open
    if(millis() - timeSinceLastMessage >= PING_INTERVAL) {
        // Ping Grandeur if PING_INTERVAL milliseconds have passed.
        timeSinceLastMessage = millis();

        DEBUG_GRANDEUR("Pinging Grandeur.");
        send("ping");
    }
    // Running duplex loop
    _client.loop();
  }
}

Message DuplexHandler::prepareMessage(const char* task) {
  // Generate a new message id.
  gId messageId = gid();
  // Creating new message.
  Var oMessage;
  oMessage["header"]["id"] = messageId;
  oMessage["header"]["task"] = task;

  return {messageId, JSON.stringify(oMessage)};
}

Message DuplexHandler::prepareMessage(const char* task, Var payload) {
  // Generate a new message id.
  gId messageId = gid();
  // Creating new message.
  Var oMessage;
  oMessage["header"]["id"] = messageId;
  oMessage["header"]["task"] = task;
  oMessage["payload"] = payload;

  // Preparing message string.
  String message = JSON.stringify(oMessage);
  DEBUG_GRANDEUR("Prepared message:: message: %s.", message.c_str());

  return {messageId, message};
}

void DuplexHandler::sendMessage(const char* message) {
  // Returning if channel isn't alive.
  if(_status != CONNECTED) return;

  // Resetting timeSinceLastMessage.
  //timeSinceLastMessage = millis();

  DEBUG_GRANDEUR("Sending message:: %s.", message);
  // Sending on channel.
  _client.sendTXT(message);
}

Message DuplexHandler::send(const char* task, Callback cb) {
  // Preparing a new message.
  Message message = prepareMessage(task);

  // Adding task to receive the response message.
  _tasks.once(message.id, cb);

  // If channel isn't connected yet, buffer the message and return.
  if(_status != CONNECTED) {
    _buffer.push(message.id, message.str);
    return {message.id, message.str};
  }

  // Sending message.
  sendMessage(message.str.c_str());

  return {message.id, message.str};
}

Message DuplexHandler::send(const char* task) {
  // Preparing a new message.
  Message message = prepareMessage(task);

  // If channel isn't connected yet, buffer the message and return.
  if(_status != CONNECTED) {
    _buffer.push(message.id, message.str);
    return {message.id, message.str};
  }

  // Sending message.
  sendMessage(message.str.c_str());

  return {message.id, message.str};
}

Message DuplexHandler::send(const char* task, Var payload, Callback cb) {
  // Preparing a new message.
  Message message = prepareMessage(task, payload);

  // Adding task to receive the response message.
  _tasks.once(message.id, cb);
  
  // If channel isn't connected yet, buffer the message and return.
  if(_status != CONNECTED) {
    _buffer.push(message.id, message.str);
    return {message.id, message.str};
  }

  // Sending message.
  sendMessage(message.str.c_str());

  return {message.id, message.str};
}

Message DuplexHandler::send(const char* task, Var payload) {
  // Preparing a new message.
  Message message = prepareMessage(task, payload);
  
  // If channel isn't connected yet, buffer the message and return.
  if(_status != CONNECTED) {
    _buffer.push(message.id, message.str);
    return {message.id, message.str};
  }

  // Sending message.
  sendMessage(message.str.c_str());

  return {message.id, message.str};
}

void DuplexHandler::receive(Var header, Var payload) {
  // Extracting task and id from header and code.
  const char* task = header["task"];
  gId id = header["id"];
  const char* code = payload["code"];
  
  // Extracting data.
  Var data = null;
  // Response to Get has data in payload["data"].
  if(strcmp(task, "/device/data/get") == 0) data = payload["data"];
  // Response to Set has data in payload["update"].
  else if(strcmp(task, "/device/data/set") == 0) data = payload["update"];
  // For datastore, we delete code and message from the payload and send the rest.
  else if(strcmp(task, "/datastore/insert") == 0 || strcmp(task, "/datastore/delete") == 0 ||
    strcmp(task, "/datastore/update") == 0 || strcmp(task, "/datastore/pipeline") == 0) {
    data = payload;
    data["code"] = undefined;
    data["message"] = undefined;
  }

  DEBUG_GRANDEUR( "Response message:: code: %s, data: %s.", code, JSON.stringify(data).c_str() );

  // Emit on the Id from the tasks.
  if(Var::typeof_(data) != "null" && Var::typeof_(data) != "undefined")
    _tasks.emit( id, code, data );
  else
    _tasks.emit( id, code, undefined );
}

void DuplexHandler::publish(const char* event, const char* path, Var data) {
  DEBUG_GRANDEUR( "Data update:: path: %s, data: %s.", path, JSON.stringify(data) );

  // Handling the backward compatibility for summary/parms.
  if (strcmp(event, "deviceParms") == 0 || strcmp(event, "deviceSummary") == 0)
    strcpy((char*) event, "data");

  // If it's update for device data, emit on the pattern "event/path". So that the listeners
  // subscribing to "event/"" get the update for "event/path" as well.
  if(strcmp(event, "data") == 0) {
    _subscriptions.pEmit( String(event) + "/" + String(path), path, data );
    return;
  }

  // Otherwise just emit on the event.
  _subscriptions.emit(String(event), "", data);
  return;
}

gId DuplexHandler::subscribe(const char* topic, Var payload, Callback updateHandler) {
  DEBUG_GRANDEUR("Subscribing to topic:: %s.", topic);

  // Sending subscription request.
  Message message = send("/topic/subscribe", payload, NULL);
  // Setting update handler.
  _subscriptions.on(String(topic), updateHandler);
  // Buffer subscription request message regardless of connection/disconnection to handle the case
  // of subscribing, disconnecting, and reconnecting without record of previous subscriptions.
  _buffer.push(message.id, message.str);

  // Return the message Id.
  return message.id;
}

void DuplexHandler::unsubscribe(const char* topic, gId eventId, Var payload) {
  DEBUG_GRANDEUR("Unsubscribing from topic:: %s.", JSON.stringify(payload).c_str());

  // Sending unsubscription request to Grandeur. and remove subscription request message from buffer for future reconnection.
  send("/topic/unsubscribe", payload, NULL);
  // Unset the update handler
  _subscriptions.off(String(topic));
  // Debuffer the subscription packet.
  _buffer.remove(eventId);
}

void DuplexHandler::duplexEventHandler(WStype_t eventType, uint8_t* message, size_t length) {
  // Resetting timeSinceLastMessage.
  // timeSinceLastMessage = millis();
  // Switch over event type
  switch(eventType) {
    case WStype_CONNECTED:
      DEBUG_GRANDEUR("Duplex channel established.");
      // When duplex connection opens
      _status = CONNECTED;
      // Running connection handler.
      _connectionHandler(_status);

      // Flushing all buffered messages to the channel.
      _buffer.forEach([=](const char* message) { sendMessage(message); });

      break;

    case WStype_DISCONNECTED:
      DEBUG_GRANDEUR("Duplex channel broke.");
      // When duplex connection closes
      _status = DISCONNECTED;
      // Running connection handler.
      _connectionHandler(_status);

      // Clear all tasks.
      _tasks.offAll();

      break;

    case WStype_TEXT:
      // When a duplex message is received.
      DEBUG_GRANDEUR("Message is received:: %s.", message);
      // Parsing the JSON message.
      Var oMessage = JSON.parse((char*) message);
      // Handling any parsing errors
      if (JSON.typeof(oMessage) == "undefined") {
        // Just for internal errors of Arduino_JSON
        // if the parsing fails.
        DEBUG_GRANDEUR("Parsing message failed!");
        return;
      }
      
      Var header = oMessage["header"];
      Var payload = oMessage["payload"];
      const char* task = header["task"];

      // ROUTES:
      // We do not need to handle the unpair event in Device SDKs.
      if(strcmp(task, "unpair") == 0);
      // Ping has no data so we simply emit.
      else if(strcmp(task, "ping") == 0);
      // If it is an update event rather than a task (response message).
      else if(strcmp(task, "update") == 0)
        publish(payload["event"], payload["path"], payload["update"]);
      // Otherwise: It's a response message for a task. So we receive it.
      else {
        receive(header, payload);

        // Debuffer the message except if it's a subscription request. This is to solve losing
        // subscriptions due to reconnection.
        if (strcmp(task, "/topic/subscribe") == 0);
        else
          _buffer.remove((gId) header["id"]);
      }
  }
}

void Buffer::push(gId id, String message) {
  _buffer[id] = message;
}

void Buffer::remove(gId id) {
  _buffer.erase(id);
}

void Buffer::forEach(std::function<void(const char*)> callback) {
  // Iterating through the _buffer running callback on each message.
  for(std::map<gId, String>::iterator it = _buffer.begin(); it != _buffer.end(); it++) {
    DEBUG_GRANDEUR("Flushing:: Id: %lu, Message: %s.", it->first, it->second.c_str());
    callback(it->second.c_str());
  }
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
