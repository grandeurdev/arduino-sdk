# Grandeur Cloud [![Version](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://cloud.grandeur.tech)

We are making it easier for you to build internet of things based smart products with our cloud platform and software development kit. [Let`s Sign Up][Grandeur Cloud] and create something amazing rightnow!

# Apollo Device

**Apollo Device** is the official SDK for hardware that utilizes *Apollo* API to authenticate and connect it to **[Grandeur Cloud][Grandeur Cloud]**.

***NOTE*** : Currently, **Apollo Device** SDK is available only for Arduino IDE.

# List of Contents
- [Get Started](#get-started)
- [Documentation](#documentation)
  * [init](#init)
  * [getSSID](#get-SSID)
  * [getPassphrase](#get-Passphrase)
  * [getDeviceIP](#get-DeviceIP)
  * [update](#update)
  * [getApiKey](#get-Api-Key)
  * [getToken](#get-Token)
  * [onConnected](#on-Connected)
  * [onDisconnected](#on-Disconnected)
  * [getSummary](#get-Summary)
  * [getParms](#get-Parms)
  * [setSummary](#set-Summary)
  * [setParms](#set-Parms)
  * [subscribe](#subscribe)
  * [unsubscribe](#unsubscribe)
- [A peek into Grandeur Ecosystem](#)

   

## A Quick Case Study

Let's say, a person A (consumer) buys a smart invertor developed by a person B (developer). He uses it to stabilize the power consumption of his air conditioner. On an app provided by the developer person B, he gets to monitor the voltage, current and power consumption of his AC. He can also turn his AC ON/OFF, or regulate the percentage of total power it would consume.

In this case, the controllable parameters are AC's ON/OFF state and its percentage power consumption, and therefore, can be defined under *[parms][parms]*. On the other hand, voltage and current of the AC are not directly controllable, but can be monitored. Therefore, they can be defined under *[summary][summary]*.

Now the consumer might want to see live voltage and current waveforms in his app and turn the AC ON/OFF or regulate its percentage power consumption in realtime.

Since, AC voltage and current are defined under *device summary*, the app would subscribe to *device summary* so that any updates to variables under *summary* would be sent to the app in realtime and displayed in a graph.

Similarly, the device can subscribe to *parms* to get realtime updates when the consumer toggles its state or changes its percentage power consumption.

Also, the app may also subscribe to *parms* to get notification if and when the device goes offline (turns OFF).

## Installation

You can clone **Apollo Device SDK** from [here][Apollo Device SDK].

## Basics of Apollo Device

**Apollo Device** provides a global `apollo` object to interact with all functionalities of the SDK. All `apollo` variables are private and can be accessed and updated using `apollo` getters and setters.

### WiFi Connectivity of the Device

**Apollo Device** takes care of the WiFi connectivity of the device. You can either:

1. *provide* **WiFi SSID** and **Passphrase** along with **API Key** and **Auth Token** while `apollo.init()`, in which case it starts trying to connect to the WiFi and then Duplex right away.
OR
2. call `apollo.wifi.smartConfig()` to put device in [smart configuration][WiFi Smart Configuration] and provide **API Key** and **Auth Token** through `apollo.duplex.init()`.

#### WiFi Smart Configuration

Smart configuration is a protocol by which you can configure the WiFi of your device on runtime.

### Duplex Connectivity of the Device

/* HIDE THIS UNDER THE SHADOWS. NO DUPLEX EXISTS FROM NOW ON. */

Duplex is the communication protocol by which a device makes realtime connection to *Apollo server*. **Apollo Device** takes care of the duplex connectivity of the device. All you do is provide your *project's API Key* and your *device's Auth Token* while `apollo.init()` or later through `apollo.duplex.init()`. As soon as the WiFi gets connected, **Apollo Device** begins trying to connect to *Apollo server* using the *API Key* and the *Auth Token*. When it connects, only then can it request to the *Apollo server* to fetch, update or subscribe to any data of the device.

### Payload

/* REMOVE PAYLOAD */
Serialized JSON packet from *Apollo server* is parsed by **Apollo device** into this special object. Payload is based on three parameters:

* `numberOfKeys`
* `keys`
* `values`

For example, a packet like

`{"header": {id: 1, task: "getSummary"}, "payload": {"foo": 0, "bar": "foobar"}}`

is parsed into Payload object as

`payload.numberOfKeys = 2`,

`payload.keys = ["foo", "bar"]`,

`payload.values = [0, "foobar"]`

However, in the case of nested keys, only the keys at the lowest level are parsed in the Payload. For example, a packet like

`{"header": {id: 1, task: "getParms"}, "payload": {"foo": {"bar": "foobar"}}}`

is parsed into Payload as

`payload.numberOfKeys = 1`,

`payload.keys = ["bar"]`,

`payload.values = ["foobar"]`

### Callback

`Callback` is a special type of function that can be passed to another function to call it before exiting or when some type of event occurs.

`Apollo Device` methods only accept a function that *receives* a `pointer to Payload object` and *returns* `void`.

```cpp
/* Only this type of functions are accepted as callback
*/
void function(Payload* payload) {
    /*
    ** Your logic here
    */
}
```

## Starting with Apollo Device

### Case 1: Doing `apollo.init()`

```cpp
void setup() {
    /* You can access any function using the global object "apollo"
    */
    apollo.init({ApiKey, DeviceToken, WiFiSSID, WiFiPassphrase});
}

void loop() {
    /* Getting the state of this device
    ** State can exclusively be one of these: {"WIFI*NOT*CONNECTED", "WIFI*CONNECTED", "DUPLEX*CONNECTED"}
    */
    char* state = apollo.getState();
    if(state == "DUPLEX*CONNECTED") {
        /*
        ** Your logic here
        */
    }
    // Updating duplex buffer
    apollo.duplex.update();
}
```

### Case 2: Doing `apollo.wifi.smartConfig()`

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
    ** State can exclusively be one of these: {"WIFI*NOT*CONNECTED", "WIFI*CONNECTED", "DUPLEX*CONNECTED"}
    */
    char* state = apollo.getState();
    if(state == "DUPLEX*CONNECTED") {
        /*
        ** Your logic here
        */
    }
    // Updating duplex buffer
    apollo.duplex.update();
}
```

## Quick Summary of Apollo Device SDK

### Subclasses

*Apollo* is the main class that wraps the whole functionality of **Apollo Device** SDK. It classifies and exposes these functionalities through objects of these *three* subclasses:

1. *WiFi class*: Provides methods to initialize WiFi connectivity and an interface to interact with WiFi configurations of the device.

2. *Duplex class*: Provides methods to initialize Duplex connectivity, set listeners to events related to duplex connectivity or triggered by *Apollo server*.

3. *Device class*: Provides methods to interact with device's own parameters.

You can access methods of these subclasses using dot notation on the global object `apollo`. For example, doing `apollo.wifi.METHOD`, you can access any method of WiFi class. Similarly, you can do `apollo.duplex.METHOD` and `apollo.device.METHOD` to access methods of duplex and device subclass respectively.

<!-- A list of all the methods under these classes can be found [here][Methods].

### Methods

#### Apollo Class

*Apollo*, the global class, only provides one method:

* [`init()`][apollo.init]: Method to initialize device's WiFi (SSID and Passphrase) and duplex configurations ( API Key and Auth Token ) in one go.

* [`getSSID()`][apollo.wifi.getSSID]: Gets WiFi SSID currently in use by the device.

* [`getPassphrase()`][apollo.wifi.getPassphrase]: Gets WiFi Passphrase currently in use by the device.

* [`getDeviceIP()`][apollo.wifi.getDeviceIP]: Gets the dynamic IP allocated to the device after it's connected to WiFi.

#### Duplex Class

`Apollo Duplex` subclass provides interface to manage realtime connection to *Apollo server*:

* [`update()`][apollo.duplex.update]: Updates device's duplex buffer. This must be called in `loop()` and without being suspected to any kind of *delay*.

* [`getApiKey()`][apollo.duplex.getApiKey]: Gets the API Key currently in use by the device for connecting to *Apollo server*. This method is what receives and pushes messages to and from the duplex channel.

* [`getToken()`][apollo.duplex.getToken]: Gets the Auth Token currently in use by the device for connecting to *Apollo server*.

* [`onConnected()`][apollo.duplex.onConnected]: Receives a function to call when the device successfully connects to *Apollo server*.

* [`onDisconnected()`][apollo.duplex.onDisconnected]: Receives a function to call when the device disconnects from *Apollo server*.

#### Device Class

`Apollo Device` subclass provides methods to interact with device's data on *Apollo server*:

* [`getSummary()`][apollo.device.getSummary]: Getter method for device's [summary][summary].

* [`getParms()`][apollo.device.getParms]: Getter method for device's [parms][parms].

* [`setSummary()`][apollo.device.setSummary]: Setter method for device's [summary][summary].

* [`setParms()`][apollo.device.setParms]: Setter method for device's [parms][parms].

* [`subscribe()`][apollo.device.subscribe]: Method to subscribe a device [topic][topic].

* [`unsubscribe()`][apollo.device.unsubscribe]: Method to unsubscribe a device [topic][topic]. -->

## Documentation

### Init
> init ( ) : returns *void*

Method to initialize device's WiFi (SSID and Passphrase) and duplex configurations (API Key and Auth Token) in one go.

**Parameters**  

<table>
  <tr>
    <th>Name</th>
    <th>Type</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>apiKey</td>
    <td><em>char*</em></td>
    <td>your valid API key.</td>
  </tr>
  <tr>
    <td>token</td>
    <td><em>char*</em></td>
    <td>your auth token.</td>
  </tr>
  <tr>
    <td>ssid</td>
    <td><em>char*</em></td>
    <td>your WIFI ssid.</td>
  </tr>
  <tr>
    <td>passphrase</td>
    <td><em>char*</em></td>
    <td>your WIFI password.</td>
  </tr>
</table>


### Smart Config
> smartConfig ( ) : returns *void*

Puts device in smart configuration mode.


### Get SSID
> getSSID ( ) returns *char**

Gets WiFi SSID currently in use by the device.


### Get Passphrase
> getPassphrase ( ) returns *char**

Gets WiFi Passphrase currently in use by the device.


### getDeviceIP
> getDeviceIP ( ) : returns *char**

Gets the dynamic IP allocated to the device after it's connected to WiFi.


<!-- ### `apollo.duplex.init()`

Initializes device's duplex configurations (API Key and Auth Token).

**Parameters** `(DuplexConfig {char* ssid, char* passphrase})`

**Returns** `char* wifiSSID` -->

### Update
> update ( ) : returns *char**

Updates device's duplex buffer. This must be called in `loop()` and without being suspected to any kind of *delay*. This method is what receives and pushes messages to and from the duplex channel.


### Get Api Key
> getApiKey ( ) : returns *char**

 Gets the API Key currently in use by the device for connecting to *Apollo server*. This method is what receives and pushes messages to and from the duplex channel.

More on API key [here][apikey].

### Get Token
> getToken ( ) : returns *char**

Gets the Auth Token currently in use by the device for connecting to *Apollo server*.

More on Auth token [here][token].

### On Connected
> onConnected (callback : *function*) : returns *void*

Receives a function to call when the device successfully connects to *Apollo server*.

More on Callback [here][callback].

### On Disconnected
> onDisconnected (callback : *function*) : returns *void*

Receives a function to call when the device disconnects from *Apollo server*.

More on Callback [here][callback].

### Get Summary
> getSummary ( ) returns *char**  

Getter method for device's [summary][summary].

More on device Summary [here][summary].

### Get Parms  

> getParms ( ) returns *char**

Getter method for device's [parms][parms].

More on device Parms [here][parms].

### Set Summary
> setSummary (payload : *Payload*) : returns *void* 

Setter method for device's [summary][summary].

More on Payload [here][payload].

### Set Parms
> setParms (payload : *Payload*) : returns *void*  

Setter method for device's [parms][parms].  

More on Payload [here][payload].

### Subscribe
> subscribe (payload : *Payload*) : returns *void*  

Method to subscribe a device [topic][topic].

More on Payload [here][payload].

### Unsubscribe
> unsubscribe (payload : *Payload*) : returns *void*   

Method to unsubscribe a device [topic][topic].

More on Payload [here][payload].  


## A Peek into Grandeur's Ecosystem

### Project

A *project* is the most top level entity in **[Grandeur Cloud][Grandeur Cloud]**. It defines an isolated ecosystem for all your nodes (Users and devices), which means no node in one project can be related to or interact with any node of some other project.

Each project is identified by a unique string of characters. We call it the [API Key][api key].

A project is created by a *[user][user]*.

A project can have any number of consumers and devices, interconnected in any number of ways.

### API key

*API Key* is a unique string of characters, generated when you create a new *[project][project]* on **[Grandeur Cloud][Grandeur cloud]**. It is an identifier for the *project* you request to interact with.

Each node that belongs to project A must send project A's *API Key* with every request to be able to interact with project A's ecosystem.

### User

A *user* is an entity that creates, develops and maintains one or more *[projects][project]* on **[Grandeur Cloud][Grandeur Cloud]**. It's the admin of the *project* and has the full authority to monitor and control all its *projects* from [Grandeur Dashboard][grandeur dashboard].

A *user* can create any number of *projects* but a *project* can have at the most one admin aka *user*.

### Consumer

A *consumer* is the end user that uses the *user*'s product. It lives in the ecosystem aka project created by the *user*, and interacts with other nodes (devices and other consumers) of the *[project][project]*.

### Device

A *device* is the hardware product that a *consumer* can monitor and control. To be precise, there are some *device* variables that a *[consumer][consumer]* actually interacts with. These interactive variables are specified under *device*'s *[summary][summary]* and *[parms][parms]*.

All the nodes in a *project* interact with each other through *[Apollo server][apollo server]*.

### Apollo Server

*Apollo server* is the central hub for all the communications happening among all the *[consumers][consumer]* and all the *[devices][device]* in all the *[projects][project]*. *Apollo server* is what isolates *projects* from each other, maintains *[duplex connection][duplex]* with all nodes, and routes messages among them.

Each node communicate with the other through a realtime *duplex* channel.

### Duplex Channel

**Duplex** is the channel on which realtime communication is done between a node and *Apollo server*. An *interaction* between two nodes happens through two *duplex* channels, one between the source node and *Apollo server* and the other between *Apollo server* and destination node.

A *project* can open as many *duplex* channels as it needs.

### Auth Token

*Auth Token* is an identification token that lets *Apollo server* identify who a node is in a *[project][project]*'s ecosystem.

When a consumer logs in using its password, an *Auth token* is sent back to it. This *Auth token* along with the project's *[API key][apiKey]* is sent with every request made to *Apollo server* for the request to be considered valid.

When a consumer *pairs* a device, a *device Auth token* is sent to the *[consumer][consumer]* who forwards it to the *[device][device]* to make the device live in the project.

A *consumer* Auth token cannot be used in place of a device token or vice versa.

### Device Summary

Each device has some variables that a consumer might want to interact with (monitor or control).

*Summary* includes those device variables which *are not directly controllable*. For example, in an air conditioner, the controllable variable is its state (ON/OFF) or the temperature dial you see on its display, while its voltage, current and power consumption would be non-controllable variables, thus opted to be under *summary*.

### Device Parms

*Parms* are *the directly controllable* variables. In the previous air conditioner example, its state and the temperature dial would be opted for *parms* category.

### Subscription

*Subscription* is the way by which a node in a consumer-device pair shows its interest in some device variables. Whenever an update occurs in those variables, the subscribing node is notified about it.

### Topic

*Topics* are the device *variables that can be [subscribed][subscription]*. For example, a *consumer* may want to be notified if air conditioner's power consumption changed or if a *device* turned off for some reason.


[Grandeur Cloud]: https://cloud.grandeur.tech "Grandeur Cloud"
[Apollo Device SDK]: https://gitlab.com/grandeurtech/apollo-device "Apollo Device"
[WiFi Smart Configuration]: #wifi-smart-configuration "WiFi Smart Configuration"
[project]: #project "Project"
[api key]: #api-key "API Key"
[user]: #user "User"
[consumer]: #consumer "Consumer"
[device]: #device "Device"
[apollo server]: #apollo-server "Apollo Server"
[duplex]: #duplex-channel "Duplex Channel"
[auth token]: #auth-token "Auth Token"
[grandeur dashboard]: https://cloud.grandeur.tech/dashboard "Grandeur Dashboard"
[Methods]: #methods "Methods"
[summary]: #device-summary "Summary"
[parms]: #device-parms "Parms"
[topic]: #topic "Topic"
[subscription]: #subscription "Subscription"
[callback]: #callback "Callback"
[payload]: #payload "Payload"

[apollo.init]: #apolloinit
[apollo.wifi.init]: #apollowifiinit
[apollo.wifi.smartConfig]: #apollowifismartconfig
[apollo.wifi.getSSID]: #apollowifigetssid
[apollo.wifi.getPassphrase]: #apollowifigetpassphrase
[apollo.wifi.getDeviceIP]: #apollowifigetdeviceip
[apollo.duplex.init]: #apolloduplexinit
[apollo.duplex.update]: #apolloduplexupdate
[apiKey]: #getApiKey

[apollo.duplex.getToken]: #apolloduplexgettoken
[apollo.duplex.onConnected]: #apolloduplexonconnected
[apollo.duplex.onDisconnected]: #apolloduplexondisconnected
[apollo.device.getSummary]: #apollodevicegetsummary
[apollo.device.getParms]: #apollodevicegetparms
[apollo.device.setSummary]: #apollodevicesetsummary
[apollo.device.setParms]: #apollodevicesetparms
[apollo.device.subscribe]: #apollodevicesubscribe
[apollo.device.unsubscribe]: #apollodeviceunsubscribe