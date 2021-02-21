/**
 * @file grandeurtypes.h
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "macros.h"
#include "Var.h"
#include "Callback.h"

#ifndef GRANDEURTYPES_H_
#define GRANDEURTYPES_H_

// Macro to generate an Id anywhere in the SDK.
#define gid() micros();
// Type of the generated Id.
typedef unsigned long gId;

// EventID
typedef gId EventID;

//EventData
typedef Callback EventData;

// Event key
typedef String EventKey;

// Event payload
typedef String EventPayload;

// Event key
typedef String EventKey;

// Config class for storing configurations for duplex connection establishment.
class Config {
  public:
    String apiKey;
    String token;

    // Constructor
    Config() : apiKey(""), token("") {};
    Config(String apiKey, String token) : apiKey(apiKey), token(token) {};
};

// Data structure to store data in Buffer.
struct Message {
  gId id;
  String str; // Full packet containing task and payload after being stringified.

  Message(gId id, String message) : id(id), str(message) {}
};

#endif