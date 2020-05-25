/**
 * @file apollotypes.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

#include <apollotypes.h>
#include "Arduino.h"

Config::Config(
  const char* deviceID,
  const char* apiKey,
  const char* token,
  const char* ssid,
  const char* passphrase
  ) {
  strcpy(this->deviceID, deviceID);
  strcpy(this->apiKey, apiKey);
  strcpy(this->token, token);
  strcpy(this->ssid, ssid);
  strcpy(this->passphrase, passphrase);
}


Config::~Config() {
    
}

SendData::SendData(const char* task, const char* payload, Callback callback) {
  strcpy(this->task, task);
  strcpy(this->payload, payload);
  this->callback = callback;
}


SendData::~SendData() {
}