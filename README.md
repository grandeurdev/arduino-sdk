# Grandeur Cloud [![Version](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://cloud.grandeur.tech)

Building a smart (IoT) product is an art, because it is about unifying the physical world with the digital one. When you connect a hardware to the web, magic happens. But it involves development across a huge technology stack (you need to develop your hardware, your apps to monitor/control your hardware and a server backend manage both) in order to make such products work over production. Then if you are (somehow) done with the development operations, there comes the hardest part; you are going to have to scale it all as your user base grows.

We understand this, because we have been there.

Introducing Grandeur Cloud; a backend as a service platform for IoT. We have designed this platform so you do not have to worry about the backend of your next big thing, and can focus on what matters the most: your hardware and your apps. It is designed specifically to accelerate your IoT product development, and push your product to market in weeks rather than months or years.

## Why Grandeur Cloud

Grandeur Cloud is designed keeping in mind all the challenges a hardware engineer can face in developing and commercializing a smart (IoT) product. You can use out-of-the-box APIs to integrate in your devices and apps.

Like you can do authentication of users who use your products, to make sure each user gets his own data and no one other than the device admin himself should be able to access it. You can save huge amount of data in cloud database, in order to extract intelligent information and display it in useful graphs. You can also host your static website on Grandeur Cloud. Along with all that, you can subscribe your hardware device to events from your app and your app to events from hardware device to communicate in realtime with each other.

By this time you would be like, okay, we got it why Grandeur is building this platform and yeah it is super great and super useful. But how can we integrate it in our product? So here you go. We have built these amazing SDKs for your apps and hardware to make the integration process of Grandeur Cloud in your product simple.

# Hardware SDK

Now that you have seen how much Grandeur Cloud can lay off your shoulders, you can dive straight into learning how you can integrate it in your IoT product development.

[Here][Get Started With Grandeur Cloud] is how you can create a new project on Grandeur Cloud and start using the Javascript SDK to build your IoT apps.

From here onwards, we'll look at how you can use the Hardware SDK to put your devices live and connected on Grandeur Cloud. Let's go!

**Hardware SDK** is the official SDK for hardware that utilizes *Apollo* API to authenticate and connect the hardware device to **[Grandeur Cloud][Grandeur Cloud]**.

***NOTE*** : Currently, **Apollo Device** SDK is available only for Arduino IDE.

Just follow the [get started][Get Started with Hardware SDK] guidelines to quickly start integrating your devices to Grandeur Cloud.

In order to get deeper insights into the hardware SDK you can have a look at the [documentation][Documentation]. To understand some of the core concepts Grandeur Cloud is build upon, simply dive into the [ecosystem][Ecosystem] section.

