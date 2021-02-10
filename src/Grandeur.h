/**
 * @file Grandeur.h
 * @date 20.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Including headers
#include "Device.h"
#include "Datastore.h"
#include "DuplexHandler.h"

#ifndef GRANDEUR_H_
#define GRANDEUR_H_

class Grandeur;

class Project {
  // Class for handling a complete project
  private:
    // Duplex handles the realtime connection with the project.
    DuplexHandler _duplexHandler;
    Device _device;
    Datastore _datastore;
  public:
    // Project constructor
    Project();
    // Connection related methods
    Project& onConnection(void connectionCallback(bool));
    void clearConnectionCallback(void);
    bool isConnected(void);
    // Instantiator methods - return the objects of their classes
    Device& device(String deviceID);
    Datastore& datastore(void);

    // This method runs the SDK.
    void loop(bool valve);

    // Subclasses
    class Device;
    class Datastore;

    friend class Grandeur;
};

class Grandeur {
  public:
    
  private:
    Config _config;
    Project _project;
  public:
    Grandeur();
    Project& init(String apiKey, String token);
};

extern Grandeur grandeur;

#endif