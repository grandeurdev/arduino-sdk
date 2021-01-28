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
#include <functional>
#include "EventEmitter/EventEmitter.h"
#include "grandeurtypes.h"
#include "grandeurmacros.h"
#include "arduinoWebSockets/WebSocketsClient.h"

#ifndef DUPLEXHANDLER_H_
#define DUPLEXHANDLER_H_

class DuplexHandler {
  private:
    // Connection query
    String _query;
    // Connection token
    String _token;
    // Connection state variable
    bool _status;
    // Events Table
    EventEmitter<JSONObject> _tasks;
    // Subscription Array for update handler functions
    EventEmitter<JSONObject> _subscriptions;
    // Container for connection callback
    void (*_connectionCallback)(bool);
    // Queue to store packets
    //JSONObject _queue[10];
    // List of subscribable events
    const char* _events[1] = {"data"};

    WebSocketsClient _client;
    void duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length);

    //void handle(void);
    //void flush(void);

  public:
    DuplexHandler();
    void init(Config config);
    // Ping function to keep connection alive.
    void ping();
    // Function to send a generic duplex message.
    void send(const char* task, const char* payload, Callback callback);
    // Function to subscribe to a device topic.
    std::function<void(void)> subscribe(const char* topic, String path, const char* payload, Callback updateHandler);
    // Function to schedule an event handler for connection with Grandeur
    void onConnectionEvent(void connectionCallback(bool));
    void clearConnectionCallback(void);
    
    // Getter for connection state
    short getStatus(void);
    
    // This runs duplex
    void loop(bool valve);

    //friend void duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length);

};

#endif