#include "payloadparser.h"
#include "jsmn/jsmn.h"

jsmn_parser parser;
jsmntok_t tokens[128];

int PayloadHandler::getValue(uint8_t* packet, char* key, char* value) {
    if(*key == '\0') {
        // If key is empty
        value[0] = '\0';
        return 0;
    }
    // Converting uint8_t to char*
    char* data = (char*) packet;
    
    // Finding key in the payload
    char *keyPtr = strstr(data, key);
    if (keyPtr != NULL) {
        // If key is found in the payload
        int start = false;
        
        // Traversing it to find the value
        for (int i = 0, j = 0; i < strlen(keyPtr); i++) {
            if (keyPtr[i] == ',' || keyPtr[i] == '}') {
                // Ending copying condition
                if(keyPtr[i - 1] == '"')
                    j--;
                value[j] = '\0';
                break;
            }

            if (start) {
                // Copying the value
                value[j++] = keyPtr[i];
            }

            if (keyPtr[i] == ':') {
                while(keyPtr[i + 1] == ' ')
                    i++;
                if(keyPtr[i + 1] == '"')
                    i++;
                // Start copying condition
                start = true;
            }
        }
        // If value is copied
        return 0;
    }

    // If value is not found
    return -1;
}

int PayloadHandler::getValues(uint8_t* packet, char** keys, uint_fast16_t numberOfKeys, char** values, uint_fast16_t maxSizeOfValue) {
    for(int i = 0; i < numberOfKeys; i++) {
        char* value = new char[maxSizeOfValue];
        if(getValue(packet, keys[i], value) == 0) {
            values[i] = new char[maxSizeOfValue];
            memcpy(values[i], value, sizeof(char) * maxSizeOfValue);
        }
        else {
            return -1;
        }
    }
    return 0;
}

int PayloadHandler::getPayloadKeys(uint8_t* packet, char** keys, size_t numberOfKeys, size_t sizeOfKey) {
    // Traversing to the starting bracket of the payload
    char* data = strstr((char*) packet, "payload");
    if(data == NULL) {
        // If no payload is present in the packet
        for(int i = 0; i < numberOfKeys; i++) {
            keys[i] = new char[sizeOfKey];
            *keys[i] = '\0';
        }
        return 0;
    }
    data = strchr(data, '{');
    int brackets = 1;
    int start = false;
    for(int i = 0, j = 0, c = 0; i < strlen(data); i++) {
        // Traversing the payload
        char* temp;
        if(data[i] == '}') {
            if(--brackets == 0) {
                // If closing bracket of payload is reached
                while(j < numberOfKeys) {
                    keys[j] = new char[sizeOfKey];
                    *keys[j++] = '\0';
                }
                break;
            }
        }
        if(!start && data[i] == ':') {
            // Ignoring value in key-value pair
            while(data[i] != ',') {
                if(data[i] == '{') {
                    brackets++;
                    j--;
                    break;
                }
                if(data[i] == '}') {
                    brackets--;
                    break;
                }
                i++;
            }
        }

        if (start && data[i] == '"') {
            // Ending copying condition
            temp[c] = '\0';
            start = false;
            keys[j++] = temp;
            if(j >= numberOfKeys)
                // If limit on number of keys is reached, breaking out of loop
                break;
            // Going to the start of the loop
            continue;
        }

        if (start) {
            // Copying the value
            temp[c++] = data[i];
        }

        if (!start && data[i] == '"' && data[i + 1] != ':') {
            // Start copying condition
            start = true;
            temp = new char[sizeOfKey];
            c = 0;
        }
    }
    if(start) {
        // If closing condition was not satisfied
        return -1;
    }
    return 0;
}

int PayloadHandler::getFeed(uint8_t* packet, Feed* feed, uint_fast16_t maxSizeOfValue = 10) {
    if(getValues(packet, feed->keys, feed->numberOfKeys, feed->values, maxSizeOfValue) == 0) {
        return 0;
    }
    else {
        return -1;
    }
}

