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
  // Store reference to duplex into context
  _duplex = duplexHandler;
}

Datastore::Datastore() {
  // Default constructor
}

Collection Datastore::collection(String name) {
  // Return a reference to collection object
  return Collection(name, _duplex);
}

Collection::Collection(String name, DuplexHandler duplexHandler) {
  // Store name of collection and reference to object
  _name = name;
  _duplex = duplexHandler;
}

void Collection::insert(Var documents, Callback inserted) {
  // Insert documents to datastore
  Var jsonObject;

  // Define string
  char jsonString[PACKET_SIZE];
  
  // Append collection name and documents
  jsonObject["collection"] = _name;
  jsonObject["documents"] = documents;
  
  // Convert to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send request to server
  _duplex.send("/datastore/insert", jsonString, inserted);
}

void Collection::remove(Var filter, Callback removed) {
  // Remove documents from datastore
  Var jsonObject;

  // Define string
  char jsonString[PACKET_SIZE];
  
  // Append collection name and filter
  jsonObject["collection"] = _name;
  jsonObject["filter"] = filter;
  
  // Convert to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send request to server
  _duplex.send("/datastore/delete", jsonString, removed);
}

void Collection::update(Var filter, Var update, Callback updated) {
  // Update document from datastore
  Var jsonObject;

  // Define string
  char jsonString[PACKET_SIZE];
  
  // Append collection name, filter and update
  jsonObject["collection"] = _name;
  jsonObject["filter"] = filter;
  
  // Convert to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send request to server
  _duplex.send("/datastore/update", jsonString, updated);
}

void Collection::search(Var filter, Var projection, int nPage, Callback searched) {
  // Basically it will use pipeline
  Pipeline searchPipeline = Pipeline(_name, {}, _duplex).match(filter);

  // Add project stage if provided
  if(projection == undefined);
  else {
    searchPipeline = searchPipeline.project(projection);
  }

  // Execute the pipeline
  return searchPipeline.execute(nPage, searched);
}

Pipeline Collection::pipeline(void) {
  // Return a reference to pipeline
  return Pipeline(_name, undefined, _duplex);
}

Pipeline::Pipeline(String collection, Var query, DuplexHandler duplexHandler) {
  // Setup the context
  _collection = collection;
  _query = query;
  _duplex = duplexHandler;
}

Pipeline Pipeline::match(Var filter) {
  // Add match stage to the pipeline
  int stage = _query.length() + 1;

  // Add type and filter
  _query[stage]["type"] = "match";
  _query[stage]["filter"] = filter;

  // Return reference to pipeline to basically help in chaining
  return Pipeline(_collection, _query, _duplex);
}

Pipeline Pipeline::project(Var specs) {
  // Add project stage to the pipeline
  int stage = _query.length() + 1;

  // Add type and specs
  _query[stage]["type"] = "match";
  _query[stage]["specs"] = specs;

  // Return reference to pipeline to basically help in chaining
  return Pipeline(_collection, _query, _duplex);
}

Pipeline Pipeline::group(Var condition, Var fields) {
  // Add group stage to the pipeline
  int stage = _query.length() + 1;

  // Add type, condition and fields
  _query[stage]["type"] = "match";
  _query[stage]["condition"] = condition;
  _query[stage]["fields"] = fields;

  // Return reference to pipeline to basically help in chaining
  return Pipeline(_collection, _query, _duplex);
}

Pipeline Pipeline::sort(Var specs) {
  // Add sort stage to the pipeline
  int stage = _query.length() + 1;

  // Add type and specs
  _query[stage]["type"] = "match";
  _query[stage]["specs"] = specs;

  // Return reference to pipeline to basically help in chaining
  return Pipeline(_collection, _query, _duplex);
}

void Pipeline::execute(int nPage, Callback executed) {
  // Define an object
  Var jsonObject;

  // Define string
  char jsonString[PACKET_SIZE];

  // Formulate query
  jsonObject["collection"] = _collection;
  jsonObject["pipeline"] = _query;
  jsonObject["nPage"] = nPage;

  // Convert query to string
  JSON.stringify(jsonObject).toCharArray(jsonString, PACKET_SIZE);

  // Send to server
  _duplex.send("/datastore/pipeline", jsonString, executed);
}
