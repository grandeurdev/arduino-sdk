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
Event::Event(String deviceID, DuplexHandler duplexHandler, gID id, String path) {
  _duplex = duplexHandler;
  _deviceID = deviceID;
  _id = id;
  _path = path;
}

Event::Event() {}

void Event::clear() {
  // Clear an event handler on path
  // Create a new json object
  Var jsonObject;

  // Create packet
  char jsonString[PACKET_SIZE];

  // Add device id
  jsonObject["deviceID"] = _deviceID;

  // Add path 
  jsonObject["path"] = _path;

  // Add data
  jsonObject["event"] = "data";

  // Conver the object to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send 
  _duplex.unsubscribe(_id, jsonString);
}

// Constructors
Data::Data(String deviceID, DuplexHandler duplexHandler) {
  _duplex = duplexHandler;
  _deviceID = deviceID;
}

Data::Data() {}

void Data::get(const char* path, Callback callback) {
  // Get data from server
  // Create a new json object
  Var jsonObject;

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

void Data::set(const char* path, Var data, Callback callback) {
  // Set data to server
  // Create a new json object
  Var jsonObject;

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

Event Data::on(const char* path, Callback callback) {
  // Place an event handler on path update
  // Create a new json object
  Var jsonObject;

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
  gID packetID = _duplex.subscribe(event.c_str(), jsonString, callback);

  // Return an event
  return Event(_deviceID, _duplex, packetID, path);
}