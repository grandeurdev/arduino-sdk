#include "types.h"

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