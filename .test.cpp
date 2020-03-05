#include <iostream>
#include <cstring>

class Payload {
    public:
        uint_fast16_t numberOfKeys;
        char** keys;
        char** values;
        Payload(uint_fast16_t numberOfKeys, char** keys, char** values);
        ~Payload();
};

Payload::Payload(uint_fast16_t numberOfKeys, char** keys, char** values) {
    this->numberOfKeys = numberOfKeys;
    this->keys = keys;
    this->values = values;
}

Payload::~Payload() {
    for (int i = 0; i < this->numberOfKeys; i++) {
      if (this->values[i] != NULL)
         delete this->values[i];
         delete[] this->values;
   }
}

int getValue(uint8_t* payload, char* key, char* value) {
    if(*key == '\0') {
        // If key is empty
        value[0] = '\0';
        return 0;
    }
    // Converting uint8_t to char*
    char* data = (char*) payload;
    
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

int getValues(uint8_t* payload, char** keys, uint_fast16_t numberOfKeys, char** values, uint_fast16_t maxSizeOfValue = 10) {
    
    for(int i = 0; i < numberOfKeys; i++) {
        char* value = new char[maxSizeOfValue];
        if(getValue(payload, keys[i], value) == 0) {
            values[i] = new char[maxSizeOfValue];
            memcpy(values[i], value, sizeof(char) * maxSizeOfValue);
        }
        else {
            return -1;
        }
    }
    return 0;
}

int getPayload(uint8_t* packet, Payload* payload, uint_fast16_t maxSizeOfValue = 10) {
    if(getValues(packet, payload->keys, payload->numberOfKeys, payload->values, maxSizeOfValue) == 0) {
        return 0;
    }
    else {
        return -1;
    }
}


int getKeys(uint8_t* packet, char** keys, size_t numberOfKeys, size_t sizeOfKey) {
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

int main(void) {
    uint8_t* packet = (uint8_t*) "{\"headers\": {\"task\":\"getParms\", \"id\": \"1529644667834\"}, \"payload\": {\"code\": \"SUMMARY-FETCHED\", \"message\": \"Summary is fetched.\", \"summary\": {\"voltage\": 220, \"current\": 10}, \"parms\": {\"state\": {\"hello\": 1}}}}";
    //uint8_t* packet = (uint8_t*) "{\"headers\": {\"task\":\"getParms\", \"id\": \"1529644667834\"}}";
    char* keys[6];
    if(getKeys(packet, keys, 6, 20) == 0) {
        for(int i = 0; i < 6; i++) {
            std::cout<<"Key is:_"<<keys[i]<<"_\n";
        }
    }
    else {
        std::cout<<"Error.";
    }
    
    char* values[10];
    Payload *payload = new Payload(6, keys, values);
    
    if(getPayload(packet, payload, 25) == 0) {
        for(int i = 0; i < payload->numberOfKeys; i++) {
            std::cout<<payload->keys[i]<<": "<<payload->values[i]<<" \n";
        }
    }
    else {
        std::cout<<"Error.";
    }
    
    //delete[] payload->keys;
    for(int i = 0; i < payload->numberOfKeys; i++) {
        delete[] payload->values[i];
    }
    
    return 0;
}