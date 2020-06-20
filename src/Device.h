/**
 * @file Device.h
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

// Including headers
#include "apollotypes.h"
#include "apollomacros.h"
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
    void getSummary(Callback callback);
    void getParms(Callback callback);
    void setSummary(JSONObject summary, Callback callback);
    void setParms(JSONObject parms, Callback callback);

    // Listeners for events from the Cloud
    void onSummaryUpdated(Callback callback);
    void onParmsUpdated(Callback callback);
};

#endif