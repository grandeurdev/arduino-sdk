/**
 * @file Data.cpp
 * @date 09.02.2021
 * @author Grandeur Technologies
 *
 * Copyright (c) 2020 Grandeur Technologies Inc. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "Data.h"

Event::Event() {}

Event::Event(String deviceID, DuplexHandler* duplexHandler, gID id, String path) {
  _deviceID = deviceID;
  _duplex = duplexHandler;
  _id = id;
  _path = path;
}

void Event::clear() {
  // Clear an event handler on path
  // Create a new json object
  Var jsonObject;
  jsonObject["deviceID"] = _deviceID;
  jsonObject["path"] = _path;
  jsonObject["event"] = "data";

  // Creating a string.
  char jsonString[PACKET_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Unsubscribing from the event.
  _duplex.unsubscribe(_id, jsonString);
}

Data::Data() {}

Data::Data(String deviceID, DuplexHandler* duplexHandler) {
  _deviceID = deviceID;
  _duplex = duplexHandler;
}

void Data::get(const char* path, Callback cb) {
  // Creating a new json object to store device ID and variable path.
  Var jsonObject;
  jsonObject["deviceID"] = _deviceID;
  jsonObject["path"] = path;

  // Creating a string.
  char jsonString[PACKET_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Sending the packet and scheduling callback.
  _duplex.send("/device/data/get", jsonString, cb);
}

void Data::get(Callback cb) {
  // Creating a new json object to store device ID.
  Var jsonObject;
  jsonObject["deviceID"] = _deviceID;

  // Creating a string.
  char jsonString[PACKET_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Sending the packet and scheduling callback.
  _duplex.send("/device/data/get", jsonString, cb);
}

void Data::set(const char* path, Var data, Callback cb) {
  // Creating a new json object to store device ID, variable path, and data to update the variable to.
  Var jsonObject;
  jsonObject["deviceID"] = _deviceID;
  jsonObject["path"] = path;
  jsonObject["data"] = data;

  // Creating a string.
  char jsonString[PACKET_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Sending the packet and scheduling callback.
  _duplex.send("/device/data/set", jsonString, cb);
}

void Data::set(const char* path, Var data) {
  // Creating a new json object to store device ID, variable path, and data to update the variable to.
  Var jsonObject;
  jsonObject["deviceID"] = _deviceID;
  jsonObject["path"] = path;
  jsonObject["data"] = data;

  // Creating a string.
  char jsonString[PACKET_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Sending the packet and scheduling callback.
  _duplex.send("/device/data/set", jsonString, NULL);
}

Event Data::on(const char* path, Callback cb) {
  // Creating a new json object to store device ID and variable path.
  Var jsonObject;
  jsonObject["deviceID"] = _deviceID;
  jsonObject["path"] = path;
  jsonObject["event"] = "data";

  // Creating a string.
  char jsonString[PACKET_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send 
  _duplex.subscribe("data", path, jsonString, callback);

  // Return the event object to let the user unsubscribe to this event at a later time.
  return Event(_deviceID, _duplex, packetID, path);
}