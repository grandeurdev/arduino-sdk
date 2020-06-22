/**
 * @file Apollo.cpp
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

#include "Apollo.h"
#include "DuplexHandler.h"

Apollo apollo;

Apollo::Apollo() {}

Project Apollo::init(String apiKey, String token) {
  // Setting config
  _config = {apiKey, token};
  // Creating a new project reference.
  Project project;
  // Duplex handles the connection to the Cloud project.
  project._duplexHandler = DuplexHandler(_config);
  // Starting Duplex.
  project._duplexHandler.init();
  return project;
}

Project::Project() {}

void Project::onConnection(void connectionCallback(bool)) {
  _duplexHandler.onConnectionEvent(connectionCallback);
}

bool Project::isConnected(void) {
  return _duplexHandler.getStatus() == CONNECTED;
}

Device Project::device(String deviceID) {
  return Device(deviceID, _duplexHandler);
}

Datastore Project::datastore(void) {
  return Datastore(_duplexHandler);
}

void Project::loop(bool valve) {
  // Running duplex loop
  _duplexHandler.loop(valve);
}