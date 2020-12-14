/**
 * @file Datastore.cpp
 * @date 20.06.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Including headers
#include "Datastore.h"

Datastore::Datastore(DuplexHandler duplexHandler) {
  _duplex = duplexHandler;
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

  _duplex.send("/datastore/insert", jsonString, inserted);
}

void Collection::remove(JSONObject filter, Callback removed) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];

  jsonObject["collection"] = _name;
  jsonObject["filter"] = filter;

  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  
  _duplex.send("/datastore/delete", jsonString, removed);
}

void Collection::update(JSONObject filter, JSONObject update, Callback updated) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];

  jsonObject["collection"] = _name;
  jsonObject["filter"] = filter;
  jsonObject["update"] = update;

  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  _duplex.send("/datastore/update", jsonString, updated);
}

void Collection::search(JSONObject filter, JSONObject projection, int pageNumber, Callback searched) {
  Pipeline searchPipeline = Pipeline(_name, {}, _duplex).match(filter);
  if(projection == undefined) {}
  else {
    searchPipeline = searchPipeline.project(projection);
  }

  return searchPipeline.execute(pageNumber, searched);
}

Pipeline Collection::pipeline(void) {
  return Pipeline(_name, undefined, _duplex);
}

Pipeline::Pipeline(String collectionName, JSONObject query, DuplexHandler duplexHandler) {
  _collection = collectionName;
  _query = query;
  _duplex = duplexHandler;
}

Pipeline Pipeline::match(JSONObject filter) {
  int stage = _query.length() + 1;
  _query[stage]["type"] = "match";
  _query[stage]["filter"] = filter;

  return Pipeline(_collection, _query, _duplex);
}

Pipeline Pipeline::project(JSONObject specs) {
  int stage = _query.length() + 1;
  _query[stage]["type"] = "project";
  _query[stage]["specs"] = specs;

  return Pipeline(_collection, _query, _duplex);
}

Pipeline Pipeline::group(JSONObject condition, JSONObject fields) {
  int stage = _query.length() + 1;
  _query[stage]["type"] = "group";
  _query[stage]["condition"] = condition;
  _query[stage]["fields"] = fields;

  return Pipeline(_collection, _query, _duplex);
}

Pipeline Pipeline::sort(JSONObject specs) {
  int stage = _query.length() + 1;
  _query[stage]["type"] = "sort";
  _query[stage]["specs"] = specs;

  return Pipeline(_collection, _query, _duplex);
}

void Pipeline::execute(int pageNumber, Callback executed) {
  JSONObject jsonObject;
  char jsonString[PACKET_SIZE];

  jsonObject["collection"] = _collection;
  jsonObject["pipeline"] = _query;
  jsonObject["pageNumber"] = pageNumber;

  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  _duplex.send("/datastore/pipeline", jsonString, executed);
}
