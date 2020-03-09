#include "apollotypes.h"

Payload::Payload(unsigned int numberOfKeys, char** keys, char** values) {
    this->numberOfKeys = numberOfKeys;
    this->keys = keys;
    this->values = values;
}

Payload::~Payload() {
    for(int i = 0; i < this->numberOfKeys; i++) {
        //delete[] this->values[i];
    }
}

Config::Config(char* apiKey, char* token, char* ssid, char* passphrase) {
    strcpy(this->apiKey, apiKey);
    strcpy(this->token, token);
    strcpy(this->ssid, ssid);
    strcpy(this->passphrase, passphrase);
}


Config::~Config() {
    
}