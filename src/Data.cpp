/**
 * @file Data.h
 * @date 23.01.2021
 * @author Grandeur Technologies
 *
 * Copyright (c) 2021 Grandeur Technologies Inc. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "Grandeur.h"

Grandeur::Project::Device::Event::Event() {}

Grandeur::Project::Device::Event::Event(
  String deviceId,
  DuplexHandler* duplexHandler,
  gId id,
  String path
) : _deviceId(deviceId), _duplex(duplexHandler), _id(id), _path(path) {}

void Grandeur::Project::Device::Event::clear() {
  // Clear an event handler on path
  // Create a new json object
  Var jsonObject;
  jsonObject["deviceID"] = _deviceId;
  jsonObject["path"] = _path;
  jsonObject["event"] = "data";

  // Creating a string.
  char jsonString[MESSAGE_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, MESSAGE_SIZE);

  // Unsubscribing from the event.
  _duplex->unsubscribe(_id, jsonString);
}

Grandeur::Project::Device::Data::Data() {}

Grandeur::Project::Device::Data::Data(DuplexHandler* duplexHandler, String deviceId)
: _duplex(duplexHandler), _deviceId(deviceId) {}

void Grandeur::Project::Device::Data::get(const char* path, Callback cb) {
  // Creating a new json object to store device ID and variable path.
  Var jsonObject;
  jsonObject["deviceID"] = _deviceId;
  jsonObject["path"] = path;

  // Creating a string.
  char jsonString[MESSAGE_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, MESSAGE_SIZE);

  // Sending the packet and scheduling callback.
  _duplex->send("/device/data/get", jsonString, cb);
}

void Grandeur::Project::Device::Data::get(Callback cb) {
  // Creating a new json object to store device ID.
  Var jsonObject;
  jsonObject["deviceID"] = _deviceId;

  // Creating a string.
  char jsonString[MESSAGE_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, MESSAGE_SIZE);

  // Sending the packet and scheduling callback.
  _duplex->send("/device/data/get", jsonString, cb);
}

void Grandeur::Project::Device::Data::set(const char* path, Var data, Callback cb) {
  // Creating a new json object to store device ID, variable path, and data to update the variable to.
  Var jsonObject;
  jsonObject["deviceID"] = _deviceId;
  jsonObject["path"] = path;
  jsonObject["data"] = data;

  // Creating a string.
  char jsonString[MESSAGE_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, MESSAGE_SIZE);

  // Sending the packet and scheduling callback.
  _duplex->send("/device/data/set", jsonString, cb);
}

void Grandeur::Project::Device::Data::set(const char* path, Var data) {
  // Creating a new json object to store device ID, variable path, and data to update the variable to.
  Var jsonObject;
  jsonObject["deviceID"] = _deviceId;
  jsonObject["path"] = path;
  jsonObject["data"] = data;

  // Creating a string.
  char jsonString[MESSAGE_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, MESSAGE_SIZE);

  // Sending the packet and scheduling callback.
  _duplex->send("/device/data/set", jsonString, NULL);
}

Grandeur::Project::Device::Event Grandeur::Project::Device::Data::on(const char* path, Callback cb) {
  // Creating a new json object to store device ID and variable path.
  Var jsonObject;
  jsonObject["deviceID"] = _deviceId;
  jsonObject["event"] = "data";
  jsonObject["path"] = path;

  // Creating a string.
  char jsonString[MESSAGE_SIZE];
  // Parsing json object to json string.
  JSON.stringify(jsonObject).toCharArray(jsonString, MESSAGE_SIZE);
  // Send 
  gId eventId = _duplex->subscribe(("data/" + String(path)).c_str(), jsonString, cb);

  // Return the event object to let the user unsubscribe to this event at a later time.
  return Event(_deviceId, _duplex, eventId, path);
}