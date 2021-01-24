/**
 * @file grandeurtypes.h
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "grandeurmacros.h"
#include <cstring>
#include "Arduino_JSON/Arduino_JSON.h"

#ifndef GRANDEURTYPES_H_
#define GRANDEURTYPES_H_

// Include json
typedef JSONVar JSONObject;

// Define callback
typedef void (*Callback)(JSONObject);

// Define send function
typedef void (*Send)(const char* task, const char* payload, Callback callback);

// Define Grandeur ID
typedef long GrandeurID;

// Config class for storing configurations of connection
class Config {
  public:
    String apiKey;
    String token;

    // Constructor
    Config(String apiKey, String token);
    Config();
};

// Class for Send Queue Data
class SendData {
  public:
    char task[TASK_SIZE];
    char payload[PACKET_SIZE];
    Callback callback;

    // Constructor
    SendData(
      const char* task,
      const char* payload,
      Callback callback
    );
};

#endif