/**
 * @file apollotypes.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Hardware SDK for Grandeur Cloud.
 *
 */

#include <apollotypes.h>
#include "Arduino.h"

Feed::Feed(unsigned int numberOfKeys, char** keys, char** values) {
    this->numberOfKeys = numberOfKeys;
    this->keys = new char*[numberOfKeys];
    memcpy(this->keys, keys, sizeof(char*) * numberOfKeys);
    this->values = values;
}

void Feed::print() {
    for(int i = 0; i < this->numberOfKeys; i++) {
        Serial.printf("%s: %s\n", this->keys[i], this->values[i]);
    }
}

Feed::~Feed() {
    for(int i = 0; i < this->numberOfKeys; i++) {
        delete[] this->values[i];
    }
}

Config::Config(char* deviceID, char* apiKey, char* token, char* ssid, char* passphrase) {
    strcpy(this->deviceID, deviceID);
    strcpy(this->apiKey, apiKey);
    strcpy(this->token, token);
    strcpy(this->ssid, ssid);
    strcpy(this->passphrase, passphrase);
}


Config::~Config() {
    
}