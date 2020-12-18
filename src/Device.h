/**
 * @file Device.h
 * @date 20.06.2020
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

#ifndef DEVICE_H_
#define DEVICE_H_

class Device {
  // Class for handling device related functions
  private:
    DuplexHandler _duplex;
    String _deviceID;
  public:
    // Device constructor
    Device(String deviceID, DuplexHandler duplexHandler);
    Device();

    // Async getter/setter methods
    void get(String path, Callback callback);
    void get(Callback callback);
    void set(String path, JSONObject data, Callback callback);
    void set(String path, JSONObject data);
    // Listeners for events from Grandeur
    void on(String path, Callback callback);
    void on(Callback callback);
};

#endif