#include "apollomacros.h"
#include <cstring>

#ifndef APOLLOTYPES_H_
#define APOLLOTYPES_H_
// Feed class for apollo payload parsing
class Feed {
    public:
        unsigned int numberOfKeys;
        char** keys;
        char** values;
        Feed(unsigned int numberOfKeys, char** keys, char** values);
        ~Feed();
        void print();
};

// Config class for apollo configurations
class Config {
    public:
        char deviceID[DEVICEID_SIZE];
        char apiKey[APIKEY_SIZE];
        char token[TOKEN_SIZE];
        char ssid[SSID_SIZE];
        char passphrase[PASSPHRASE_SIZE];
        Config(char* deviceID, char* apiKey, char* token, char* ssid, char* passphrase);
        ~Config();
};

// Callback function
typedef void (*Callback)(unsigned char*);

#endif APOLLOTYPES_H_