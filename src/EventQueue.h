/**
 * @file EventQueue.h
 * @date 29.01.2021
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include <cstdlib>
#include <string>
#include "grandeurtypes.h"

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

// Event table entry is the block which represents
// an event
class EventQueueEntry {
  public:
    EventID id;
    EventKey key;
    EventPayload payload;
    Callback callback;
    EventQueueEntry* next;

    // Constructor
    EventQueueEntry(EventID id, EventKey key, EventPayload payload, Callback callback);
    EventQueueEntry(int k);

    // Override bool operator not
    bool operator!();
};

// The main class
class EventQueue {
  private:
    // Define queue 
    EventQueueEntry *queue;

    // Queue length
    int _length = 0;

  public:
    // Constructor for a table
    EventQueue();

    // Method to insert an entry into the queue
    int push(EventID id, EventKey key, EventPayload payload, Callback callback);

    // Method to remove an entry
    int remove(EventID id);

    // Method to loop over the elements in queue
    // and return the entry in callback
    void forEach(void (*callback)(EventID, EventKey, EventPayload, Callback));

    // Function return queue size
    int length();

    // Print queue
    void print();

    // Method to clear queue
    void clear();
    
    // Destructor for a queue
    ~EventQueue();
};

#endif