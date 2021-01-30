/**
 * @file Device.h
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "Device.h"

// Constructor
Device::Device(String deviceID, DuplexHandler duplexHandler) {
  _duplex = duplexHandler;
  _deviceID = deviceID;
}

Device::Device() {}

// Return reference to data
Data Device::data() {
  // Return new device class object
  return Data(_deviceID, _duplex);
}