/**
 * @file grandeurtypes.h
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "grandeurmacros.h"
#include <cstring>
#include "Arduino_JSON/Arduino_JSON.h"

#ifndef GRANDEURTYPES_H_
#define GRANDEURTYPES_H_

// Include json
typedef JSONVar Var;

// Define Callback using template
class Callback {
	private:
    // Create a private variable
		void * _c;

    // To store type of the callback
    int _type = 0;

    // Type to string
    std::string mapType(int type) {
      // Match type and return string
      switch(type) {
        // Handle cases
        case 0: return "null";
        case 1: return "object<res>";
        case 2: return "object";
        case 3: return "boolean";
        case 4: return "int";
        case 5: return "long";
        case 6: return "double";
        case 7: return "string";
      }
    }

    // Function to generate an error
    void error(int type) {
      // Throw error
      Serial.printf("[CASTING-ERROR] Was expecting %s and received %s\n", mapType(_type).c_str(), mapType(type).c_str());
    }

	public:
    // Default constructor will init the pointer
    Callback() {
      // We will init the context
      _c = NULL;

      // Set type to null
      _type = 0;
    }

    Callback(int ptr) {
      // We will init the context
      _c = NULL;

      // Set type to null
      _type = 0;
    }
    
		// For json callback
		Callback(void (*c)(Var)) {
      // We will store it in context
      _c = (void *) c;

      // Set type to object
      _type = 1;
    }

    Callback(void (*c)(Var, const char*)) {
      // We will store it in context
      _c = (void *) c;

      // Set type to object & str
      _type = 2;
    }

    Callback(void (*c)(bool, const char*)) {
      // We will store it in context
      _c = (void *) c;

      // Set type to bool & str
      _type = 3;
    }

    Callback(void (*c)(int, const char*)) {
      // We will store it in context
      _c = (void *) c;

      // Set type to int & str
      _type = 4;
    }

    Callback(void (*c)(long, const char*)) {
      // We will store it in context
      _c = (void *) c;

      // Set type to long & str
      _type = 5;
    }

    Callback(void (*c)(double, const char*)) {
      // We will store it in context
      _c = (void *) c;

      // Set type to double & str
      _type = 6;
    }

    Callback(void (*c)(const char*, const char*)) {
      // We will store it in context
      _c = (void *) c;

      // Set type to str & str
      _type = 7;
    }

    // Function to return type of callback
    std::string type() {
      // Map type and return
      return mapType(_type);
    }

		// Then we will override the function call
    // operator to pass data to callback that we stored
    // in the context
		void operator()(Var data) {
      // Validate type
      if (_type == 1)
        // Cast as var function pointer only when function was accepting var
        ((void (*)(Var)) _c)(data);

      else
        // Invalid type
        error(1);
    }

    // Overload for with string
    void operator()(Var data, const char* path) {
      // Validate type
      if (_type == 2)
        // Cast as var function pointer only when function was accepting var
        ((void (*)(Var, const char*)) _c)(data, path);  
      
      else
        // Invalid type
        error(2);
    }

    void operator()(bool data, const char* path) {
      // Validate type
      if (_type == 3)
        // Cast as bool function pointer only when function was accepting bool
        ((void (*)(bool, const char*)) _c)(data, path);
          
      else
        // Invalid type
        error(3);
    }

    void operator()(int data, const char* path) {
      // Validate type
      if (_type == 4)
        // Cast as int function pointer only when function was accepting int
        ((void (*)(int, const char*)) _c)(data, path);
          
      else
        // Invalid type
        error(4);
    }

    void operator()(long data, const char* path) {
      // Validate type
      if (_type == 5)
        // Cast as long function pointer only when function was accepting long
        ((void (*)(long, const char*)) _c)(data, path);
        
      else
        // Invalid type
        error(5);
    }

    void operator()(double data, const char* path) {
      // Validate type
      if (_type == 6)
        // Cast as double function pointer only when function was accepting double
        ((void (*)(double, const char*)) _c)(data, path);

      else
        // Invalid type
        error(6);
    }

    void operator()(const char* data, const char* path) {
      // Validate type
      if (_type == 7)
        // Cast as string function pointer only when function was accepting string
        ((void (*)(const char*, const char*)) _c)(data, path);
          
      else
        // Invalid type
        error(7);
    }

    // Override bool operator not
    bool operator!() {
      // Return true if the callback is not set
      if (!_c && _type == 0) return true;
      else return false; 
    }
};

// Define Grandeur ID
typedef long gID;

// EventID
typedef gID EventID;

//EventData
typedef Callback EventData;

// Event key
typedef std::string EventKey;

// Event payload
typedef std::string EventPayload;

// Event key
typedef std::string EventKey;

// Config class for storing configurations of connection
class Config {
  public:
    String apiKey;
    String token;

    // Constructor
    Config(String apiKey, String token);
    Config();
};
#endif