int PayloadHandler::parsePacket(uint8_t* packet, Feed* feed, uint_fast16_t maxSizeOfValue = 10) {
    jsmn_init(&parser);
    int r = jsmn_parse(&parser, (char*) packet, strlen((char*) packet), tokens,
                    sizeof(tokens) / sizeof(tokens[0]));
    if (r < 0) {
        return r;
    }

    if (r < 1 || tokens[0].type != JSMN_OBJECT) {
        // If top-level element is not an object
        return -1;
    }

    // Number of keys in JSON object
    feed->keys = new char*[r];
    int k = 0;
    // Looping over all parsed tokens
    for (int i = 1; i < r; i++) {
        if (tokens[i + 1].type == JSMN_OBJECT) {
            Serial.println("Object");
            // If the key contains an object
            // Looping over child keys of the object
            for (int j = 1; j < tokens[i + 1].size * 2; j += 2) {
                if (tokens[i + j + 2].type == JSMN_OBJECT) {
                    // If the child key contains an object
                    break;
                }
                feed->keys[k] = new char[tokens[i + j + 1].end - tokens[i + j + 1].start];
                memcpy(feed->keys[k], packet + tokens[i + j + 1].start, tokens[i + j + 1].end - tokens[i + j + 1].start);
                feed->keys[k++][tokens[i + j + 1].end - tokens[i + j + 1].start] = '\0';
                Serial.printf("Key is: %s\n", feed->keys[k - 1]);
                Serial.printf("- %.*s: %.*s\n", tokens[i + j + 1].end - tokens[i + j + 1].start,
                    (char*) packet + tokens[i + j + 1].start,
                    tokens[i + j + 2].end - tokens[i + j + 2].start,
                    (char*) packet + tokens[i + j + 2].start);
            }
            i += tokens[i + 1].size * 2 + 1;
            /* We may additionally check if the value is either "true" or "false" */
        }
        else if (tokens[i + 1].type == JSMN_STRING) {
            Serial.printf("String: %.*s\n", tokens[i].end - tokens[i].start, (char*) packet + tokens[i].start);
            // If the key contains a string
            /* We may use strndup() to fetch string value */
            // Copying the key name
            feed->keys[k] = new char[tokens[i].end - tokens[i].start];
            memcpy(feed->keys[k++], (char*) packet + tokens[i].start, tokens[i].end - tokens[i].start);
            feed->keys[k++][tokens[i].end - tokens[i].start] = '\0';
            Serial.printf("Key is: %s\n", feed->keys[k - 1]);
            Serial.printf("- %.*s: %.*s\n", tokens[i].end - tokens[i].start,
                    (char*) packet + tokens[i].start, tokens[i + 1].end - tokens[i + 1].start,
                    (char*) packet + tokens[i + 1].start);
            i++;
        } else if (tokens[i + 1].type == JSMN_PRIMITIVE) {
            Serial.println("Primitive");
            // If the key contains a primitive
            /* We may want to do strtol() here to get numeric value */
            // Copying the key name
            feed->keys[k] = new char[tokens[i].end - tokens[i].start];
            memcpy(feed->keys[k++], (char*) packet + tokens[i].start, tokens[i].end - tokens[i].start);
            feed->keys[k++][tokens[i].end - tokens[i].start] = '\0';
            Serial.printf("Key is: %s\n", feed->keys[k - 1]);
            Serial.printf("- %.*s: %.*s\n", tokens[i].end - tokens[i].start,
                (char*) packet + tokens[i].start, tokens[i + 1].end - tokens[i + 1].start,
                (char*) packet + tokens[i + 1].start);
            i++;
        } else if (tokens[i + 1].type == JSMN_ARRAY) {
            Serial.println("Array");
            // If the key contains an array
            // Copying the key name
            feed->keys[k] = new char[tokens[i].end - tokens[i].start];
            memcpy(feed->keys[k++], (char*) packet + tokens[i].start, tokens[i].end - tokens[i].start);
            feed->keys[k++][tokens[i].end - tokens[i].start] = '\0';
            Serial.printf("Key is: %s\n", feed->keys[k - 1]);
            for (int j = 0; j < tokens[i + 1].size; j++) {
                Serial.printf("  * %.*s\n", tokens[i + j + 2].end - tokens[i + j + 2].start,
                    (char*) packet + tokens[i + j + 2].start);
            }
            i += tokens[i + 1].size + 1;
        } else {
            return -1;
        }
    }
    for(; k < feed->numberOfKeys; k++) {
        feed->keys[k] = '\0';
        Serial.printf("Key is: %s\n", feed->keys[k]);
    }
    return 0;
}