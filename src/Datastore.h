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

class Project::Datastore {
  // Class for handling datastore related functions
  private:
    DuplexHandler _duplex;
    // Subclasses
    class Collection;
  public:
    // Datastore constructor
    Datastore(DuplexHandler duplexHandler);
    Datastore();
    // Methods
    Collection collection(String name);
};

class Project::Datastore::Collection {
  // Class for a datastore collection
  private:
    String _name;
    DuplexHandler _duplex;
    // Subclasses
    class Pipeline;
  public:
    // Collection constructor
    Collection(String name, DuplexHandler duplexHandler);
    // Methods
    void insert(Var documents, Callback inserted);
    void remove(Var filter, Callback removed);
    void update(Var filter, Var update, Callback updated);
    void search(Var filter, Var projection, int pageNumber, Callback searched);
    Pipeline pipeline(void);
};

class Project::Datastore::Collection::Pipeline {
  // Class for a datastore collection
  private:
    String _collection;
    Var _query;
    DuplexHandler _duplex;
  public:
    // Collection constructor
    Pipeline(String collectionName, Var query, DuplexHandler duplexHandler);
    // Methods
    Pipeline match(Var filter);
    Pipeline project(Var specs);
    Pipeline group(Var condition, Var fields);
    Pipeline sort(Var specs);
    void execute(int pageNumber, Callback executed);
};

#endif