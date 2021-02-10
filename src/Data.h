/**
 * @file Data.h
 * @date 23.01.2021
 * @author Grandeur Technologies
 *
 * Copyright (c) 2020 Grandeur Technologies Inc. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Including headers
#include "grandeurtypes.h"
#include "grandeurmacros.h"
#include "DuplexHandler.h"

#ifndef DATA_H_
#define DATA_H_

class Event {
  // Class will help in clearing an event
  private:
    DuplexHandler* _duplex;
    String _deviceID;
    gId _id;
    String _path;

  public:
    Event(String deviceID, DuplexHandler* duplexHandler, gId id, String path);
    Event();

    // Clears an event
    void clear();
};

class Data {
  // Class that provides methods for working with device data.
  private:
    DuplexHandler* _duplex;
    String _deviceID;

  public:
    Data();
    Data(String deviceID, DuplexHandler* duplexHandler);

    // Async getter/setter methods:
    // Gets the variable specified in path and makes it available in cb function scope.
    void get(const char* path, Callback cb);
    // Gets all variables from Grandeur and makes them available in cb function scope.
    void get(Callback cb);
    // Sets the variable specified in path with what's in the data and schedules cb function for when
    // acknowledgement arrives from Grandeur.
    void set(const char* path, Var data, Callback cb);
    // Sets the variable specified in path with what's in the data without scheduling a function.
    void set(const char* path, Var data);

    // Sets a listener on update of a variable and runs cb function whenever the update occurs.
    Event on(const char* path, Callback cb);
    // Sets a listener on update of any variable and runs cb function whenever the update occurs.
    Event on(Callback cb);
};

#endif