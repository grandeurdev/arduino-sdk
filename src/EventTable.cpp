/**
 * @file EventTable.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

#include <EventTable.h>
#include <iostream>

const char* events[T_S] =
  {"ping", "subscribeTopic", "unsubscribeTopic", "unsubscribeAllTopics", "getDeviceSummary",
  "getDeviceParms", "setDeviceSummary", "setDeviceParms", "insertDocumentsDatastore"};

EventTableEntry::EventTableEntry(EventTableKey k, EventTableData v) {
  this->k= k;
  this->v = v;
  this->next = NULL;
}

EventTable::EventTable() {
  t = new EventTableEntry * [T_S];
  for (int i = 0; i < T_S; i++) {
    t[i] = NULL;
  }
}

int EventTable::hashFunc(EventTableKey k) {
  for(int i = 0; i < T_S; i++) {
    if(events[i] == k) {
      return i;
    }
  }
  return (T_S - 1);
}

int EventTable::insert(EventTableKey k, EventID id, EventData data) {
  int h = hashFunc(k);
  if(t[h] == NULL) {
    // If this bucket is empty,
    // creating the first entry
    t[h] = new EventTableEntry(k, {id, data});
    return 0;
  }
  if(t[h]->v.id == id) {
    // If entry is found in the first element,
    // replacing its data
    t[h]->v.data = data;
    return 0;
  }
  // If the bucket is not empty,
  // getting ready for traversing the chain
  EventTableEntry* p = t[h];
  while (p->next != NULL && p->next->v.id != id) {
    p = p->next;
  }
  if (p->next != NULL) {
    // If found an index without reaching the end of chain,
    // just replace the data
    p->next->v.data = data;
    return 0;
  }
  // If the end of chain is reached,
  // creating a new entry in the chain
  p->next = new EventTableEntry(k, {id, data});
  return 0;
}

int EventTable::remove(EventTableKey k, EventID id) {
  int h = hashFunc(k);
  if(t[h] == NULL) {
    // If this bucket is empty,
    // which means the entry does not exist
    return -1;
  }
  if(t[h]->v.id == id) {
    // If entry is found in the first element,
    // deleting it and pulling the chain backwards
    EventTableEntry* p = t[h];
    t[h] = t[h]->next;
    delete p;
    return 0;
  }
  // If the bucket is not empty,
  // getting ready for traversing the chain
  EventTableEntry* p = t[h];
  while (p->next != NULL) {
    if (p->next->v.id == id)
      // If element is found,
      // breaking out of the loop
      break;
    p = p->next;
  }
  if (p->next == NULL) {
    // If the end of the chain is reached,
    // and no element is found
    return -1;
  }
  // If the element is found
  EventTableEntry* q = p->next;
  p->next = p->next->next;
  delete q;
  return 0;
}

EventData EventTable::findAndRemove(EventTableKey k, EventID id) {
  int h = hashFunc(k);
  if(t[h] == NULL) {
    // If this bucket is empty,
    // which means the entry does not exist
    return NULL;
  }
  if(t[h]->v.id == id) {
    // If entry is found in the first element,
    // copying it, deleting it and pulling the chain backwards
    EventTableEntry* p = t[h];
    EventData data = p->v.data;
    t[h] = t[h]->next;
    delete p;
    return data;
  }
  // If the bucket is not empty,
  // getting ready for traversing the chain
  EventTableEntry* p = t[h];
  while (p->next != NULL) {
    if (p->next->v.id == id)
      // If element is found,
      // breaking out of the loop
      break;
    p = p->next;
  }
  if (p->next == NULL) {
    // If the end of the chain is reached,
    // and no element is found
    return NULL;
  }
  // If the element is found
  // copying its data, deleting it and pulling the chain backwards
  EventTableEntry* q = p->next;
  EventData data = q->v.data;
  p->next = p->next->next;
  delete q;
  return data;
}

/* ONLY FOR PRINTABLE DATA TYPES */
void EventTable::print() {
  for(int i = 0; i < T_S; i++) {
    EventTableEntry* p = t[i];
    std::cout<<i<<" -> ";
    while (p != NULL) {
      std::cout<<p->k<<" : ";
      std::cout<<p->v.id<< " : ";
      //std::cout<<p->v.data<<" -> ";
      p = p->next;
    }
    std::cout<<"\n";
  }
}

EventTable::~EventTable() {
  for (int i = 0; i < T_S; i++) {
    if (t[i] != NULL)
    for(EventTableEntry* p = t[i]; p != NULL; p = p->next) {
      delete p;
      t[i] = NULL;
    }
  }
  delete[] t;
}