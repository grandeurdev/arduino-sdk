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
class Callback {
	private:
    // Create a private variable
		void (*_cJSON)(JSONObject);
		void (*_cBool)(bool);
		void (*_cInt)(int);
		void (*_cLong)(long);
		void (*_cDouble)(double);
		void (*_cStr)(const char*);

	public:
    // Default constructor will init the pointer
    Callback() {
      // We will init the context
      _cJSON = NULL;
      _cBool = NULL;
      _cInt = NULL;
      _cLong = NULL;
      _cDouble = NULL;
      _cStr = NULL;
    }

    // Constructor to handle null
    Callback(int c) {
      // We will init the context
      _cJSON = NULL;
      _cBool = NULL;
      _cInt = NULL;
      _cLong = NULL;
      _cDouble = NULL;
      _cStr = NULL;
    }
    
		// For json callback
		Callback(void (*c)(JSONObject)) {
      // We will store it in context
      _cJSON = c;
    }

    // For bool type callback
		Callback(void (*c)(bool)) {
      // We will store it in context
      _cBool = c;
    }

    // For int callback
		Callback(void (*c)(int)) {
      // We will store it in context
      _cInt = c;
    }

    // For long datatype
		Callback(void (*c)(long)) {
      // We will store it in context
      _cLong = c;
    }


    // For double datatype
		Callback(void (*c)(double)) {
      // We will store it in context
      _cDouble = c;
    }

    // For handling strings
		Callback(void (*c)(const char*)) {
      // We will store it in context
      _cStr = c;
    }

		// Then we will override the function call
    // operator to pass data to callback that we stored
    // in the context
		void operator()(JSONObject data) {
      // Transfer data to json callback
      _cJSON(data);
    }

    void operator()(bool data) {
      // Transfer it to bool callback
      _cBool(data);
    }

    void operator()(int data) {
      // Transfer data to int callback
      _cInt(data);
    }

    void operator()(long data) {
      // Transfer data to long callback
      _cLong(data);
    }

    void operator()(double data) {
      // Transfer data to double callback
      _cDouble(data);
    }

    void operator()(const char* data) {
      // Transfer data to string callback
      _cStr(data);
    }

    // Override bool operator not
    bool operator!() {
      // Return true if none of the callbacks are set
      if (!_cJSON && !_cBool && !_cInt && !_cLong && !_cDouble && !_cStr) return true;
      else return false; 
    }
};

// Define send function
typedef void (*Send)(const char* task, const char* payload, Callback callback);

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
    Callback callback;

    // Constructor
    SendData(
      const char* task,
      const char* payload,
      Callback callback
    );
};

#endif