* [Get Started](#get-started)

* [Documentation](#documentation)
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

* [A peek into Grandeur Ecosystem](#A-Peek-into-Grandeur's-Ecosystem)

## Get Started

### Installation

You can clone **Apollo Device SDK** from [here][Apollo Device SDK].

### Initialization

Initializing ApolloDevice is as simple as calling `apollo.init()` with the your credentials.

### Event Listening

You can also attach connection-related event listeners to it. For example, to run a function when the device makes a successful connection to Grandeur Cloud, just call `apolloDevice.onApolloConnected()` with the **function to be called** as *parameter*.

Similarly, you can pass a function as parameter to `apolloDevice.onApolloDisconnected()` and it will be called when the device disconnects from the cloud.

### Current State of the Device

`apolloDevice.getState()` returns state of the device, which can either be WIFI_DISCONNECTED, WIFI_CONNECTED or APOLLO_CONNECTED.
You can also get the stringified form of these states by calling `apolloDevice.getStringified
State()`.

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
    apolloDevice.onApolloConnected([](char* message) {
      puts("Device is now connected to the cloud.");
    });

    /* Setting up callback function for disconnection of the device to the cloud
    */
    apolloDevice.onApolloDisconnected([](char* message) {
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

# Grandeur Ecosystem

The purpose behind writing is to tell you what is the thought process and psychology behind Grandeur Cloud Platform. We believe that the first important step toward chosing a platform for you product and company is to understand that how the developer designed the system. So we wanted to write about it in detail. We wanted to document that how can use this platform effectively to make your life as a developer or founder a bit simpler. So in this section, we will first illustrate that why to use Grandeur Cloud as a developer, then we will present a breif case study and finally we will write about the concepts.

## Why Use Grandeur as a Developer

* It is simple to [get started][Get Started with Grandeur Cloud]. Just create a project at cloud dashboard and simply get a reference to your project using our SDK.

* No need to mix and match various services because it is single spot solution for all of your needs. It has built in support for authenticating users and device registration. You can access all the features like authentication, file storage, database and device registry from a single SDK.

* Simple pricing. [Start free][Grandeur Cloud Sign Up] and then pay as you go based on resource consumption. Checkout [pricing][Grandeur Cloud Pricing] to get more details.

## A Brief Case Study

Suppose you are a clean tech startup and want to radicalize the home appliances market to make the appliances more eco and user friendly. You analyzed the market, did user interviews and realized that there is one really big problem in the air conditioner market. Every year millions of new air conditioners are produced but the problem is there are so many old and inefficient ACs already in the market installed in people's homes and offices. These old air conditioners consume a huge chunk of power and a major cause of CFCs emissions. Something has to be done because these devices are impacting both the users and the ecosystem. But upgrading each single one of them is just not feasible at all economically.

In order to resolve the energy efficiency issue of these old ACs, you decided to build an electronic solution which could be used as an extension with these old ACs already installed in people's homes. So that, we could control the power consumption of the ACs without actually upgrading them. Then you realized that you can provide your users an interface to interact with their appliances. You wanted your users to see how much has this new extension been saving them in expenses by cutting down the power consumption. You also wanted to give your users control over how much they wanted to save through this app. In short, you decided to make your product smart (on IoT). You decided to build a companion app for your device.

That's where the problem arose. You are a hardware startup after all, experted in building amazing electronics technology. But here you got to deal with a few more things as well. You have to build your app and figure out how to establish communication between your hardware product and app. You may decide to hire more engineers. But do you know how much of them will you have to hire? To give you a perspective, you need 8+ engineers just to do the server-end, like one to figure out your networking, one to design and manage your database, one to develop your API (the interface layer between your users and devices), about four for building your SDKs (one for each platform android, ios, web and hardware) and then when you start scaling up a bit, one for your DevOps. Which makes it a package of $8000+ just to figure out the backend of your system and you haven't even validated your product yet. That is exhausting for your business. You have hit a concrete wall with no idea what to do about it.

Then one day the sun of fate shown. You came across a platform that goes by the name of Grandeur Cloud. You went through its [website][Grandeur Technologies] and discovered a perfectly fitting solution for all your headaches. You wanted a solution for authentication of your users, it has the Auth feature in it. You needed an online file storage to store maybe the profile pictures of your users and other stuff, it comes with a storage builtin. You were in dire need of a scalable out-of-the-box database to store power consumption logs of your device to show your users graphs on their monthly/yearly savings, it provides a cloud datastore service. And the most important of these all, you needed a realtime communication bridge between your hardware and your apps, THANK GOD, its SDKs are available for all the stacks including arduino, web and mobile (both android and ios).

So here you are giving it a shot. You simply [registered for the platform][Grandeur Cloud], created your first project, downloaded their SDKs and started connecting your devices and apps together through Grandeur Cloud. You didn't even have to worry about the security of your users and devices, because the data on Grandeur Cloud is protected under standard security protocols. Each user and device in a project has its own limited scope. All you had to worry about was designing your product core and develop your business logics. And in matter of weeks, your product was out in people's hands, your apps live on app stores. People loved what you built and you were getting live feedback on it. You made an early entry into the market and now adding a dent to the universe.

By the way, that is the story of team SolDrive. Checkout their [website][SolDrive] right now and explore how they are transforming the world with Grandeur Cloud.

## Concepts

In this sub section we will explore the Grandeur Cloud Platform in detail. We will see how it all works in depth. So let's get started.

### Projects

To start working with Grandeur Cloud, the first thing that you will have to do is to create a new project. Now what is a project? Project is like a workspace and we store, communicate and display data with respect to your project. While you can technically create unlimited number of projects, but you cannot share data or resources of any sort between two projects. Your project works like a namespace. Like users registered to one project, cannot login to applications based on other projects. Similarly devices regsitered to one project, cannot be shared to another project.

When you create a project, we give you a project API key. An API key is a digital signature that identifies a project in our system, just like your identification card or your social security number identifies you as a citizen. In order to connect your apps or hardware to our network, that is what you should provide to our SDK. Our SDK sends us your API key with every request and that is what we utilize to understand what data are we supposed to update or return or in which namespace we are supposed to execute your request. Checkout sdk section in concepts to read more about it.

In the end, it is important to note that our pricing applies separately to each project. So you will get free tier on every project and then you will pay for each project separately with respect to what you consume in each namespace.

### SDK

You use our SDKs to communicate to our cloud platform. We call our SDK Apollo and it acts like an interface that gives you easy access to integrate our platform in your stack. Like in case of web apps, simply drop in the link of JS SDK CDN in your code base and done. We have tried our best to make it coherent in between the platforms. So that you could work and collaborate seemlessly.

So that is how it works. You can the SDK global object by name and intialize it with API key (plus couple of more stuff in case of hardware SDK). As a result of initialization, you will get the refernece to either your project (in case of app) or your device (in case of hardware), with which you can access all the features of Grandeur Cloud depending upon the scope. Like in case of device reference you can access features limited to device only, while with project reference, you can access all the possible features after user authentication. Checkout [authentication and access][Authentication and Access] section to get more insight into scope.

### Users and Administrator

This topic is about the relationship between you as an administrator and your users. We will describe who can access what so technically we are gonna start talking about scope of access. You create a project and add resources to it like users and devices. So you owns it all. You owns the namespace of a project and only you can manage your project using your account credentials and our dashboard application. But in real world settings, you want your apps to handle a part of your responsibilities. Like obviously you would like your users to automatically sign up. Then you would want your users to automatically access their devices and some data upon authentication. Means you will have to delegate some of your authorities to our SDK, because in the end your apps are gonna communicate to our platform with SDK. You can do this by just giving our SDK your API key and configuring [allowed origins][Allowed Origins] setting through dashboard.

But who are users? Users are just entities that you can either create through dashboard or through SDK. To be more precise, a user entity defines the scope of access of a person who is using your app. By default, a user is limited to access devices paired to his own account, but can access all of your files and datastore (at the moment - you cannot define fine grain control of your files and data stored in storage and datastore respectively. While we are actively working on adding this support to our platform but currently it is very important to develop data model in a way that privacy of users could be protected.). This way a user can request the device logs or his profile picture from our platform upon authentication but cannot access devices paired to another user accounts. But we mean by accessing devices? To read more about it, checkout [device registry][Device Registry] topic.

### Device Registry

Just like users, devices are entities which essentially defines the scope of access. But unlike users, you can only register new devices through dashboard only. This is one of the key features of Grandeur Cloud. We wanted to resolve this issue. There are two types of solutions out there i) those are dealing with users part only e.g. firebase and ii) those are just dealing with devices registry part e.g. Google IoT Core. We wanted to combine best of both worlds. This is why at Grandeur Cloud, where on one hand we have added the authentication feature, then on the other hand we also maintain a devices registry for you. Where you can seamlessly register new entities and pair those entities to users.

Now how this entity defines the scope? Devices are global entities. Unlike user account entity, no body owns a device except an administrator. So a user can pair a device to his account or in order words say a user can claim a device and access data specific to it. But a user cannot delete or inherently modify a device. We designed this considering the real world relation that we develop with our things. So the devices entity in the end defines two things i) what a hardware can access in your namespace and ii) which hardware devices a user can control. Now this really matters a lot because that's the key. You obviously do not want your neighbors to control your air conditioner (that would be so horrible situation). That's what this entity has been designed for. So as mentioned earlier, this entity also limits what a hardware product can access in your namespace. So when you pair a device to an account, we generate an access token for you. Then you can provide this token to our hardware SDK in init in order to access the data (unlike app SDK, hardware SDK can access scope limited to a device only and you delegate a device access to hardware SDK by providing access token at the init. That is how you authenticate your products)

What do we mean by saying that a user can access data of devices paired to his own account only? We have made it simple for you. You won't have to handle data specific to your device yourself using datastore. We do this for you with devices registry. When you register a device to your registry, we create a new namespace for you where we save data in two objects i) Parms and ii) Summary specific to the device you just created. In other words, we maintain a special storage space where you can save data specific to device. It is like a secure contained spot and only accounts to which the device is paired can access it through the SDK, similarly, only the devices with access token of the namespace can access it through hardware SDK.

In the end, let's define what you can store in the two object i) Parms and ii) Summary. To be honest, we are very flexible in it. We just created two objects instead of a single one to help you develop understanding. Our approach is, Parms are like controllable state of a device and Summary is something that device wanted to log or display to apps. In other words, Parms are like the inputs and Summary is like the output of a device. So for example, consider an example where you have a smart light bulb. The parms can be utilized to control the bulb state, while in summary you can log that how much voltage is being consumed by the bulb or any other sensor data. We defined this because we wanted to go a step forward. We wanted to help you in building a logic. But again, we are very flexible. So you can define what should be the initial schema of these two objects but we never monitor that either the schema is being followed or not.

### Authentication and Access

In last two sections, we have discussed in depth that what are the various access scopes and who can access what. This section will revisit this topic again to give you an overall picture of auth and access. Let's start with outlining the relationships. There are three kinds of entities i) projects ii) users and iii) devices. You create a project so own a project and can access all the possible features and data. You do this all by using our dashboard application. End goal is to provide access of the data and devices (that you are building) to your users. You can do this by using our SDK in your apps and hardware. But for this purpose, our SDK should also have access to your namespace. So technically, you can also delegate your access.

This is where the other two entities comes. We designed these two entities to give you fine grain control over what you are delegating and how are you granting access. Users got wider scope of access. A user entity can access devices data (paired to his account), can access the files you stored in storage and can also access the data you maintained in datastore. In other words, this entity allows you to delegate access of your project to your apps through our SDK in a controlled fashion. Or in another way, we can say that the piece of code that you have written in your apps can only make a request to our platform once a user authenticate. Now I believe that you can see the big picture here! You delegate actually delegate some authorities to your users. Now your users can request to authenticate (via our SDK). As a result we will generate an auth token that your hardware can then utilize to communicate to our platform.

Now comes the devices entity. We designed this entity as a global resource. Your users can claim access to a device but cannot actually own it. Whenever a user claims a device, we first verify that an other user hasn't claimed the same device earlier and then generate an access token. The hardware SDK can use this access token to communicate to our platform and access data specific to the device only.

That is the whole picture. That is how we make sure that everyone is getting what they are allowed to access. That is how we grant you authority over your data and resources and keep in check that everything is working just the way you want them to work.

### Networking

Grandeur Cloud is a managed backend as a service platform. We tie it all up for you so you won't have to do it yourself. This is one of the pain points that you face when building your IoT products with services like Google Cloud Platform and AWS. You will have a mix and match services together and tie it all up yourself. But it is always a good practice to fully understand a platform before using it. Therefore we are writing about how the networking works with Grandeur Cloud.

We works with two communication channel in apps SDK i) http based REST API and ii) duplex based realtime API. We use the first REST API channel to do stuff like authentication and file upload, while the other realtime API for fast request transmission like device data requests etc. Realtime channel is really fast and designed to transfer packets to server and from server at transmission rate of 200ms. It is based on our custom protocol and we call it duplex. We don't allow unauthenticated connection over this channel so initially establish communication over REST till authentication.

