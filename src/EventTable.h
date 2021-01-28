/**
 * @file EventTable.h
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include <cstdlib>
#include <string>
#include "grandeurtypes.h"

#ifndef EVENTTABLE_H_
#define EVENTTABLE_H_

// Event table entry is the block which represents
// an event
class EventTableEntry {
  public:
    EventKey key;
    EventID id;
    EventData data;
    EventTableEntry* next;

    // Constructor
    EventTableEntry(EventKey key, EventID id, EventData data);
};

// The main class
class EventTable {
  private:
    // Define table 
    EventTableEntry *table;

  public:
    // Constructor for a table
    EventTable();

    // Method to insert an entry into the hashtable
    int insert(EventKey key, EventID id, EventData data);

    // Method to remove an entry from the table
    int remove(EventKey key, EventID id);

    // Method to find and remove an entry from the hashtable
    EventData findAndRemove(EventKey key, EventID id);

    // Method to send some data to all callbacks at a key
    int emit(EventKey key, Var packet, const char* path);

    // Print table entreis
    void print();

    // Clear event table entries
    void clear();
    
    // Destructor for a hashtable
    ~EventTable();
};

#endif