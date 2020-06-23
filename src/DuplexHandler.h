/**
 * @file DuplexHandler.h
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

// Including headers
#include "EventTable.h"
#include "apollotypes.h"
#include "apollomacros.h"
#include "DuplexClient.h"

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
    // Events Table
    static EventTable _eventsTable;
    // Subscription Array for update handler functions
    static Callback _subscriptions[4];
    
    // Container for connection callback
    static void (*_connectionCallback)(bool);

  public:
    DuplexHandler(Config config);
    DuplexHandler();
    void init(void);
    // Ping function to keep connection alive.
    void ping();
    // Function to send a generic duplex message.
    static void send(const char* task, const char* payload, Callback callback);
    // Function to subscribe to a device topic.
    void subscribe(short event, const char* payload, Callback updateHandler);
    // Function to schedule an event handler for connection with the Cloud
    void onConnectionEvent(void connectionEventHandler(bool));
    
    // Getter for connection state
    short getStatus(void);
    
    // This runs duplex
    void loop(bool valve);

    friend void duplexEventHandler(WStype_t eventType, uint8_t* packet, size_t length);

};

#endif