In the hardware SDK, we use only a single channel; duplex based realtime API. Because of the fact that hardware uses access token to authenticate and which we provide very early on at init. So we do not need a separate channel for authentication and neither we are required to do heavy lifting like file upload. Because while duplex is very fast, it is reliable only for quick messages communication.

### Allowed Origins

This is another amazing topic and somehow related to access delegation in the end. As mentioned earlier in other sections that you can also access your project namespace using SDK by simply providing your API key to the SDK durning init. SDK returns a reference object to the project after initialization which can be then utilized to access the namespace. Which poses a security threat particularly in case of web apps but API key can be easily stolen. While in the end, you require a user to be authenticated first before making any request to platform, a hacker with stolen API key can still cause some damage. Like a hacker can register bogus users to your namespace or can create a copycat site for pishing. To handle this, we introduced cross origin request protection to our platform.

So in order to establish communication from a website using the web SDK, you will have to first whitelist the domain name via settings page in the dashboard application. You cannot even send a request from your localhost without first whitelisting it. Now at this point, it is important to note that whitelisting localhost in production application is not a good practice and should be avoided in order to protect your users.

## Basics of Apollo Device

**Apollo Device** provides a global `apollo` object to interact with all functionalities of the SDK. All `apollo` variables are private and can be accessed and updated using `apollo` getters and setters.

