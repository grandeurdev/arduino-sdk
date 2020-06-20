/**
 * @file Device.h
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

#include "Device.h"

Device::Device(String deviceID, DuplexHandler duplexHandler) {
  _duplex = duplexHandler;
  _deviceID = deviceID;
}

Device::Device() {}

void Device::getParms(Callback callback) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex.send("getDeviceParms", jsonString, callback);
}

void Device::setSummary(JSONObject summary, Callback callback) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  jsonObject["summary"] = summary;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex.send("setDeviceSummary", jsonString, callback);
}

void Device::setParms(JSONObject parms, Callback callback) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  jsonObject["parms"] = parms;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex.send("setDeviceParms", jsonString, callback);
}

void Device::onSummaryUpdated(Callback updateHandler) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  jsonObject["event"] = "setDeviceSummary";
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex.subscribe(SUMMARYUPDATE, jsonString, updateHandler);
}

void Device::onParmsUpdated(Callback updateHandler) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  jsonObject["event"] = "setDeviceParms";
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex.subscribe(PARMSUPDATE, jsonString, updateHandler);
}