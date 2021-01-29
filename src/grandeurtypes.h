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

typedef JSONVar Var;
typedef void (*Callback)(Var);
typedef void (*Send)(const char* task, const char* payload, Callback callback);
typedef long GrandeurID;

// Config class for grandeur configurations
class Config {
  public:
    String apiKey;
    String token;
    Config(String apiKey, String token);
    Config();
};

// Class for Send Queue Data
class SendData {
  public:
    char task[TASK_SIZE];
    char payload[PACKET_SIZE];
    Callback callback;
    SendData(
      const char* task,
      const char* payload,
      Callback callback
    );
};

#endif