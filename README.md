# Apollo Device

`Apollo Device` is the hardware SDK that utilizes apollo API to authenticate and connect a hardware device to the matrix of [Grandeur Cloud][Grandeur Cloud].

_**NOTE**_: Currently, `Apollo Device` SDK is available only for Arduino IDE.

## Features

## How does it work

## Installation

You can clone `Apollo Device` SDK from [here][Apollo Device SDK].

## Basics of Apollo Device

`Apollo Device` provides a global apollo object which can be used to access its functions. All `Apollo` variables are private and can only be accessed and updated using `Apollo` getters and setters.

### WiFi Connectivity

`Apollo Device` takes care of the WiFi connectivity of the device. There can be following two cases:

1. You provide WiFi SSID and Passphrase while `apollo.init()`, in which case it starts trying to connect to the WiFi.

2. You do not provide WiFi SSID and Passphrase while `apollo.init()` in which case apollo waits for you to
    * either provide WiFi configurations
    * or call `apollo.wifi.smartConfig()`.

More on [WiFi Smart Configuration](#WiFi-Smart-Configuration) later.

#### WiFi Smart Configuration

Smart configuration is a protocol by which you can configure the WiFi of your device on runtime.

### Duplex Connectivity

`Apollo Device` also takes care of your duplex connectivity. All you need is to provide your project's API Key and your device's Auth Token while `apollo.init()`. As soon as the WiFi gets connected, `Apollo Device` begins trying connecting to the Apollo server using the API Key and the Auth Token. When it connects, only then it can request to fetch or update any device's data.

### Quick Example

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

## Subclasses

`Apollo` class consists of _four_ subclasses:

1. _WiFi class_: Provides methods to initialize WiFi connectivity and an interface to interact with device's WiFi configurations.

2. _Duplex class_: Provides methods to initialize Duplex connectivity, set listeners to various events related to duplex connectivity or triggered by `Apollo` server.

3. _Device class_: Provides methods to interact with device's own parameters.

`Apollo` class contains objects of these subclasses to provide access to their methods, for example, you can use `apollo.wifi` to access methods of WiFi class. Similarly, you can you `apollo.duplex` and `apollo.device` objects to access their respective methods.

A list of all the methods provided by these classes can be found [here][methods].

## Methods

### Apollo Class

Following are the methods to provided by the `Apollo` class:

* `init()`: Method to initialize device's WiFi configurations ( SSID and Passphrase ).

### WiFi Class

Following are the methods to provided by the `Apollo WiFi` subclass:

* `init()`: Method to initialize device's WiFi configurations ( SSID and Passphrase ).

* `smartConfigure()`: Method to put device in smart configuration mode.

* `getSSID()`: Method to get WiFi SSID currently in use by the device.

* `getPassphrase()`: Method to get WiFi passphrase currently in use by the device.

* `getDeviceIP()`: Method to get the dynamic IP allotted to the device after it's connected to the WiFi.

### Duplex Class

Following are the methods to provided by the `Apollo Duplex` subclass:

* `init()`: Method to initialize device's duplex configurations ( API Key and Auth Token).

* `update()`: Method to update device's duplex buffer. This must be called in `loop()` and without being suspected to any kind `delay()`.

* `getApiKey()`: Method to get the API Key currently in use by the device to connect to the `Apollo` server.

* `getToken()`: Method to get the Auth Token currently in use by the device to connect to the `Apollo` server.

* `onConnected()`: Receives a function to be called when the device successfully connects to the `Apollo` server.

* `onDisconnected()`: Receives a function to be called when the device disconnects from the `Apollo` server.

### Device Class

Following are the methods to provided by the `Apollo Device` subclass:

* `getSummary`: Getter method for device's [summary][summary].

* `getParms`: Getter method for device's [parms][parms].

* `setSummary`: Setter method for device's [summary][summary].

* `setParms`: Setter method for device's [parms][parms].

* `subscribe`: Method to subscribe to a device's [topic][topic].

* `unsubscribe`: Method to unsubscribe from a device's [topic][topic].

[Grandeur Cloud]: https://cloud.grandeur.tech "Grandeur Cloud"
[Apollo Device SDK]: https://gitlab.com/grandeurtech/apollo-device "Apollo Device"
[methods]: #methods "Methods"
[summary]: https://link/to/summary "Summary"
[parms]: https://link/to/parms "Parms"
[topic]: https://link/to/topic "Topic"
