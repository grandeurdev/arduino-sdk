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
typedef JSONVar JSONObject;

// Define Callback using template
template <class T>
class Callback {
	private:
    // Create a private variable
		void (*_c)(T);

	public:
    // Default constructor will init the pointer
    Callback() {
      // We will init the context
      _c = NULL;
    }
    
		// It will receive an function
		Callback(void (*c)(T)) {
      // We will store it in context
      _c = c;
    }

		// Then we will override the function call
    // operator to pass data to callback that we stored
    // in the context
		void operator()(T data) {
      // Call the function stored in context
      _c(data);
    }

    // Override bool operator not
    bool operator!() {
      // Return true if callback was set
      if (!_c) return true;
      else return false; 
    }
};

// Define send function
typedef void (*Send)(const char* task, const char* payload, Callback<JSONObject> callback);

// Define Grandeur ID
typedef long GrandeurID;

// Config class for storing configurations of connection
class Config {
  public:
    String apiKey;
    String token;

    // Constructor
    Config(String apiKey, String token);
    Config();
};

// Class for Send Queue Data
class SendData {
  public:
    char task[TASK_SIZE];
    char payload[PACKET_SIZE];
    Callback<JSONObject> callback;

    // Constructor
    SendData(
      const char* task,
      const char* payload,
      Callback<JSONObject> callback
    );
};

#endif