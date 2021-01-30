/**
 * @file EventQueue.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Inlcude event table
#include <EventQueue.h>
#include <iostream>

// Implement event queue block
EventQueueEntry::EventQueueEntry(EventID id, EventKey key, EventPayload payload, Callback callback) {
  this->id = id;
  this->key = key;
  this->payload = payload;
  this->callback = callback;
  this->next = NULL;
}

EventQueueEntry::EventQueueEntry(int k) {
  this->id = NULL;
}

bool EventQueueEntry::operator!() {
  // If the queue block was not set as null
  // then return true
  if (id != NULL) {
    return true;
  }

  // Otherwise return false
  return false;
}

// Implement event queue constructor
EventQueue::EventQueue() {
  // Init queue as null
  queue = NULL;
}

int EventQueue::push(EventID id, EventKey key, EventPayload payload, Callback callback) {
  // We will insert a block at the head
  // Store the reference to next of queue
  EventQueueEntry* p = queue;

  // Then assign the queue to new block
  queue = new EventQueueEntry(id, key, payload, callback);

  // Then assign queue next of old block
  queue->next = p;

	// Plus the length
	_length++;

  return 0;
}

int EventQueue::remove(EventID id) {
  // Check if bucket is empty
  if(queue == NULL) {
    // which means the entry does not exist
    return -1;
  }

  // If the id was found in first block
  if(queue->id == id) {
    // Pull the chain backwards
    EventQueueEntry* p = queue;
    queue = queue->next;

    // and delete it
    delete p;
    return 0;
  }

  // If the bucket is not empty,
  // getting ready for traversing the chain
  EventQueueEntry* p = queue;

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
  EventQueueEntry* q = p->next;
  p->next = p->next->next;

  // and delete it
  delete q;
  return 0;
}

void EventQueue::forEach(void (*callback)(EventID, EventKey, EventPayload, Callback)) {
  // Loop over the queue
  EventQueueEntry* p = queue;

  // While not the end
  while (p != NULL) {
    // Call the callback
    callback(p->id, p->key, p->payload, p->callback);

    // Move to next
    p = p->next;
  }
}

int EventQueue::length() {
	// Return length of queue
	return _length;
}

void EventQueue::print() {
  // Loop over the queue
  EventQueueEntry* p = queue;

  // Print header
  Serial.printf("\n*******Queue*******\n");

  // While not the end
  while (p != NULL) {
    // Print the id
    Serial.printf("%s %d\n", p->key.c_str(), p->id);

    p = p->next;
  }

  // Print footer
  Serial.printf("*******************\n");
}

void EventQueue::clear() {
  // Function to delete all blocks
  // If the bucket is not empty,
  // getting ready for traversing the chain
  EventQueueEntry* p = queue;

  // While not the end has reached
  while (p != NULL) {
    // Copy block address to a pointer
    EventQueueEntry* next = p->next;

    // delete p
    delete p;

    // Move next
    p = next;
  }

  // Mark queue as empty
  queue = NULL;
	_length = 0;
}

EventQueue::~EventQueue() {
  clear();
}