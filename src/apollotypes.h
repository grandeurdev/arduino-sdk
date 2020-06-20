/**
 * @file apollotypes.h
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

#include "apollomacros.h"
#include <cstring>
#include "Arduino_JSON/Arduino_JSON.h"

#ifndef APOLLOTYPES_H_
#define APOLLOTYPES_H_

typedef JSONVar JSONObject;
typedef void (*Callback)(JSONObject);
typedef void (*Send)(const char* task, const char* payload, Callback callback);
typedef long ApolloID;

// Config class for apollo configurations
class Config {
  public:
    String deviceID;
    String apiKey;
    String token;
    Config(String deviceID, String apiKey, String token);
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