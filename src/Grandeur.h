/**
 * @file Grandeur.h
 * @date 20.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Including headers
#include "DuplexHandler.h"

#ifndef GRANDEUR_H_
#define GRANDEUR_H_

class Grandeur {
  private:
    Config _config;
    // Duplex handles the realtime connection with the project.
    DuplexHandler _duplexHandler;
  public:
    Grandeur();
    // Class that models everything in a project at device scope.
    class Project;
    // Returns the project and initializes the duplex connection with Grandeur.
    Project init(String apiKey, String token);
};

class Grandeur::Project {
  private:
    // Duplex handles the realtime connection with the project.
    DuplexHandler* _duplex;

  public:
    // Constructor
    Project();
    Project(DuplexHandler* duplex);
    // Class that models a Grandeur device.
    class Device;

    // Connection related methods:
    // Schedules a connection handler function to be called on successful connection establishment
    // with Grandeur.
    void onConnection(void connectionCallback(bool));
    // Removes the connection handler function.
    void clearConnectionCallback(void);
    // Checks if we are connected with Grandeur.
    bool isConnected(void);

    // Instantiator methods — return reference to objects of their classes.
    Device device(String deviceId);

    // This method runs the SDK.
    void loop(bool valve);
    void loop();

    #if DEBUG
    // Defines these when in debug mode.
    DuplexHandler* duplex = _duplex;
    #endif /* DEBUG */
};

class Grandeur::Project::Device {
  private:
    // Stores reference to duplex channel we are connected through to Grandeur.
    DuplexHandler* _duplex;
    // Stores device Id.
    String _deviceId;

  public:
    // Device constructor
    Device();
    Device(DuplexHandler* duplexHandler, String deviceId);

    class Event {
      // Class will help in clearing an event
      private:
        // Stores reference to duplex channel we are connected through to Grandeur.
        DuplexHandler* _duplex;
        String _deviceId;
        String _event;
        String _path;
        gId _id;

      public:
        // Constructor
        Event();
        Event(DuplexHandler* duplexHandler, String deviceId, String event, String path, gId id);

        // Clear method
        void clear();
    };

    // Class that models a device's data.
    class Data {
      private:
        // Stores reference to duplex channel we are connected through to Grandeur.
        DuplexHandler* _duplex;
        // Stores device Id this data belongs to.
        String _deviceId;

      public:
        // Constructor
        Data();
        Data(DuplexHandler* duplexHandler, String deviceId);

        // Async getter/setter methods:
        // Gets the variable specified in path and makes it available in cb function scope.
        void get(const char* path, Callback cb);
        // Gets all variables from Grandeur and makes them available in cb function scope.
        void get(Callback cb);
        // Sets the variable specified in path with what's in the data and schedules cb function for when
        // acknowledgement arrives from Grandeur.
        void set(const char* path, Var data, Callback cb);
        // Sets the variable specified in path with what's in the data without scheduling a function.
        void set(const char* path, Var data);

        // Sets a listener on update of a variable and runs cb function whenever the update occurs.
        Event on(const char* path, Callback cb);
        // Sets a listener on update of any variable and runs cb function whenever the update occurs.
        Event on(Callback cb);
    };

    // Instantiator method — returns reference to object of data class
    Data data();
};

extern Grandeur grandeur;

#endif