### WiFi Connectivity of the Device

**Apollo Device** takes care of the WiFi connectivity of the device. All you need to do is to provide **WiFi SSID** and **Passphrase** along with **Project's API Key** and **Device's Access Token** through `apollo.init()`, in which case it starts trying to connect to the WiFi and then to Grandeur Cloud right away.

### Connection of Device to Grandeur Cloud

**Apollo Device** takes care of the connection of the device to [Grandeur Cloud][Grandeur Cloud]. All you do is provide your *project's API Key* and your *device's Auth Token* while doing `apollo.init()`. As soon as the WiFi gets connected, **Apollo Device** begins trying to connect to *[Grandeur Cloud][Grandeur Cloud]* using the *API Key* and the *Auth Token*. When it connects, only then can it request the *[cloud][Grandeur Cloud]* to fetch, update any data of the device.

### Callback

`Callback` is a special function type defined in **Apollo Device** that can be passed to another function for the purpose of being called when the running function exists or when some type of event occurs.

**Apollo Device** functions only accept a function as callback which *receives* a `char*` as *input parameter* and *returns* `void`.

```cpp
void callThisWhenDeviceConnectsToGrandeurCloud(char* string) {
  /* Prints this when device makes a successful connection to Grandeur Cloud */
  puts("Device is connected to the cloud.");
}

void setup() {
  /* Initializing apollo configurations
  */
  ApolloDevice device = apollo.init(
    apiKey, authToken, wiFiSsid, wiFiPassphrase
    );
  /* Setting up onApolloConnected callback
  */
  apolloDevice.onApolloConnected(
    callThisWhenDeviceConnectsToGrandeurCloud
    );
}

void loop() {}
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

### Apollo Init

> init ( ) : returns _void_

Method to initialize device's WiFi (SSID and Passphrase) and duplex configurations (API Key and Auth Token) in one go.

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
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  // Updates TCP buffer. It sends connection request to Grandeur Cloud as soon as the WiFi gets connected.
  apolloDevice.update();
}

/***RESULT**
  Device begins connecting to WiFi and then Grandeur Cloud right away.
*/
```

