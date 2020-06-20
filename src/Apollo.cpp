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

Project Apollo::init(String deviceID, String apiKey, String token) {
  // Setting config
  _config = {deviceID, apiKey, token};
  Project apolloProject;
  apolloProject._deviceID = _config.deviceID;
  apolloProject._duplexHandler = DuplexHandler(_config);
  // Starting Duplex
  apolloProject._duplexHandler.init();
  return apolloProject;
}

Project::Project() {}

void Project::onConnection(void connectionCallback(bool)) {
  _duplexHandler.onConnectionEvent(connectionCallback);
}

bool Project::isConnected(void) {
  return _duplexHandler.getStatus() == CONNECTED;
}

Device Project::device(void) {
  return Device(_deviceID, _duplexHandler);
}

Datastore Project::datastore(void) {
  return Datastore(_deviceID, _duplexHandler);
}

void Project::loop(bool valve) {
  // Running duplex loop
  _duplexHandler.loop(valve);
}