# Apollo Device

`Apollo Device` is the official SDK for hardware that utilizes `Apollo` API to authenticate and connect it to the matrix of [Grandeur Cloud][Grandeur Cloud].

_**NOTE**_: Currently, `Apollo Device` SDK is available only for Arduino IDE.

## Features

## How does it work

## Installation

You can clone `Apollo Device` SDK from [here][Apollo Device SDK].

## Basics of Apollo Device

`Apollo Device` provides a global `apollo` object to interact with all functionalities of the SDK. All `Apollo` variables are private and can be accessed and updated using `Apollo` getters and setters.

### WiFi Connectivity of the Device

`Apollo Device` takes care of the WiFi connectivity of the device. You can either:

1. _provide_ __WiFi SSID__ and __Passphrase__ along with __API Key__ and __Auth Token__ while `apollo.init()`, in which case it starts trying to connect to the WiFi and then Duplex right away.
OR
2. call `apollo.wifi.smartConfig()` to put device in [smart configuration][WiFi Smart Configuration] and provide __API Key__ and __Auth Token__ through `apollo.duplex.init()`.

#### WiFi Smart Configuration

Smart configuration is a protocol by which you can configure the WiFi of your device on runtime.

### Duplex Connectivity of the Device

Duplex is the protocol by which a device makes realtime connection to `Apollo` server. `Apollo Device` takes care of the duplex connectivity of the device. All you do is provide your project's API Key and your device's Auth Token while `apollo.init()` or later through `apollo.duplex.init()`. As soon as the WiFi gets connected, `Apollo Device` begins trying to connect to the `Apollo` server using the __API Key__ and the __Auth Token__. When it connects, only then can it request to the `Apollo` server to fetch, update or subscribe to any data of the device.

### Quick Example

#### Case 1: Doing `apollo.init()`

```cpp
void setup() {
    /* You can access any function using the global object "apollo"
    */
    apollo.init({ApiKey, DeviceToken, WiFiSSID, WiFiPassphrase});
}

void loop() {
    /* Getting the state of this device
    ** State can exclusively be one of these: {"WIFI_NOT_CONNECTED", "WIFI_CONNECTED", "DUPLEX_CONNECTED"}
    */
    char* state = apollo.getState();
    if(state == "DUPLEX_CONNECTED") {
        /*
        ** Your logic here
        */
    }
}
```

#### Case 2: Doing `apollo.wifi.smartConfig()`

```cpp
void setup() {
    /* You can access any function using the global object "apollo"
    */
    // Putting device in smart configuration mode
    apollo.wifi.smartConfig();
    // Initializing duplex configurations
    apollo.duplex.init({ApiKey, DeviceToken});
}

void loop() {
    /* Getting the state of this device
    ** State can exclusively be one of these: {"WIFI_NOT_CONNECTED", "WIFI_CONNECTED", "DUPLEX_CONNECTED"}
    */
    char* state = apollo.getState();
    if(state == "DUPLEX_CONNECTED") {
        /*
        ** Your logic here
        */
    }
}
```

## Subclasses

`Apollo`  is the main class that wraps the whole functionality of `Apollo Device` SDK. It classifies and expose these functionalities through objects of these _three_ subclasses:

1. _WiFi class_: Provides methods to initialize WiFi connectivity and an interface to interact with WiFi configurations of the device.

2. _Duplex class_: Provides methods to initialize Duplex connectivity, set listeners to events related to duplex connectivity or triggered by `Apollo` server.

3. _Device class_: Provides methods to interact with device's own parameters.

You can access methods of these subclasses using dot notation on the global object `apollo`. For example, doing `apollo.wifi.METHOD`, you can access any method of WiFi class. Similarly, you can do `apollo.duplex.METHOD` and `apollo.device.METHOD` to access methods of duplex and device subclass respectively.

A list of all the methods under these classes can be found [here][Methods].

## Methods

### Apollo Class

`Apollo`, the global class, only provides one method:

* `init()`: Method to initialize device's WiFi ( SSID and Passphrase ) and duplex configurations ( API Key and Auth Token ) in one go.

### WiFi Class

`Apollo WiFi` subclass provides interface for WiFi related functions:

* `init()`: Initializes WiFi configurations ( SSID and Passphrase ) of the device.

* `smartConfigure()`: Puts device in smart configuration mode.

* `getSSID()`: Gets WiFi SSID currently in use by the device.

* `getPassphrase()`: Gets WiFi Passphrase currently in use by the device.

* `getDeviceIP()`: Gets the dynamic IP allocated to the device after it's connected to WiFi.

### Duplex Class

`Apollo Duplex` subclass provides interface to manage realtime connection to `Apollo` server:

* `init()`: Initializes device's duplex configurations ( API Key and Auth Token).

* `update()`: Updates device's duplex buffer. This must be called in `loop()` and without being suspected to any kind of `delay()`.

* `getApiKey()`: Gets the API Key currently in use by the device for connecting to `Apollo` server.

* `getToken()`: Gets the Auth Token currently in use by the device for connecting to `Apollo` server.

* `onConnected()`: Receives a function to call when the device successfully connects to `Apollo` server.

* `onDisconnected()`: Receives a function to call when the device disconnects from `Apollo` server.

### Device Class

`Apollo Device` subclass provides methods to interact with device's data on `Apollo` server:

* `getSummary()`: Getter method for device's [summary][summary].

* `getParms()`: Getter method for device's [parms][parms].

* `setSummary()`: Setter method for device's [summary][summary].

* `setParms()`: Setter method for device's [parms][parms].

* `subscribe()`: Method to subscribe a device [topic][topic].

* `unsubscribe()`: Method to unsubscribe a device [topic][topic].

[Grandeur Cloud]: https://cloud.grandeur.tech "Grandeur Cloud"
[Apollo Device SDK]: https://gitlab.com/grandeurtech/apollo-device "Apollo Device"
[WiFi Smart Configuration]: #WiFi-Smart-Configuration "WiFi Smart Configuration"
[Methods]: #methods "Methods"
[summary]: https://link/to/summary "Summary"
[parms]: https://link/to/parms "Parms"
[topic]: https://link/to/topic "Topic"
