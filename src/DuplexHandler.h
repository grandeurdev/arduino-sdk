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
#include "EventTable.h"
#include "EventQueue.h"
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
    static short _status;

    // Event Queue
    static EventQueue _queue;

    // Events Table
    static EventTable _eventsTable;

    // Subscriptions
    static EventTable _subscriptions;
    
    // Container for connection callback
    static void (*_connectionCallback)(bool);

    // Define function to handle the queued events
    static void handle(EventID id, EventKey key, EventPayload payload, Callback callback);

  public:
    // Constructor
    DuplexHandler(Config config);
    DuplexHandler();

    // Init the connection
    void init(void);

    // Ping function to keep connection alive.
    void ping();

    // Function to send a generic duplex message.
    static void send(const char* task, const char* payload, Callback callback);

    // Function to unsubscribe to a device topic.
    void unsubscribe(gID id, const char* payload);

    // Function to subscribe to a device topic.
    gID subscribe(const char* event, const char* payload, Callback updateHandler);

    // Function to schedule an event handler for connection with Grandeur
    void onConnectionEvent(void connectionEventHandler(bool));
    
    // Getter for connection state
    short getStatus(void);
    
    // This runs duplex
    void loop(bool valve);

    friend void duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length);
};

#endif