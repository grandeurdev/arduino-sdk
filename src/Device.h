/**
 * @file Device.h
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Including headers
#include "grandeurtypes.h"
#include "grandeurmacros.h"
#include "DuplexHandler.h"
#include "Data.h"

#ifndef DEVICE_H_
#define DEVICE_H_

class Device {
  // Class for handling device related functions
  private:
    // Store duplex and device id
    DuplexHandler _duplex;
    String _deviceID;

  public:
    // Device constructor
    Device(String deviceID, DuplexHandler duplexHandler);
    Device();

    // Instantiator methods - return the objects of their classes
    Data data();
};

#endif