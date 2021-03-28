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
  DuplexHandler* duplexHandler,
  String deviceId,
  String event,
  String path,
  gId id
) : _duplex(duplexHandler), _deviceId(deviceId), _event(event), _path(path), _id(id) {}

void Grandeur::Project::Device::Event::clear() {
  // Clear an event handler on path
  // Prepare the message payload.
  Var oPayload;
  oPayload["deviceID"] = _deviceId;
  oPayload["event"] = _event;
  oPayload["path"] = _path;

  // Unsubscribing from the event.
  _duplex->unsubscribe((_event + "/" + _path).c_str(), _id, oPayload);
}

Grandeur::Project::Device::Data::Data() {}

Grandeur::Project::Device::Data::Data(DuplexHandler* duplexHandler, String deviceId)
: _duplex(duplexHandler), _deviceId(deviceId) {}

void Grandeur::Project::Device::Data::get(const char* path, Callback cb) {
  // Prepare the message payload.
  Var oPayload;
  oPayload["deviceID"] = _deviceId;
  oPayload["path"] = path;

  // Sending the packet and scheduling callback.
  _duplex->send("/device/data/get", oPayload, cb);
}

void Grandeur::Project::Device::Data::get(Callback cb) {
  // Prepare the message payload.
  Var oPayload;
  oPayload["deviceID"] = _deviceId;

  // Sending the packet and scheduling callback.
  _duplex->send("/device/data/get", oPayload, cb);
}

void Grandeur::Project::Device::Data::set(const char* path, Var data, Callback cb) {
  // Prepare the message payload.
  Var oPayload;
  oPayload["deviceID"] = _deviceId;
  oPayload["path"] = path;
  oPayload["data"] = data;

  // Sending the packet and scheduling callback.
  _duplex->send("/device/data/set", oPayload, cb);
}

void Grandeur::Project::Device::Data::set(const char* path, Var data) {
  // Prepare the message payload.
  Var oPayload;
  oPayload["deviceID"] = _deviceId;
  oPayload["path"] = path;
  oPayload["data"] = data;

  // Sending the packet without response.
  Serial.println(oPayload);
  _duplex->send("/device/data/set", oPayload);
}

Grandeur::Project::Device::Event Grandeur::Project::Device::Data::on(const char* path, Callback cb) {
  // Prepare the message payload.
  Var oPayload;
  oPayload["deviceID"] = _deviceId;
  oPayload["event"] = "data";
  oPayload["path"] = path;
  
  // Send 
  gId eventId = _duplex->subscribe(("data/" + String(path)).c_str(), oPayload, cb);

  // Return the event object to let the user unsubscribe to this event at a later time.
  return Event(_duplex, _deviceId, "data", path, eventId);
}

Grandeur::Project::Device::Event Grandeur::Project::Device::Data::on(Callback cb) {
  // Prepare the message payload.
  Var oPayload;
  oPayload["deviceID"] = _deviceId;
  oPayload["event"] = "data";
  
  // Send 
  gId eventId = _duplex->subscribe("data/", oPayload, cb);

  // Return the event object to let the user unsubscribe to this event at a later time.
  return Event(_duplex, _deviceId, "data", "", eventId);
}