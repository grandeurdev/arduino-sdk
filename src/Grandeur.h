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
    DuplexHandler _duplexHandler;

  public:
    // Project constructor
    Project();

    // Connection related methods
    void onConnection(void connectionCallback(bool));
    bool isConnected(void);

    // Instantiator methods - return the objects of their classes
    Device device(String deviceID);
    Datastore datastore(void);

    // This method runs the SDK.
    void loop(bool valve);

    friend class Grandeur;
};

class Grandeur {
  private:
    // Create new config object
    Config _config;
    
  public:
    // Constructor of Grandeur
    Grandeur();

    // Function to init the connection
    Project init(String apiKey, String token);
};

extern Grandeur grandeur;

#endif