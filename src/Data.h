/**
 * @file Data.h
 * @date 23.01.2021
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Including headers
#include "grandeurtypes.h"
#include "grandeurmacros.h"
#include "DuplexHandler.h"

#ifndef DATA_H_
#define DATA_H_

class Data {
  // Class for handling device related functions
  private:
    DuplexHandler _duplex;
    String _deviceID;

  public:
    // Data constructor
    Data(String deviceID, DuplexHandler duplexHandler);
    Data();

    // Async getter/setter methods
    void get(const char* path, Callback<JSONObject> callback);
    void set(const char* path, JSONObject data, Callback<JSONObject> callback);
    void set(const char* path, bool data, Callback<JSONObject> callback);
    void set(const char* path, int data, Callback<JSONObject> callback);
    void set(const char* path, long data, Callback<JSONObject> callback);
    void set(const char* path, double data, Callback<JSONObject> callback);
    void set(const char* path, const char* data, Callback<JSONObject> callback);

    // Sync event handlers
    void on(const char* path, Callback<JSONObject> callback);
};

#endif