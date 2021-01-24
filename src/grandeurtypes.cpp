/**
 * @file grandeurtypes.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include <grandeurtypes.h>
#include "Arduino.h"

// Define config class
Config::Config(String apiKey, String token) {
  this->apiKey = apiKey;
  this->token = token;
}

// Config override constructor
Config::Config() {
  this->apiKey = "";
  this->token = "";
}

// Object which stores the task while it is on queue
SendData::SendData(const char* task, const char* payload, Callback callback) {
  strcpy(this->task, task);
  strcpy(this->payload, payload);
  this->callback = callback;
}