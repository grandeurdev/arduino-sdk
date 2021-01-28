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
		void (*_c)(Var);
    void (*_cWStr)(Var, const char*);

	public:
    // Default constructor will init the pointer
    Callback() {
      // We will init the context
      _c = NULL;
      _cWStr = NULL;
    }

    Callback(int ptr) {
      // We will init the context
      _c = NULL;
      _cWStr = NULL;
    }
    
		// For json callback
		Callback(void (*c)(Var)) {
      // We will store it in context
      _c = c;
    }

    Callback(void (*c)(Var, const char*)) {
      // We will store it in context
      _cWStr = c;
    }

		// Then we will override the function call
    // operator to pass data to callback that we stored
    // in the context
		void operator()(Var data) {
      // Transfer data callback
      _c(data);
    }

    // Overload for with string
    void operator()(Var data, const char* path) {
      // Transfer data callback with string
      _cWStr(data, path);
    }

    // Override bool operator not
    bool operator!() {
      // Return true if the callback is not set
      if (!_c && !_cWStr) return true;
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