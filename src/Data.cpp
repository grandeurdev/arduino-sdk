/**
 * @file Device.h
 * @date 23.01.2021
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "Data.h"

// Constructors
Data::Data(String deviceID, DuplexHandler duplexHandler) {
  _duplex = duplexHandler;
  _deviceID = deviceID;
}

Data::Data() {}

void Data::get(const char* path, Callback<JSONObject> callback) {
  // Get data from server
  // Create a new json object
  JSONObject jsonObject;

  // Create packet
  char jsonString[PACKET_SIZE];

  // Add device id
  jsonObject["deviceID"] = _deviceID;

  // Add path 
  jsonObject["path"] = path;

  // Conver the object to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send 
  _duplex.send("/device/data/get", jsonString, callback);
}

void Data::set(const char* path, JSONObject data, Callback<JSONObject> callback) {
  // Set data to server
  // Create a new json object
  JSONObject jsonObject;

  // Create packet
  char jsonString[PACKET_SIZE];

  // Add device id
  jsonObject["deviceID"] = _deviceID;

  // Add path 
  jsonObject["path"] = path;

  // Add data
  jsonObject["data"] = data;

  // Conver the object to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send 
  _duplex.send("/device/data/set", jsonString, callback);
}

void Data::set(const char* path, bool data, Callback<JSONObject> callback) {
  // Set data to server
  // Create a new json object
  JSONObject jsonObject;

  // Create packet
  char jsonString[PACKET_SIZE];

  // Add device id
  jsonObject["deviceID"] = _deviceID;

  // Add path 
  jsonObject["path"] = path;

  // Add data
  jsonObject["data"] = data;

  // Conver the object to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send 
  _duplex.send("/device/data/set", jsonString, callback);
}


void Data::set(const char* path, int data, Callback<JSONObject> callback) {
  // Set data to server
  // Create a new json object
  JSONObject jsonObject;

  // Create packet
  char jsonString[PACKET_SIZE];

  // Add device id
  jsonObject["deviceID"] = _deviceID;

  // Add path 
  jsonObject["path"] = path;

  // Add data
  jsonObject["data"] = data;

  // Conver the object to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send 
  _duplex.send("/device/data/set", jsonString, callback);
}

void Data::set(const char* path, long data, Callback<JSONObject> callback) {
  // Set data to server
  // Create a new json object
  JSONObject jsonObject;

  // Create packet
  char jsonString[PACKET_SIZE];

  // Add device id
  jsonObject["deviceID"] = _deviceID;

  // Add path 
  jsonObject["path"] = path;

  // Add data
  jsonObject["data"] = data;

  // Conver the object to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send 
  _duplex.send("/device/data/set", jsonString, callback);
}

void Data::set(const char* path, double data, Callback<JSONObject> callback) {
  // Set data to server
  // Create a new json object
  JSONObject jsonObject;

  // Create packet
  char jsonString[PACKET_SIZE];

  // Add device id
  jsonObject["deviceID"] = _deviceID;

  // Add path 
  jsonObject["path"] = path;

  // Add data
  jsonObject["data"] = data;

  // Conver the object to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send 
  _duplex.send("/device/data/set", jsonString, callback);
}

void Data::set(const char* path, const char* data, Callback<JSONObject> callback) {
  // Set data to server
  // Create a new json object
  JSONObject jsonObject;

  // Create packet
  char jsonString[PACKET_SIZE];

  // Add device id
  jsonObject["deviceID"] = _deviceID;

  // Add path 
  jsonObject["path"] = path;

  // Add data
  jsonObject["data"] = data;

  // Conver the object to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send 
  _duplex.send("/device/data/set", jsonString, callback);
}


void Data::on(const char* path, Callback<JSONObject> callback) {
  // Place an event handler on path update
  // Create a new json object
  JSONObject jsonObject;

  // Create packet
  char jsonString[PACKET_SIZE];

  // Add device id
  jsonObject["deviceID"] = _deviceID;

  // Add path 
  jsonObject["path"] = path;

  // Add data
  jsonObject["event"] = "data";

  // Conver the object to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Convert path to string
  std::string p(path);

  // Formulate the event
  std::string event = "data/" + p;

  // Send 
  _duplex.subscribe(event.c_str(), jsonString, callback);
}