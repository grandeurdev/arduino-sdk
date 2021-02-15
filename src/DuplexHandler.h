/**
 * @file DuplexHandler.h
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Including headers
#include "EventEmitter/EventEmitter.h"
#include "EventQueue.h"
#include "grandeurtypes.h"
#include "grandeurmacros.h"
#include "arduinoWebSockets/WebSocketsClient.h"

#ifndef DUPLEXHANDLER_H_
#define DUPLEXHANDLER_H_

// Class to establish and handle real-time communication channel with Grandeur and send/receive
// messages on this channel.
class DuplexHandler {
  private:
    // We make connection with Grandeur over Websockets.
    // This variable stores information about the websockets connection.
    WebSocketsClient _client;
    // Store query and access token to use while establishing the connection with Grandeur.
    String _query;
    String _token;

    // Stores current status (CONNECTED / DISCONNECTED) of the connection.
    bool _status;
    
    // Points to the connection handler function to call when connection with Grandeur is successfully
    // estbalished.
    void (*_connectionHandler)(bool);
    // List of subscribable events
    const char* _events[1] = {"data"};
    
    // Events Table
    EventEmitter<String, Callback> _tasks;
    // Subscriptions
    EventEmitter<String, Callback> _subscriptions;

    void duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length);

    // Event Queue
    EventQueue _queue;

    // Define function to handle the queued events
    void handle(EventID id, EventKey key, EventPayload payload, Callback callback);

  public:
    // Constructor
    DuplexHandler();
    void init(Config config);
    // Ping function to keep connection alive.
    void ping();
    // Sends a generic duplex message.
    void send(const char* task, const char* payload, Callback cb);

    // Unsubscribes from a topic.
    void unsubscribe(gId id, const char* payload);

    // Subscribes to a topic.
    gId subscribe(const char* event, const char* payload, Callback updateHandler);

    // Schedules a connection handler function to be called when connection with Grandeur
    // establishes/drops.
    void onConnectionEvent(void connectionCallback(bool));
    // Removes the connection handler function.
    void clearConnectionCallback(void);
    
    // Gets current status (CONNECTED / DISCONNECTED) of the connection.
    bool getStatus(void);
    
    // This runs duplex
    void loop(bool valve);

    #if DEBUG
    // Defines these functions when in debug mode.
    void (*connectionHandler)(bool) = _connectionHandler;
    const char* query = _query.c_str();
    const char* token = _token.c_str();
    #endif /* DEBUG */
};

#endif