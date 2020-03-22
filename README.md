# Grandeur Cloud [![Version](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://cloud.grandeur.tech)

Building a smart (IoT) product is an art because it is about unifying the physical world with the digital one. When you connect a hardware device to the web, magic happens. But it involves development across a huge technology stack (you need to develop your hardware, your apps to monitor/control your hardware and a server backend to manage both) to make such products work in production. Then if you are (somehow) done with the development operations, there comes the hardest part; you are going to have to scale it all as your user base grows.

We understand this because we have been there.

Introducing Grandeur Cloud: A backend as a service (Baas) platform for IoT. We have designed this platform so you do not have to worry about the backend of your next big thing, and can focus on what matters the most: your hardware and your apps. It is designed specifically to accelerate your IoT product development and push your product to market in weeks rather than in months or years.

## Why Grandeur Cloud

Grandeur Cloud is designed keeping in mind all the challenges a hardware engineer can face in developing and commercializing a smart (IoT) product. You can use out-of-the-box APIs to integrate your devices and apps.

Like you can do authentication of users who use your products, to make sure each user has access to his own data and no one other than the device admin himself should be able to interact with a device. You can save a huge amount of data in a cloud database to extract intelligent information and display it in useful graphs. You can also host your product's static website on Grandeur Cloud. Also, your hardware device can subscribe to events from your app and your app can subscribe to events from your hardware device and communicate data with each other in realtime.

Here are some points that make Grandeur Cloud stand out from the rest of the IoT solutions out there.

* Grandeur Cloud is product-centered. It is much more than just a medium of communication between your app and your hardware. Focusing on expediting IoT product development, it offers an ecosystem of the most necessary tools integrated to make the most head-cracking development problems seamless.

* With Grandeur Cloud, it is extremely simple to [get started][Get Started with Grandeur Cloud] with your IoT product development. Just create a project from the [cloud dashboard][Grandeur Cloud Dashboard], plug your project's API key into the SDK and start developing.

* No need to mix and match various services. Grandeur Cloud is a single spot solution for all of your needs. It has built-in authentication of your users and devices, an integrated database, an out-of-the-box file storage system and a registry of data for all of your devices. You can manage everything from a dashboard and integrate them into your product using our SDKs.

* Simple pricing. [Start free][Grandeur Cloud Sign Up] and then pay as you go based on your resource consumption. Checkout [pricing][Grandeur Cloud Pricing] for more details.

By this time you would be like, okay! we got what motivated Grandeur to build this platform and yeah it is super great and super useful too. But how can we integrate it into our product? So here we have built these amazing SDKs for your apps and hardware to make the integration of Grandeur Cloud in your product quick and seamless. So dive straight in.

[Here][Get Started With Grandeur Cloud] is how you can create a new project on Grandeur Cloud and start using the Javascript SDK to build your IoT apps.

From here onwards, we'll look at how you can use the Hardware SDK to put your devices live and connected on Grandeur Cloud. Let's go!

# Hardware SDK

**Hardware SDK** is the official SDK for hardware that utilizes the *Apollo* API to connect your device to **[Grandeur Cloud][Grandeur Cloud]**.

***NOTE***: Currently, **Apollo Device** SDK is available only for esp8266. Support for the following microcontrollers is coming very soon:

* esp32

Follow the [get started][Get Started with Hardware SDK] guidelines to quickly get into the context of integrating your devices to Grandeur Cloud or jump straight to an [Arduino example][Example] to make your hands dirty.

To get deeper insights and a developer reference for the hardware SDK, you can have a look at the [documentation][Documentation].

To understand the core concepts Grandeur Cloud is built upon, simply dive into the "[A Peek into Grandeur Ecosystem][Ecosystem]" section.

