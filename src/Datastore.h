/**
 * @file Datastore.h
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Including headers
#include "DuplexHandler.h"

#ifndef DATASTORE_H_
#define DATASTORE_H_

class Pipeline {
  // Class for a datastore collection
  private:
    String _collection;
    JSONObject _query;
    DuplexHandler _duplex;
  public:
    // Collection constructor
    Pipeline(String collectionName, JSONObject query, DuplexHandler duplexHandler);
    // Methods
    Pipeline match(JSONObject filter);
    Pipeline project(JSONObject specs);
    Pipeline group(JSONObject condition, JSONObject fields);
    Pipeline sort(JSONObject specs);
    void execute(int pageNumber, Callback executed);
};

class Collection {
  // Class for a datastore collection
  private:
    String _name;
    DuplexHandler _duplex;
  public:
    // Collection constructor
    Collection(String name, DuplexHandler duplexHandler);
    // Methods
    void insert(JSONObject documents, Callback inserted);
    void remove(JSONObject filter, Callback removed);
    void update(JSONObject filter, JSONObject update, Callback updated);
    void search(JSONObject filter, JSONObject projection, int pageNumber, Callback searched);
    Pipeline pipeline(void);
};

class Datastore {
  // Class for handling datastore related functions
  private:
    DuplexHandler _duplex;
  public:
    // Datastore constructor
    Datastore(DuplexHandler duplexHandler);
    Datastore();
    // Methods
    Collection collection(String name);
};

#endif