### Update

> update ( ) : returns _char*_

Updates device's TCP buffer. This must be called in `loop()` and without being suspected to any kind of *delay*. This method is what pulls and pushes messages to and from the TCP channel. Any kind of *delay* in `loop()` would cause delay in pushing the request and pulling the response to and from the server.

[Here][Using Millis Instead of Delay] is how you can use `millis()` instead of `delay()` if you want a function to run after every few moments.

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* Calling the update() function in loop()
  */
  apolloDevice.update();  
  /***RESULT**
    Updates the TCP buffer every time loop() gets executed
  */
}
```

### On Apollo Connected

> onApolloConnected (callback : _Callback_) : returns _void_

Receives a function to call when the device successfully connects to Grandeur Cloud.

#### Parameters

| Name        | Type       | Description                                                    |
|-------------|------------|----------------------------------------------------------------|
| callback    | _Callback_ | A function to be called when device connects to Grandeur Cloud |

More on Callback [here][callback].

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);

  /* Passing the callback as lambda function. You can pass any function here as long as it accepts a "char*" string as parameter and returns "void", e.g., a function of the form:

    void callbackFunction(char* string) {}

  */
  apolloDevice.onApolloConnected([]() {
    std::cout<<"Device Connected!\n";
  });
  /***RESULT**
    Prints "Device Connected" on the stdout when device makes a successful connection to Grandeur Cloud
  */
}

void loop() {
  apolloDevice.update();
}
```

