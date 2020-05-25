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
typedef short ApolloState;

// Config class for apollo configurations
class Config {
  public:
    char deviceID[DEVICEID_SIZE];
    char apiKey[APIKEY_SIZE];
    char token[TOKEN_SIZE];
    char ssid[SSID_SIZE];
    char passphrase[PASSPHRASE_SIZE];
    Config(
      const char* deviceID,
      const char* apiKey,
      const char* token,
      const char* ssid,
      const char* passphrase
    );
    ~Config();
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
        ~SendData();
};

#endif