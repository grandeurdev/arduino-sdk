#include "apollomacros.h"
#include <cstring>

#ifndef APOLLOTYPES_H_
#define APOLLOTYPES_H_
// Payload class for apollo payload parsing
class Payload {
    public:
        unsigned int numberOfKeys;
        char** keys;
        char** values;
        Payload(unsigned int numberOfKeys, char** keys, char** values);
        ~Payload();
};

// Config class for apollo configurations
class Config {
    public:
        char deviceID[DEVICEID_SIZE];
        char apiKey[APIKEY_SIZE];
        char token[TOKEN_SIZE];
        char ssid[SSID_SIZE];
        char passphrase[PASSPHRASE_SIZE];
        Config(char* apiKey, char* token, char* ssid, char* passphrase);
        ~Config();
};

// Callback function
typedef void (*Receiver)(unsigned char*);
// Callback function
typedef void (*Callback)(Payload*);

#endif APOLLOTYPES_H_