### On Apollo Disconnected

> onApolloDisconnected (callback : _Callback_) : returns _void_

Receives a function to call when the device disconnects from Grandeur Cloud.

#### Parameters

| Name        | Type       | Description                                                       |
|-------------|------------|-------------------------------------------------------------------|
| callback    | _Callback_ | A function to be called when device disconnects to Grandeur Cloud |

More on Callback [here][callback].

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);

  /* Passing the callback as lambda function. You can pass any function here as long as it accepts a "char*" string as parameter and returns "void", e.g., a function of the form:

    void callbackFunction(char* string) {}

  */
  apolloDevice.onApolloDisconnected([]() {
    std::cout<<"Device Disconnected!\n";
  });
  /***RESULT**
    Prints "Device Disconnected" on the stdout when device disconnects from Grandeur Cloud
  */
}

void loop() {
  apolloDevice.update();
}
```

### Get State

> getState ( ) returns _int_

Gets the current state of the device. State of the device can be one of the following:

| Macro                 | Expands to   |
|-----------------------|--------------|
| WIFI_NOT_CONNECTED    | 0            |
| WIFI_CONNECTED        | 1            |
| APOLLO_CONNECTED      | 2            |

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
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
  In the beginning, getState() returns 0. When WiFi is connected, getState() returns 1. When the device is connected to the server, getState() returns 2.
  For readability, you can use the above macros. They respectively expand to these integer values.  
*/
```

### Get Stringified State

> getStringifiedState ( ) returns _char*_

Gets the current state of the device as a string.

| Macro                 | Stringified Form      |
|-----------------------|-----------------------|
| WIFI_NOT_CONNECTED    | "WIFI_NOT_CONNECTED"  |
| WIFI_CONNECTED        | "WIFI_CONNECTED"      |
| APOLLO_CONNECTED      | "APOLLO_CONNECTED"    |

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getStringifiedState() returns the state of the device as string. This is particularly useful while you want to directly print the state of the device
  */
  std::cout<<apolloDevice.getStringifiedState()<<"\n";

  apolloDevice.update();
}

/***RESULT**
  In the beginning, "WIFI_NOT_CONNECTED" is printed to the stdout. When the WiFi is connected, "WIFI_CONNECTED" begins appearing on the screen. And it quickly changes to "APOLLO_CONNECTED" when the device makes a successful connection to Grandeur Cloud.
*/
```

### Get SSID

> getSSID ( ) returns _char*_

Gets WiFi SSID currently in use by the device.

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
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

> getPassphrase ( ) returns _char*_

Gets WiFi Passphrase currently in use by the device.

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getPassphrase() returns the Passphrase of the WiFi it is currently configured to connect to.
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
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
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

### Get Api Key

> getApiKey ( ) : returns _char*_

 Gets the [API Key][apikey] currently in use by the device for connecting to *Apollo server*. This method is what receives and pushes messages to and from the duplex channel.

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getApiKey() returns the API Key the device is configured to use to connect to Grandeur Cloud.
  */
  std::cout<<apolloDevice.getApiKey()<<"\n";

  apolloDevice.update();
}

/***RESULT**
  Keeps printing the API Key in loop.
*/
```

### Get Token

> getToken ( ) : returns _char*_

Gets the [access token][token] currently in use by the device for connecting to Grandeur Cloud.

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getToken() returns the Access Token the device is configured to use to connect to Grandeur Cloud.
  */
  std::cout<<apolloDevice.getToken()<<"\n";

  apolloDevice.update();
}

/***RESULT**
  Keeps printing the Access Token in loop.
*/
```

### Get Summary

> getSummary ( ) returns _char*_

Getter method for device's [summary][summary].

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getSummary() sends the getSummary request to the server using the data specified in inputPacket. It calls the Callback function specified in the second parameter when it receives the response.
  */
  char inputPacket[128];
  // Forming a JSON packet
  sprintf(inputPacket, "{\"deviceID\": \"%s\"}", YourDeviceID);
  apolloDevice->getSummary(input, [](unsigned char* outputPacket) {
      std::cout<<outputPacket<<"\n";
  });

  apolloDevice.update();
}

/***RESULT**
  Prints the outputPacket containing the summary (Or just an error code and message in case) of the device as soon as it gets getSummary response from the server
*/
```