* [Get Started](#get-started)
  * [Installation](#installation)
  * [Initialization](#initialization)
  * [Device State](#device-state)
  * [Updating Device TCP Buffer](#updating-device-tcp-buffer)
  * [Event Listening](#event-listening)
  * [Getting and Updating Device Data](#getting-and-updating-device-data)
* [Example](#example)
* [The Dexterity of Hardware SDK](#the-dexterity-of-hardware-sdk)
* [Grandeur Ecosystem](#grandeur-ecosystem)
  * [A Brief Case Study](#a-brief-case-study)
  * [Concepts](#concepts)
    * [Project](#project)
    * [SDK](#SDK)
    * [User and Administrator](#user-and-administrator)
    * [Device](#device)
    * [Device Registry](#device-registry)
    * [Templates](#templates)
    * [Authentication and Access](#authentication-and-access)
    * [Networking](#networking)
    * [Allowed Origins](#allowed-origins)
* [Documentation](#documentation)
  * [init](#init)
  * [update](#update)
  * [onApolloConnected](#on-apollo-connected)
  * [onApolloDisconnected](#on-apollo-disconnected)
  * [getState](#get-state)
  * [getStringifiedState](#get-stringified-state)
  * [getSSID](#get-ssid)
  * [getPassphrase](#get-passphrase)
  * [getDeviceIP](#get-deviceip)
  * [getApiKey](#get-api-key)
  * [getToken](#get-token)
  * [getSummary](#get-summary)
  * [getParms](#get-parms)
  * [setSummary](#set-summary)
  * [setParms](#set-parms)

## Get Started

### Installation

You can clone **Apollo Device SDK** from [here][Apollo Device SDK].

### Initialization

Initializing Apollo configurations is as simple as calling `apollo.init()` with your credentials (WiFi SSID, WiFi Passphrase, Project's API Key and Device's Access Token).

```cpp
ApolloDevice apolloDevice;
void setup() {
    /* You can initialize device configurations using global object "apollo"
    */
    apolloDevice = apollo.init(
      YourApiKey, YourDeviceToken, YourWiFiSSID, YourWiFiPassphrase
    );
}

void loop() {}

/***RESULT**
 * Initializes Apollo configurations and tries to connect to WiFi right away.
*/
```

### Device State

As your device is transitioning its state from WIFI_DISCONNECTED to WIFI_CONNECTED and then to APOLLO_CONNECTED, you can track these transitions and run code based on this. `apolloDevice.getState()` is your buddy here. It returns the state your device currently is in.
You can also get the stringified form of these states by calling `apolloDevice.getStringifiedState()`.

```cpp
ApolloDevice apolloDevice;
void setup() {
    /* You can initialize device configurations using global object "apollo"
    */
    apolloDevice = apollo.init(
      YourApiKey, YourDeviceToken, YourWiFiSSID, YourWiFiPassphrase
    );
}

void loop() {
    /* Getting the state of this device
    ** State can exclusively be one of these:
    ** {WIFI_NOT_CONNECTED, WIFI_CONNECTED, DUPLEX_CONNECTED}
    */
    int state = apolloDevice.getState();
    if(state == WIFI_NOT_CONNECTED) {
      /* Runs initially when the device is not connected to the WiFi
      */
    }
    else if(state == WIFI_CONNECTED) {
      /* Runs after the device gets connected to the WiFi
      */
    }
    else if(state == APOLLO_CONNECTED) {
      /* Runs after the device has made a successful connection
      ** to Grandeur Cloud
      */
    }
    std::cout<<apolloDevice.getStringifiedState()<<"\n";

    // Updating device's TCP buffer
    apolloDevice.update();
}

/***RESULT**
 * Initially, the code inside the if(state == WIFI_NOT_CONNECTED) runs and
 * "WIFI_NOT_CONNECTED" is printed to stdout in a loop.
 * When device connects to WiFi, "WIFI_CONNECTED" starts
 * appearing on the screen and the code inside the if(state == WIFI_CONNECTED) runs.
 * Finally, When device gets connected to Grandeur Cloud, it changes to "APOLLO_CONNECTED"
 * on the stdout and the code inside the if(state == APOLLO_CONNECTED) starts executing.
*/
```

### Updating Device TCP Buffer

The device does not automatically push messages to the network or pulls new messages from the network. You have to constantly do polling for that.

### Event Listening

You can also attach connection-related event listeners to it. For example, to run a piece of code when the device makes a successful connection to Grandeur Cloud, just wrap that piece of code in a Callback function and pass the function to `apolloDevice.onApolloConnected()`.

Similarly, you can pass the Callback function as a parameter to `apolloDevice.onApolloDisconnected()` and it will be called when the device disconnects from the cloud.

The Callback function is a special type of function that accepts an `unsigned char*` as a parameter and returns `void`.

```cpp
ApolloDevice apolloDevice;
void setup() {
    /* You can initialize device configurations using global object "apollo"
    */
    apolloDevice = apollo.init(
      YourApiKey, YourDeviceToken, YourWiFiSSID, YourWiFiPassphrase
    );

    /* Setting up callback function for connection of the device to the cloud
    */
    apolloDevice.onApolloConnected([](unsigned char* message) {
      puts("Device is now connected to the cloud.");
    });

    /* Setting up callback function for disconnection of the device to the cloud
    */
    apolloDevice.onApolloDisconnected([](unsigned char* message) {
      puts("Device is now disconnected from the cloud.");
    });
}

void loop() {
    // Updating device's TCP buffer
    apolloDevice.update();
}

/***RESULT**
 * Prints "Device is now connected to the cloud." when device gets connected to Grandeur Cloud. And
 * prints "Device is now disconnected from the cloud." when device's connection from Grandeur Cloud
 * breaks.
*/
```

### Getting and Updating Device Data

In the Grandeur ecosystem, we generally classify the device data into two types: Summary for noncontrollable device variables and parms for controllable device variables. You can get and set both using the following functions:

* `apolloDevice.getSummary()`
* `apolloDevice.getParms()`
* `apolloDevice.setSummary()`
* `apolloDevice.setParms()`

Getter functions accept a JSON packet specifying your `deviceID` while setter functions accept a JSON packet specifying your `deviceID` as well as your new summary or parms data.

All of these functions accept a `Callback` function as the second parameter. This function is called with a JSON packet as its parameter when the response message is received.

```cpp
ApolloDevice apolloDevice;
void setup() {
    /* You can initialize device configurations using global object "apollo"
    */
    apolloDevice = apollo.init(
      YourApiKey, YourDeviceToken, YourWiFiSSID, YourWiFiPassphrase
    );
}

void loop() {
    /* Getting the state of this device
    ** State can exclusively be one of these: {WIFI_NOT_CONNECTED, WIFI_CONNECTED, DUPLEX_CONNECTED}
    */
    int state = apolloDevice.getState();
    if(state == APOLLO_CONNECTED) {
        /* Getting device's summary
        */
        char payload[128];
        sprintf(payload, "{\"deviceID\": \"%s\"}", deviceID);
        device.getSummary(payload, [](unsigned char* payload) {
            Serial.printf("Response: %s\n", payload);
        });

        /* Updating device's parms
        */
        sprintf(payload, "{\"deviceID\": \"%s\", \"parms\": {\"state\": true}}", deviceID);
        device.setParms(payload, [](unsigned char* payload) {
            Serial.printf("Response: %s\n", payload);
        });
    }

    // Updating device's TCP buffer
    apolloDevice.update();
}
```

## Example

```cpp
void setup() {
    /* You can initialize device configurations using global object "apollo"
    */
    ApolloDevice apolloDevice = apollo.init(
      YourApiKey, YourDeviceToken, YourWiFiSSID, YourWiFiPassphrase
    );

    /* Setting up callback function for connection of the device to the cloud
    */
    apolloDevice.onApolloConnected([](unsigned char* message) {
      puts("Device is now connected to the cloud.");
    });

    /* Setting up callback function for disconnection of the device to the cloud
    */
    apolloDevice.onApolloDisconnected([](unsigned char* message) {
      puts("Device is now disconnected from the cloud.");
    });
}

void loop() {
    /* Getting the state of this device
    ** State can exclusively be one of these: {WIFI_NOT_CONNECTED, WIFI_CONNECTED, DUPLEX_CONNECTED}
    */
    int state = apolloDevice.getState();
    if(state == APOLLO_CONNECTED) {
        /*
        ** Your logic here
        */
    }
    // Updating device's connection buffer
    apolloDevice.update();
}
```

## The Dexterity of Hardware SDK

What makes the hardware SDK so simple and dextrous is its way of making your code look very well structured. It takes the burden of so many things off your shoulders and makes asynchronous tasks easy to handle. For example,

* **Hardware SDK** takes care of your device's WiFi connectivity by itself. It starts trying to connect to WiFi as soon as you do `apollo.init()`.

* **Hardware SDK** also takes care of your device's connection to [Grandeur Cloud][Grandeur Cloud]. As soon as the WiFi gets connected, **Hardware SDK** begins trying to connect to *[Grandeur Cloud][Grandeur Cloud]* using the *API Key* and the *Access Token* you provide during `apollo.init()`. When it connects, only then can it request the *[cloud][Grandeur Cloud]* to fetch or update any data of your device.

* And as your device quickly flies through these states, i.e.,
  * Device is not connected to WiFi.
  * Device is connected to WiFi, but not connected to Grandeur Cloud yet.
  * Device is finally connected to Grandeur Cloud.

  **Hardware SDK** exposes the state of your device through [`getState()`][getState] function to let you make your decisions based on that.

* Not just that, you can even set event handlers on device's connection and disconnection to Grandeur Cloud using [`onApolloConnected()`][onApolloConnected] and [`onApolloDisconnected()`][onApolloDisconnected].

* There is a special type of function defined in **Hardware SDK** as [`Callback`][callback] that can be passed to some functions of `ApolloDevice` class as a parameter. This callback function is called when the running function exists or when some type of event occurs.

To see the **Hardware SDK** in action, jump to [Example][Example].

# Grandeur Ecosystem

The purpose of writing this is to give you a glimpse into the thought process and psychologies behind designing the Grandeur Cloud Platform the way it feels now. We believe that the first very important step towards choosing a platform for your product and company is to understand the design language of developers of the platform. So we thought of writing about it in detail. We wanted to document how you can use this platform effectively to make your life as a developer or founder a bit simpler.

Here we present a case study to help you understand exactly where, how and what Grandeur Cloud can help you with. Then we explain some of the terms and keywords we use to identify and classify things that make abstraction seamless. So here we go.

## A Brief Case Study

Suppose you are a cleantech startup and want to radicalize the home appliances market to make the appliances more eco and user friendly. You analyzed the market, did user interviews and realized that there is a really big problem in the air conditioner market. Even though millions of new air conditioners are produced every year but there are so many old and inefficient ACs already in the market installed in people's homes and offices. These old air conditioners consume a huge chunk of power and are a major cause of CFCs emissions. Something has to be done because these devices are impacting both the users and the environment. Upgrading each single one of them is just not feasible at all economically.

To resolve the energy efficiency issue of these old ACs, you decided to build an electronic solution that could be used as an extension with these old ACs. So people could control their power consumption without actually upgrading their devices. And you thought of providing your users with an interface to interact with their appliances. You wanted your users to see how much has this new extension been saving them in expenses by cutting down the power consumption. You also wanted to give your users control over how much they wanted to save through this app. In short, you decided to make your product smart (on IoT). And you decided to build a companion app for your device.

That's where the problem arose. You are a hardware startup, after all, that builds amazing electronics technology. But here you got to deal with a few more things as well. You have to build your app and figure out how to establish its communication with your hardware. You may decide to hire more engineers. But do you know how much of them will you have to hire? To give you a perspective, you generally need 8+ engineers just to do the server-end, like one to figure out your networking, one to design and manage your database, one to develop your API (the interface layer between your users and devices), about four for building your SDKs (one for each platform android, ios, web, and hardware) and then when you start scaling up a bit, one DevOps engineer. This makes it a package of $8000+ just to figure out the backend of your system and you haven't even validated your product yet. This turns out exhausting for your business. You have hit a concrete wall with no idea what to do about it.

Then one day the sun of fate shown. You came across a platform that goes by the name of Grandeur Cloud. You went through its [website][Grandeur Technologies] and discovered a perfectly fitting solution for all your headaches. You wanted a solution for authentication of your users, it has the Auth feature in it. You needed online file storage to store maybe the profile pictures of your users and other stuff, it comes with a storage builtin. You were in dire need of a scalable out-of-the-box database to store power consumption logs of your device to show your users graphs on their monthly/yearly savings, it provides a cloud datastore service. And the most important of these all, you needed a realtime communication bridge between your hardware and your apps, THANK GOD, its SDKs are available for all the stacks including Arduino, web, and mobile (both android and ios).

So here you are giving it a shot. You simply [registered for the platform][Grandeur Cloud], created your first project, downloaded their SDKs and started connecting your devices and apps through Grandeur Cloud. You didn't even have to worry about the security of your users and devices, because the data on Grandeur Cloud is protected under standard security protocols. Each user and device in a project is limited in its scope. All you had to worry about was designing your product core and develop your business logic. And in a matter of weeks, your product was out in people's hands, your apps live on app stores. People loved what you built and you were getting live feedback on it. You could see how many people have paired with your devices. You made an early entry into the market and now adding a dent to the universe.

By the way, that is the story of team SolDrive. Check out their [website][SolDrive] right now and explore how they are transforming the world with Grandeur Cloud.

## Concepts

In this subsection, we will explore the Grandeur Cloud platform in detail and see how it pulls it all off. So let's get started.

### Project

A project is the first thing you need to create to start working with Grandeur Cloud. A project is like a namespace, a completely isolated network of users and devices, along with separate file storage and a separate datastore. While you can create an unlimited number of projects, but no two projects can interact or share anything with one other.

Each project is identified by a digital signature that we call the API key, just as your identification card or social security number identifies you as a citizen. To connect your apps or hardware to your project's network, this is what you need to provide to the SDKs. The API key is sent with every request to Grandeur Cloud and this is what defines the project of the request. Check out the [SDK][SDK] section to read more about it.

*NOTE*: Our pricing applies separately to each project. So you get a free tier on every project and pay for each separately for what you consume when you cross your resources limit.

### SDK

Apollo is the API that exposes Grandeur Cloud to the outside world. Our SDKs utilize this API and map each functionality to a function. We have tried our best to make the integration of our SDKs into your codebase simple. For example, while developing your web app, you simply need to drop in the link of JS SDK CDN in your codebase and you are done. We have developed our SDKs for each platform in coherence with each other so you could work and collaborate everywhere seamlessly.

This is how they work: In every SDK, there is a global object aka. `apollo`. You can initialize your configurations (API Key and a couple of more stuff in case of hardware SDK) by calling `apollo.init()`. This returns you a reference to your whole project (in case of your app) or just to your device (in case of hardware because hardware scope is limited to the device itself). In **JS SDK**, you can interact with the authentication API, the device API, the file storage and the datastore API. In the case of **Hardware SDK** your scope is limited to just the device's namespace. Check out the [Authentication and Access][Authentication and Access] section to get more insight into how scope varies across the different platforms (app and hardware).

### User and Administrator

This topic is about the relationship between you as an administrator and your users and the access scope of both.

You aka. **the administrator** is an entity that creates, develops and maintains one or more [projects][Project] on Grandeur Cloud. The administrator has full authority over a project's resources (users, devices, files, and data) and can monitor and control all its projects from the [dashboard][Grandeur Cloud Dashboard].

A **user** is an entity that uses your product. While you have full control over your project, a user of your product has access to his profile and delegated access limited to its device scope only.

In the real world, you would not want to add a new user or pair a device with that user manually every time someone buys your product. Therefore you delegate a part of your project authorities to the SDK when you plug your project's API Key in. And so a new user gets to sign up, pair, monitor and control your device through your product's companion app.

Using just your project's API Key for full delegation is like putting all of your eggs in one basket. A stolen API Key can give the hacker, at the minimum, user-level access to your project. He can register any number of bogus users and do whatnot. Hence the concept of CORS comes to play. Read more on CORS in [Allowed Origins][Allowed Origins] section.

### Device

Devices are the *things* in **Internet of Things** (IoT) around which the IoT product development revolves. Like a user, a device entity has a limited scope of access. But unlike users, you can register new devices only from the dashboard. Read the [Device Registry][Device Registry] section for more on what happens when you register a new device to your project.

On Grandeur Cloud, a device falls under the ownership of the project's administrator. The project's API Key delegates the device pairing authority to the SDK which the user uses to pair with the device. Pairing a device makes it live on Grandeur Cloud and the user gets delegated access to the device's data. But a user cannot delete or modify a device's inherent data because it's not within its scope.

A user can pair with any number of devices but a device can be paired with at the most one user.

The device entity, in the end, defines two things:

* What kind of data a hardware device can access in your namespace and
* Which hardware devices a user can control.

This matters a lot because you would never want your neighbor to control your air conditioner (that would be a horrible situation). That's what this entity has been designed for. A user can only interact with devices that are paired with it.

When you pair a device with a user account, an access token is generated for the device. This token is what the device uses to connect to Grandeur Cloud. This token also delegates access of the device namespace to the Hardware SDK. The Hardware SDK takes this access token along with the project's API Key while doing `apollo.init()`. To read about the device's namespace and how the device's data is stored on Grandeur Cloud, have a look at the [Device Registry][Device Registry] section.

_*NOTE*_: A user cannot pair with a device that is already paired.

### Device Registry

The device registry constitutes one of the key elements of Grandeur Cloud. There are two types of approaches you see out there:

* Those which deal with user's authentication only e.g. firebase and
* Those which employ a device's registry e.g. Google IoT Core, to make sure no unauthorized devices get into your network.

We wanted to combine the best of both worlds. This is why at Grandeur Cloud, not do we just authenticate a device on connection, we also maintain a device's registry for you. See [Authentication and Access][Authentication and Access] section for detail on how a device is authenticated on Grandeur Cloud.

When you register a device, you make it available in your project's network. Not just that, a new namespace is created for your device in the device registry. When your device comes online, this is where all of its data is stored, in the form of i) Summary and ii) Parms.

Now let's define what you can store in **Parms** and **Summary**. To be honest, there is no hard and fast rule about it. We just created two objects instead of a single one to help you develop understanding. However, we generally take the parms as the directly controllable device variables and the summary as those device variables which are not directly controllable or are just needed to be logged or displayed to apps. In another way, the parms sometimes refer to the inputs from the user and the summary refers to the outputs of the device resulting from the inputs. Consider an example where you have a smart light bulb. The parms can store the bulb ON/OFF state which the user can control, while in summary, you can log the voltage consumption of the bulb.

We defined these two objects just to give you a basic framework to work on and build your logic fast. But we are very flexible in what and how you store data in the device registry. You can define an initial schema of these objects using [templates][Templates] and go on from there.

### Templates

### Authentication and Access

Previously, we have discussed in depth which entity (administrator, user, device) can access what. This section revisits the topic and gives you a broader picture of authentication and access scopes. Let's start by outlining the relationships. There are three major scopes:

* The global scope or project's scope
* User scope
* Device scope

You (as an administrator) create a project and therefore have global access to everything. You can access and manage your projects and their resources using the dashboard application. You want your users and devices to have limited access to your project's resources based on their scopes which you achieve by using our SDKs in your apps and hardware. Your project's API Key delegates your project's access to the SDKs and access tokens allow and limit, at the same time, this access to user and device scopes.

The user scope is wider than the device scope. A user can access its profile, the registry of the devices it's paired to, the files in the project's storage and the data in the project's datastore. When a user logs in, an Auth token is returned. This token along with the API Key, being sent with every request, is what validates the authority of the request.

The device scope is limited to the device's namespace in the device registry. When a user pairs with a device, an access token is returned for the device. This access token along with the API Key is what authenticates the device's connection to Grandeur Cloud.

This is how the global project scope is distributed among the smaller entities and we make sure that everyone gets what they are allowed to access.

### Networking

Here we write about how the networking works on Grandeur Cloud.

We work with two communication channels in **Web SDK** i) HTTP based REST API channel and ii) Duplex based realtime API channel. We use the first to do some basic things like authentication or handle big requests like file uploading, while the latter, as its name suggests, for realtime communication like fetching or updating the device's data. The realtime channel is as fast as 200ms RTT. It is based on our custom protocol aka. Duplex. We do not allow any unauthenticated communication over this channel and therefore authenticate a connection over the REST channel first.

In the **Hardware SDK**, we only use the realtime channel. A device cannot establish a connection over this channel unless and until its access token is validated. A device access token is provided while initializing the apollo configurations through `apollo.init()`.

### Allowed Origins

This is another amazing topic and somehow related to access delegation in the end. As mentioned in the sections above that you can interact with your project's namespace through the JS SDK by initializing apollo with your API key. This returns an object referring to your project which can be used to interact with its resources including its devices, datastore, and files storage. Putting this much responsibility on just the API key poses a security threat particularly in case of web apps as API Key can easily be stolen. Even though a user needs to log in first before making any request to the cloud, a hacker with having your API key can still cause some serious damage. For example, Registering bogus users to your project or creating a copycat site on your name for phishing to name a few. That's where cross-origin request sharing (CORS) policies come to play.

So to allow a web app to interact with your project using the Web SDK, you first need to whitelist the domain name your web app uses via the settings page in the dashboard. You cannot even send a request from your localhost without first whitelisting it.

_*NOTE*_: Keeping localhost whitelisted even in a production application is a very serious vulnerability that can make you pay as you go (pun intended).

## Documentation

`ApolloDevice` is the class that wraps the whole functionality of **Apollo Device** SDK. You get the object of this class when you initialize apollo configurations using `apollo.init()`. `apollo` is the global object that lets you initialize Apollo configurations and there's only one function you can call on the `apollo` object, that is, `apollo.init()`. All the other functions are part of the `ApolloDevice` Class.

### Apollo Init

> apollo.init (apiKey: _char*_, token: _char*_, ssid: _char*_, passphrase: _char*_) : returns _ApolloDevice_

Method to initialize apollo configurations (SSID, Passphrase, API Key and Auth Token) in one go. It returns an object of the ApolloDevice class. This object is what exposes all the functions of the Hardware SDK.

#### Parameters

| Name        | Type    | Description               |
|-------------|---------|---------------------------|
| apiKey      | _char*_ | your project API key      |
| token       | _char*_ | your device access token  |
| ssid        | _char*_ | your WiFi SSID            |
| passphrase  | _char*_ | your WiFi passphrase      |

#### Example

```cpp
/* Initialize apollo configurations with your project's API Key, device's Access Token, WiFi SSID and WiFi Passphrase
*/
// Global object of ApolloDevice class
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  // Updates TCP buffer. It sends a connection request to Grandeur Cloud as soon as the WiFi gets connected.
  apolloDevice.update();
}

/***RESULT**
  Device begins connecting to WiFi and then Grandeur Cloud right away.
*/
```

### Update

> update ( ) : returns _char*_

Updates the device's TCP buffer. This must be called in `loop()` and without being suspected to any kind of *delay*. This method is what pulls and pushes messages to and from the TCP channel. Any kind of *delay* in `loop()` would delay pushing the request and pulling the response to and from the server.

[Here][Using Millis Instead of Delay] is how you can use `millis()` instead of `delay()` if you want a function to run after every few moments.

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* Calling the update() function in loop()
  */
  apolloDevice.update();  
}
/***RESULT**
  Updates the TCP buffer every time loop() gets executed
*/
```

### On Apollo Connected

> onApolloConnected (callback : _Callback_) : returns _void_

Receives a function that is called when the device successfully connects to Grandeur Cloud.

#### Parameters

| Name        | Type       | Description                                                        |
|-------------|------------|--------------------------------------------------------------------|
| callback    | _Callback_ | A function to be called when the device connects to Grandeur Cloud |

More on Callback [here][callback].

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);

  /* Passing the callback as lambda function. You can pass any function here as long as it
    accepts a "unsigned char*" string as parameter and returns "void", e.g., a function of the form:

    void callbackFunction(unsigned char* string) {}

  */
  apolloDevice.onApolloConnected([]() {
    std::cout<<"Device Connected!\n";
  });
}

void loop() {
  apolloDevice.update();
}

/***RESULT**
  Prints "Device Connected" on the stdout when device makes a successful connection to Grandeur Cloud
*/
```

### On Apollo Disconnected

> onApolloDisconnected (callback : _Callback_) : returns _void_

Receives a function that is called when the device disconnects from Grandeur Cloud.

#### Parameters

| Name        | Type       | Description                                                           |
|-------------|------------|-----------------------------------------------------------------------|
| callback    | _Callback_ | A function to be called when the device disconnects to Grandeur Cloud |

More on Callback [here][callback].

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);

  /* Passing the callback as lambda function. You can pass any function here as long as it
    accepts a "unsigned char*" string as parameter and returns "void", e.g., a function of the form:

    void callbackFunction(unsigned char* string) {}

  */
  apolloDevice.onApolloDisconnected([]() {
    std::cout<<"Device Disconnected!\n";
  });
}

void loop() {
  apolloDevice.update();
}

/***RESULT**
  Prints "Device Disconnected" on the stdout when device disconnects from Grandeur Cloud
*/
```

### Get State

> getState ( ) : returns _int_

Gets the current state of the device. State of the device can be one of the following:

| Macro                 | Expands to   |
|-----------------------|--------------|
| WIFI_NOT_CONNECTED    | 0            |
| WIFI_CONNECTED        | 1            |
| APOLLO_CONNECTED      | 2            |

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getState() returns an integer state of the device
  */
  if(apolloDevice.getState() == WIFI_NOT_CONNECTED) {
    std::cout<<"Device is not connected to WiFi!\n";
  }
  else if(apolloDevice.getState() == WIFI_CONNECTED) {
    std::cout<<"Yay! Device is now connected to WiFi!\n";
  }
  else if(apolloDevice.getState() == APOLLO_CONNECTED) {
    std::cout<<"Mazay! Device has made a successful connection to Grandeur Cloud!\n";
  }

  apolloDevice.update();
}

/***RESULT**
  In the beginning, getState() returns 0. When WiFi is connected, getState() returns 1.
  When the device is connected to the server, getState() returns 2.
  For readability, you can use the above macros. They respectively expand to these integer values.  
*/
```

### Get Stringified State

> getStringifiedState ( ) : returns _char*_

Gets the current state of the device as a string.

| Macro                 | Stringified Form      |
|-----------------------|-----------------------|
| WIFI_NOT_CONNECTED    | "WIFI_NOT_CONNECTED"  |
| WIFI_CONNECTED        | "WIFI_CONNECTED"      |
| APOLLO_CONNECTED      | "APOLLO_CONNECTED"    |

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getStringifiedState() returns the state of the device as string. This is particularly
    useful while you want to directly print the state of the device
  */
  std::cout<<apolloDevice.getStringifiedState()<<"\n";

  apolloDevice.update();
}

/***RESULT**
  In the beginning, "WIFI_NOT_CONNECTED" is printed to the stdout. When the WiFi is
  connected, "WIFI_CONNECTED" begins appearing on the screen. And it quickly changes to
  "APOLLO_CONNECTED" when the device makes a successful connection to Grandeur Cloud.
*/
```

### Get SSID

> getSSID ( ) : returns _char*_

Gets WiFi SSID currently in use by the device.

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getSSID() returns the SSID of the WiFi it is currently configured to connect to.
  */
  std::cout<<apolloDevice.getSSID()<<"\n";

  apolloDevice.update();
}

/***RESULT**
  Keeps printing the WiFi SSID in loop.
*/
```

### Get Passphrase

> getPassphrase ( ) : returns _char*_

Gets WiFi Passphrase currently in use by the device.

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getPassphrase() returns the Passphrase of the WiFi it is currently configured to
    connect to.
  */
  std::cout<<apolloDevice.getPassphrase()<<"\n";

  apolloDevice.update();
}

/***RESULT**
  Keeps printing the WiFi Passphrase in loop.
*/
```

### getDeviceIP

> getDeviceIP ( ) : returns _char*_

Gets the dynamic IP allocated to the device after it's connected to WiFi.

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getDeviceIP() returns the IP Address assigned to your device.
  */
  std::cout<<apolloDevice.getPassphrase()<<"\n";

  apolloDevice.update();
}

/***RESULT**
  Starts printing the IP address of the device after the device connects to WiFi.
*/
```

### Get API Key

> getApiKey ( ) : returns _char*_

 Gets the [API Key][apikey] currently in use by the device for connecting to Grandeur Cloud.

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getApiKey() returns the API Key the device is configured to use to connect to Grandeur
    Cloud.
  */
  std::cout<<apolloDevice.getApiKey()<<"\n";

  apolloDevice.update();
}

/***RESULT**
  Keeps printing the API Key in a loop.
*/
```

### Get Token

> getToken ( ) : returns _char*_

Gets the [access token][auth token] currently in use by the device for connecting to Grandeur Cloud.

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getToken() returns the Access Token the device is configured to use to connect to
    Grandeur Cloud.
  */
  std::cout<<apolloDevice.getToken()<<"\n";

  apolloDevice.update();
}

/***RESULT**
  Keeps printing the Access Token in a loop.
*/
```

### Get Summary

> getSummary (deviceID: _char*_, callback: _Callback_) : returns _char*_

Getter method for device's [summary][summary].

#### Parameters

| Name        | Type       | Description                                                              |
|-------------|------------|--------------------------------------------------------------------------|
| deviceID    | _char*_    | A string containing a stringified object containing the device ID        |
| callback    | _Callback_ | A function to be called when getSummary response is received             |

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getSummary() sends the getSummary request to the server using the data specified in
    inputPacket. It calls the callback function specified in the second parameter when it
    receives the response.
  */
  char inputPacket[128];
  // Forming a JSON packet
  sprintf(inputPacket, "{\"deviceID\": \"%s\"}", YourDeviceID);
  apolloDevice.getSummary(input, [](unsigned char* outputPacket) {
      std::cout<<outputPacket<<"\n";
  });

  apolloDevice.update();
}

/***RESULT**
  Prints the outputPacket containing the summary (Or just an error code and message in
  case) of the device as soon as it gets getSummary response from the server
*/
```

### Get Parms  

> getParms (deviceID: _char*_, callback: _Callback_) : returns _char*_

Getter method for device's [parms][parms].

#### Parameters

| Name        | Type       | Description                                                              |
|-------------|------------|--------------------------------------------------------------------------|
| deviceID    | _char*_    | A string containing a stringified object containing the device ID        |
| callback    | _Callback_ | A function to be called when getParms response is received               |

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getParms() sends the getParms request to the server using the data specified in
    inputPacket. It calls the Callback function specified in the second parameter when it
    receives the response.
  */
  char inputPacket[128];
  // Forming a JSON packet
  sprintf(inputPacket, "{\"deviceID\": \"%s\"}", YourDeviceID);
  apolloDevice.getParms(input, [](unsigned char* outputPacket) {
      std::cout<<outputPacket<<"\n";
  });

  apolloDevice.update();
}

/***RESULT**
  Prints the outputPacket containing the parms (Or just an error code and message in case
  of the device as soon as it gets getParms response from the server
*/
```

### Set Summary

> setSummary (summary : _char*_, callback: _Callback_) : returns _void_

Setter method for device's [summary][summary].

#### Parameters

| Name        | Type       | Description                                                                                |
|-------------|------------|--------------------------------------------------------------------------------------------|
| summary     | _char*_    | A string containing a stringified object containing the device ID and the new summary data |
| callback    | _Callback_ | A function to be called when setSummary response is received                               |

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* setSummary() sends the setSummary request to the server using the data specified in
    inputPacket. It calls the Callback function specified in the second parameter when it receives the response.
  */
  char inputPacket[128];
  // Forming a JSON packet
  sprintf(inputPacket, "{\"deviceID\": \"%s\", \"summary\": {\"alpha\": 10, \"beta\": true}}", YourDeviceID);
  apolloDevice.setSummary(input, [](unsigned char* outputPacket) {
      std::cout<<outputPacket<<"\n";
  });

  apolloDevice.update();
}

/***RESULT**
  Prints the outputPacket containing the SUMMARY-UPDATED code in response (Or an error code
  and message in case) as soon as it gets it from the server
*/
```

### Set Parms

> setParms (parms : _char*_, callback: _Callback_) : returns _void_  

Setter method for device's [parms][parms].

#### Parameters

| Name        | Type       | Description                                                                                |
|-------------|------------|--------------------------------------------------------------------------------------------|
| parms       | _char*_    | A string containing a stringified object containing the device ID and the new parms data   |
| callback    | _Callback_ | A function to be called when setParms response is received                                 |

#### Example

```cpp
ApolloDevice apolloDevice;
void setup() {
  apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* setParms() sends the setParms request to the server using the data specified in
    inputPacket. It calls the Callback function specified in the second parameter when it receives the response.
  */
  char inputPacket[128];
  // Forming a JSON packet
  sprintf(inputPacket, "{\"deviceID\": \"%s\", \"parms\": {\"charlie\": false}}", YourDeviceID);

  apolloDevice.setParms(input, [](unsigned char* outputPacket) {
      std::cout<<outputPacket<<"\n";
  });

  apolloDevice.update();
}

/***RESULT**
  Prints the outputPacket containing the PARMS-UPDATED code in response (Or an error code
  and message in case) as soon as it gets it from the server
*/
```

[Grandeur Technologies]: https://grandeur.tech "Grandeur Technologies"
[Grandeur Cloud]: https://cloud.grandeur.tech "Grandeur Cloud"
[Grandeur Cloud Sign Up]: https://cloud.grandeur.tech/register "Sign up on Grandeur Cloud"
[Grandeur Cloud Dashboard]: https://cloud.grandeur.tech/dashboard "Grandeur Cloud Dashboard"
[Get Started With Grandeur Cloud]: https://github.com/grandeurtech/grandeurcloud-js-sdk#get-started "Get Started With Grandeur Cloud"
[Grandeur Cloud Pricing]: https://grandeur.tech/pricing "Pricing"

[Example]: #example "Hardware SDK Example"
[Documentation]: #documentation "Documentation"
[Ecosystem]: #grandeur-ecosystem "Grandeur Ecosystem"

[Project]: #project "Project"
[SDK]: #sdk "SDK"

[SolDrive]: https://sol-drive.com/ "SolDrive"

[Authentication and Access]: #authentication-and-access "Authentication and Access"
[Allowed Origins]: #allowed-origins "Allowed Origins"
[Device Registry]: #device-registry "Device Registry"

[Get Started with Hardware SDK]: #get-started "Get Started with Hardware SDK"
[Hardware SDK]: https://github.com/grandeurtech/grandeurcloud-hardware-sdk "Hardware SDK"
[project]: #project "Project"
[api key]: #api-key "API Key"
[user]: #user "User"
[consumer]: #consumer "Consumer"
[device]: #device "Device"
[apollo server]: #apollo-server "Apollo Server"
[duplex]: #duplex-channel "Duplex Channel"
[auth token]: #auth-token "Auth Token"
[Methods]: #methods "Methods"
[summary]: #device-summary "Summary"
[parms]: #device-parms "Parms"
[topic]: #topic "Topic"
[subscription]: #subscription "Subscription"
[callback]: #event-listening "Callback"
[payload]: #payload "Payload"

[getState]: #get-state
[onApolloConnected]: #on-apollo-connected
[onApolloDisconnected]: #on-apollo-disconnected

[Using Millis Instead of Delay]: https://www.norwegiancreations.com/2017/09/arduino-tutorial-using-millis-instead-of-delay/ "Using millis() instead of delay()"
