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

// Callback function
typedef void (*Receiver)(unsigned char*);
// Callback function
typedef void (*Callback)(Payload*);

#endif APOLLOTYPES_H_