### Get Parms  

> getParms ( ) returns _char*_

Getter method for device's [parms][parms].

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* getParms() sends the getParms request to the server using the data specified in inputPacket. It calls the Callback function specified in the second parameter when it receives the response.
  */
  char inputPacket[128];
  // Forming a JSON packet
  sprintf(inputPacket, "{\"deviceID\": \"%s\"}", YourDeviceID);
  apolloDevice->getParms(input, [](unsigned char* outputPacket) {
      std::cout<<outputPacket<<"\n";
  });

  apolloDevice.update();
}

/***RESULT**
  Prints the outputPacket containing the parms (Or just an error code and message in case) of the device as soon as it gets getParms response from the server
*/
```

### Set Summary

> setSummary (summary : _char*_) : returns _void_

Setter method for device's [summary][summary].

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* setSummary() sends the setSummary request to the server using the data specified in inputPacket. It calls the Callback function specified in the second parameter when it receives the response.
  */
  char inputPacket[128];
  // Forming a JSON packet
  sprintf(inputPacket, "{\"deviceID\": \"%s\", \"summary\": {\"alpha\": 10, \"beta\": true}}", YourDeviceID);
  apolloDevice->setSummary(input, [](unsigned char* outputPacket) {
      std::cout<<outputPacket<<"\n";
  });

  apolloDevice.update();
}

/***RESULT**
  Prints the outputPacket containing the SUMMARY-UPDATED code in response (Or an error code and message in case) as soon as it gets it from the server
*/
```

#### Parameters

| Name        | Type       | Description                                                              |
|-------------|------------|--------------------------------------------------------------------------|
| summary     | _char*_    | A string containing a stringified object containing the new summary data |

### Set Parms

> setParms (parms : _char*_) : returns _void_  

Setter method for device's [parms][parms].

#### Parameters

| Name        | Type       | Description                                                              |
|-------------|------------|--------------------------------------------------------------------------|
| parms       | _char*_    | A string containing a stringified object containing the new parms data   |

#### Example

```cpp
void setup() {
  ApolloDevice apolloDevice = apollo.init(YourApiKey, YourToken, YourWiFiSSID, YourWiFiPassphrase);
}

void loop() {
  /* setParms() sends the setParms request to the server using the data specified in inputPacket. It calls the Callback function specified in the second parameter when it receives the response.
  */
  char inputPacket[128];
  // Forming a JSON packet
  sprintf(inputPacket, "{\"deviceID\": \"%s\", \"parms\": {\"charlie\": false}}", YourDeviceID);

  apolloDevice->setParms(input, [](unsigned char* outputPacket) {
      std::cout<<outputPacket<<"\n";
  });

  apolloDevice.update();
}

/***RESULT**
  Prints the outputPacket containing the PARMS-UPDATED code in response (Or an error code and message in case) as soon as it gets it from the server
*/
```

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

[Grandeur Technologies]: https://grandeur.tech "Grandeur Technologies"
[Grandeur Cloud]: https://cloud.grandeur.tech "Grandeur Cloud"
[Grandeur Cloud Sign Up]: https://cloud.grandeur.tech/register "Sign up on Grandeur Cloud"
[Get Started With Grandeur Cloud]: https://github.com/grandeurtech/grandeurcloud-js-sdk#get-started "Get Started With Grandeur Cloud"
[Grandeur Cloud Pricing]: https://grandeur.tech/pricing "Pricing"

[SolDrive]: https://sol-drive.com/ "SolDrive"

[Authentication and Access]: #authentication-and-access "Authentication and Access"
[Allowed Origins]: #allowed-origins "Allowed Origins"
[Device Registry]: #device-registry "Device Registry"

[Get Started with Hardware SDK]: #get-started "Get Started with Hardware SDK"
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

[Using Millis Instead of Delay]: https://www.norwegiancreations.com/2017/09/arduino-tutorial-using-millis-instead-of-delay/ "Using millis() instead of delay()"