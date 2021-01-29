/**
 * @file EventTable.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Inlcude event table
#include <EventTable.h>
#include <iostream>
#include <regex>

// Implement event table block
EventTableEntry::EventTableEntry(EventKey key, EventID id, EventData data) {
  this->key = key;
  this->id = id;
  this->data = data;
  this->next = NULL;
}

// Implement event table constructor
EventTable::EventTable() {
  // Init table as null
  table = NULL;
}

int EventTable::insert(EventKey key, EventID id, EventData data) {
  // If bucket is empty,
  if(table == NULL) {
    // creating the first entry
    table = new EventTableEntry(key, id, data);
    return 0;
  }

  // Otherwise resolve the base case where the starting block id
  // is duplicate of the new supllied
  if(table->id == id) {
    // If entry is found in the first element,
    // replacing its data
    table->data = data;
    return 0;
  }

  // If the bucket is not empty,
  // getting ready for traversing the chain
  EventTableEntry * p = table;

  // Move to next block while we aren't at the end 
  while (p->next != NULL) {
    // or we haven't found a duplicate in next block
    if (p->next->id == id) break;

    // Keep moving
    p = p->next;
  }

  // When found a duplicate
  if (p->next != NULL) {
    // Without reaching the end of chain,
    // just replace the data
    p->next->data = data;
    return 0;
  }

  // If the end of chain is reached,
  // creating a new entry in the chain
  p->next = new EventTableEntry(key, id, data);
  return 0;
}

int EventTable::remove(EventKey key, EventID id) {
  // Check if bucket is empty
  if(table == NULL) {
    // which means the entry does not exist
    return -1;
  }

  // If the id was found in first block
  if(table->id == id) {
    // Pull the chain backwards
    EventTableEntry* p = table;
    table = table->next;

    // and delete it
    delete p;
    return 0;
  }

  // If the bucket is not empty,
  // getting ready for traversing the chain
  EventTableEntry* p = table;

  // While not the end has reached 
  while (p->next != NULL) {
    // Break if the next block has the id we need
    if (p->next->id == id) break;

    // Keep moving
    p = p->next;
  }

  // Handle case where we reached the end
  if (p->next == NULL) {
    // No element is found
    return -1;
  }

  // If the element is found
  // Then pull the chain backward
  EventTableEntry* q = p->next;
  p->next = p->next->next;

  // and delete it
  delete q;
  return 0;
}

EventData EventTable::findAndRemove(EventKey key, EventID id) {
  // Check if bucket is empty
  if(table == NULL) {
    // which means the entry does not exist
    return NULL;
  }

  // If the id was found in first block
  if(table->id == id) {
    // Pull the chain backwards
    EventTableEntry* p = table;
    EventData data = p->data;
    table = table->next;

    // and delete it
    delete p;
    return data;
  }

  // If the bucket is not empty,
  // getting ready for traversing the chain
  EventTableEntry* p = table;

  // While not the end has reached 
  while (p->next != NULL) {
    // Break if the next block has the id we need
    if (p->next->id == id) break;

    // Keep moving
    p = p->next;
  }

  // Handle case where we reached the end
  if (p->next == NULL) {
    // No element is found
    return NULL;
  }

  // If the element is found
  // Then pull the chain backward
  EventTableEntry* q = p->next;
  EventData data = p->data;
  p->next = p->next->next;

  // and delete it
  delete q;
  return data;
}

int EventTable::emit(EventKey key, Var packet, const char* path) {
  // Cast packet as per type of packet
  String type = JSON.typeof_(packet);

  // If the bucket is not empty,
  // getting ready for traversing the chain
  EventTableEntry* p = table;

  // While not the end has reached 
  while (p != NULL) {
    // Define pattern
    std::regex pattern("(" + p->key + ")(.*)");

    // We will use regex to match the key of the block
    if (std::regex_match(key, pattern)) {
      // Then emit packet to callback
      // Check the callback type
      if (p->data.type() == "object") {
        // The callback receives an object
        // so we don't to do the type conversion
        // Send with casting as Var
        p->data((Var) packet, path);
      }
      else {
        // Packet was boolean
        if (type == "boolean") {
          // Cast as boolean
          p->data((bool) packet, path);
          
        }

        // Packet is number
        else if (type == "number") {
          // Figure out that if it is a double or int
          if ((double) packet - (int) packet != 0)
            // Cast as double
            p->data((double) packet, path);

          else 
            // Cast as int
            p->data((int) packet, path);

        }

        // Packet was string
        else if  (type == "string") {
          // Cast as string
          p->data((const char*) packet, path);

        }

        // Packet was object or array
        else if (type == "object" || type == "array") {
          // Send with casting as Var
          p->data((Var) packet, path);

        }
      }
    }
    // Keep moving
    p = p->next;
  }

  return 0;
}

void EventTable::print() {
  // Loop over the event table
  EventTableEntry* p = table;

  // Print header
  Serial.printf("\n*******Table*******\n");

  // While not the end
  while (p != NULL) {
    // Print the id
    Serial.printf("%s %d\n", p->key.c_str(), p->id);

    p = p->next;
  }

  // Print footer
  Serial.printf("*******************\n");
}

void EventTable::clear() {
  // Function to delete all blocks
  // If the bucket is not empty,
  // getting ready for traversing the chain
  EventTableEntry* p = table;

  // While not the end has reached
  while (p != NULL) {
    // Copy block address to a pointer
    EventTableEntry* next = p->next;

    // delete p
    delete p;

    // Move next
    p = next;
  }

  // Mark table as empty
  table = NULL;
}

EventTable::~EventTable() {
  clear();
}