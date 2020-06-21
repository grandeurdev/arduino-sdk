/**
 * @file Apollo.h
 * @date 20.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

// Including headers
#include "Device.h"
#include "Datastore.h"
#include "DuplexHandler.h"

#ifndef APOLLO_H_
#define APOLLO_H_

class Apollo;

class Project {
  // Class for handling a complete project
  private:
    DuplexHandler _duplexHandler;
    // Device ID limits the scope of this project reference to only this device.
    String _deviceID;
  public:
    // Project constructor
    Project();
    // Connection related methods
    void onConnection(void connectionCallback(bool));
    bool isConnected(void);
    // Instantiator methods - return the objects of their classes
    Device device(void);
    Datastore datastore(void);

    // This method runs the SDK.
    void loop(bool valve);

    friend class Apollo;
};

class Apollo {
  private:
    Config _config;
  public:
    Apollo();
    Project init(String apiKey, String deviceID, String token);
};

extern Apollo apollo;

#endif