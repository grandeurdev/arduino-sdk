#include "apollotypes.h"
#include "apollomacros.h"
#include <Arduino.h>

class PayloadHandler {
    // Class for handling payload format to or from apollo server
    private:
        
    public:
        int getPayloadKeys(uint8_t* packet, char** keys, size_t numberOfKeys, size_t sizeOfKey);
        int getFeed(uint8_t* packet, Feed* feed, uint_fast16_t maxSizeOfValue);
        int getValues(uint8_t* packet, char** keys, uint_fast16_t numberOfKeys, char** values, uint_fast16_t maxSizeOfValues);
        int getValue(uint8_t* packet, char* key, char* value);
        int parsePacket(uint8_t* packet, Feed* feed, uint_fast16_t maxSizeOfValue = 10);
};