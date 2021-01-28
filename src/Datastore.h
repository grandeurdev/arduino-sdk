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
    // Dedine collection name
    String _collection;

    // Variable to store query
    Var _query;

    // To store reference of duplex
    DuplexHandler _duplex;

  public:
    // Collection constructor
    Pipeline(String collection, Var query, DuplexHandler duplexHandler);

    // Method to add match stage
    Pipeline match(Var filter);

    // Function to add project stage
    Pipeline project(Var specs);

    // Function to add group stage
    Pipeline group(Var condition, Var fields);

    // Function to add sort stage to query
    Pipeline sort(Var specs);

    // Execute the query by sending function 
    void execute(int nPage, Callback executed);
};

class Collection {
  // Class for a datastore collection
  private:
    // Define name of collection
    String _name;

    // Store duplex handler reference
    DuplexHandler _duplex;

  public:
    // Collection constructor
    Collection(String name, DuplexHandler duplexHandler);

    // Function to insert document to a collection
    void insert(Var documents, Callback inserted);

    // Function to remove document from a collection
    void remove(Var filter, Callback removed);

    // Function to update a document
    void update(Var filter, Var update, Callback updated);

    // Function to perform a search
    void search(Var filter, Var projection, int nPage, Callback searched);

    // Constructor
    Pipeline pipeline(void);
};

class Datastore {
  // Class for handling datastore related functions
  private:
    // Variable to store duplex reference
    DuplexHandler _duplex;

  public:
    // Datastore constructor
    Datastore(DuplexHandler duplexHandler);
    Datastore();

    // Method to get reference to constructor
    Collection collection(String name);
};

#endif