/**
 * @file Device.h
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "Grandeur.h"

Grandeur::Project::Device::Device() {}

Grandeur::Project::Device::Device(DuplexHandler* duplexHandler, String deviceId)
: _duplex(duplexHandler), _deviceId(deviceId) {}

Grandeur::Project::Device::Data Grandeur::Project::Device::data() {
  // Returning reference device's data.
  return Data(_duplex, _deviceId);
}