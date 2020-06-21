/**
 * @file Datastore.cpp
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur Cloud.
 *
 */

// Including headers
#include "Datastore.h"

Datastore::Datastore(String deviceID, DuplexHandler duplexHandler) {
  _duplex = duplexHandler;
  _deviceID = deviceID;
}

Datastore::Datastore() {}

Collection Datastore::collection(String name) {
  return Collection(name, _duplex);
}

Collection::Collection(String name, DuplexHandler duplexHandler) {
  _name = name;
  _duplex = duplexHandler;
}

void Collection::insert(JSONObject documents, Callback inserted) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];
  jsonObject["collection"] = _name;
  jsonObject["documents"] = documents;
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  _duplex.send("insertDocumentsDatastore", jsonString, inserted);
}
