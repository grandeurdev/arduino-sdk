/**
 * @file Datastore.h
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

// Including headers
#include "DuplexHandler.h"

#ifndef DATASTORE_H_
#define DATASTORE_H_

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

    friend class Apollo;
};

class Datastore {
  // Class for handling datastore related functions
  private:
    DuplexHandler _duplex;
    String _deviceID;
  public:
    // Datastore constructor
    Datastore(String deviceID, DuplexHandler duplexHandler);
    Datastore();
    // Methods
    Collection collection(String name);

    friend class Apollo;
};

#endif