/**
 * @file Grandeur.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "Grandeur.h"
#include "DuplexHandler.h"

Grandeur grandeur;

Grandeur::Grandeur() {}

Project::Project() {
  Project::_device = Device(&_duplexHandler);
  Project::_datastore = Datastore(_duplexHandler);
}

Project& Grandeur::init(String apiKey, String token) {
  // Setting config
  _config = {apiKey, token};
  // Starting Duplex.
  _project._duplexHandler.init(_config);
  // Returning project reference
  return _project;
}

Project& Project::onConnection(void connectionCallback(bool)) {
  _duplexHandler.onConnectionEvent(connectionCallback);
  return *this;
}

void Project::clearConnectionCallback(void) {
  return _duplexHandler.clearConnectionCallback();
}

bool Project::isConnected(void) {
  return (_duplexHandler.getStatus() == CONNECTED);
}

Device& Project::device(String deviceID) {
  _device.init(deviceID);
  return _device;
}

Datastore& Project::datastore(void) {
  return _datastore;
}

void Project::loop(bool valve) {
  // Running duplex loop
  _duplexHandler.loop(valve);
}