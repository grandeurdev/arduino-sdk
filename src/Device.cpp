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

Device::Device() {
  _deviceID = "";
}

Device::Device(DuplexHandler* duplexHandler) {
  _duplex = duplexHandler;
  _deviceID = "";
}

void Device::init(String deviceID) {
  _deviceID = deviceID;
}

void Device::get(String path, Callback callback) {
  Var jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  jsonObject["path"] = path;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex->send("/device/data/get", jsonString, callback);
}

void Device::get(Callback callback) {
  Var jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex->send("/device/data/get", jsonString, callback);
}

void Device::set(String path, Var data, Callback callback) {
  Var jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  jsonObject["path"] = path;
  jsonObject["data"] = data;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex->send("/device/data/set", jsonString, callback);
}

void Device::set(String path, Var data) {
  Var jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  jsonObject["path"] = path;
  jsonObject["data"] = data;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex->send("/device/data/set", jsonString, [](Var json){});
}

void Device::on(String path, Callback updateHandler) {
  Var jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  jsonObject["event"] = "data";
  jsonObject["path"] = path;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex->subscribe((const char*) jsonObject["event"], path, jsonString, updateHandler);
}

void Device::on(Callback updateHandler) {
  Var jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["deviceID"] = _deviceID;
  jsonObject["event"] = "data";
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex->subscribe((const char*) jsonObject["event"], "", jsonString, updateHandler);
}