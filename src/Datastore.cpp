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

Project::Datastore::Datastore(DuplexHandler duplexHandler) {
  _duplex = duplexHandler;
}

Datastore::Datastore() {}

Collection Datastore::collection(String name) {
  return Collection(name, _duplex);
}

Project::Datastore::Collection::Collection(String name, DuplexHandler duplexHandler) {
  _name = name;
  _duplex = duplexHandler;
}

void Project::Datastore::Collection::insert(Var documents, Callback inserted) {
  Var jsonObject;
  char jsonString[PACKET_SIZE];
  
  jsonObject["collection"] = _name;
  jsonObject["documents"] = documents;
  
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  _duplex.send("/datastore/insert", jsonString, inserted);
}

void Project::Datastore::Collection::remove(Var filter, Callback removed) {
  Var jsonObject;
  char jsonString[PACKET_SIZE];

  jsonObject["collection"] = _name;
  jsonObject["filter"] = filter;

  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);
  
  _duplex.send("/datastore/delete", jsonString, removed);
}

void Project::Datastore::Collection::update(Var filter, Var update, Callback updated) {
  Var jsonObject;
  char jsonString[PACKET_SIZE];

  jsonObject["collection"] = _name;
  jsonObject["filter"] = filter;
  jsonObject["update"] = update;

  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  _duplex.send("/datastore/update", jsonString, updated);
}

void Project::Datastore::Collection::search(Var filter, Var projection, int pageNumber, Callback searched) {
  Pipeline searchPipeline = Pipeline(_name, {}, _duplex).match(filter);
  if(projection == undefined) {}
  else {
    searchPipeline = searchPipeline.project(projection);
  }

  return searchPipeline.execute(pageNumber, searched);
}

Project::Datastore::Collection::Pipeline Project::Datastore::Collection::pipeline(void) {
  return Pipeline(_name, undefined, _duplex);
}

Project::Datastore::Collection::Pipeline::Pipeline(String collectionName, Var query, DuplexHandler duplexHandler) {
  _collection = collectionName;
  _query = query;
  _duplex = duplexHandler;
}

Project::Datastore::Collection::Pipeline Project::Datastore::Collection::Pipeline::match(Var filter) {
  int stage = _query.length() + 1;
  _query[stage]["type"] = "match";
  _query[stage]["filter"] = filter;

  return Pipeline(_collection, _query, _duplex);
}

Pipeline Pipeline::project(Var specs) {
  int stage = _query.length() + 1;
  _query[stage]["type"] = "project";
  _query[stage]["specs"] = specs;

  return Pipeline(_collection, _query, _duplex);
}

Project::Datastore::Collection::Pipeline Project::Datastore::Collection::Pipeline::group(Var condition, Var fields) {
  int stage = _query.length() + 1;
  _query[stage]["type"] = "group";
  _query[stage]["condition"] = condition;
  _query[stage]["fields"] = fields;

  return Pipeline(_collection, _query, _duplex);
}

Project::Datastore::Collection::Pipeline Project::Datastore::Collection::Pipeline::sort(Var specs) {
  int stage = _query.length() + 1;
  _query[stage]["type"] = "sort";
  _query[stage]["specs"] = specs;

  return Pipeline(_collection, _query, _duplex);
}

void Project::Datastore::Collection::Pipeline::execute(int pageNumber, Callback executed) {
  Var jsonObject;
  char jsonString[PACKET_SIZE];

  jsonObject["collection"] = _collection;
  jsonObject["pipeline"] = _query;
  jsonObject["pageNumber"] = pageNumber;

  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  _duplex.send("/datastore/pipeline", jsonString, executed);
}
