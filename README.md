# Grandeur Cloud [![Version](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://cloud.grandeur.tech)

Building a smart (IoT) product is an art because it is about unifying the physical world with the digital one. When you connect a hardware device to the web, magic happens. But it involves development across a huge technology stack (you need to develop your hardware, your apps to monitor/control your hardware and a server backend to manage both) to make such products work in production. Then if you are (somehow) done with the development operations, there comes the hardest part; you are going to have to scale it all as your user base grows.

We understand this because we have been there.

Introducing Grandeur Cloud: A backend as a service (Baas) platform for IoT. We have designed this platform so you do not have to worry about the backend of your next big thing, and can focus on what matters the most: your hardware and your apps. It is designed specifically to accelerate your IoT product development and push your product to market in weeks rather than in months or years.

## Why Grandeur Cloud

Grandeur Cloud is designed keeping in mind all the challenges a hardware engineer can face in developing and commercializing a smart (IoT) product. You can use out-of-the-box APIs to integrate your devices and apps.

Like you can do authentication of users who use your products, to make sure each user has access to his own data and no one other than the device admin himself should be able to interact with a device. You can save a huge amount of data in a cloud database to extract intelligent information and display it in useful graphs. You can also host your product's static website on Grandeur Cloud. Also, your hardware device can subscribe to events from your app and your app can subscribe to events from your hardware device and communicate data with each other in realtime.

Here are some points that make Grandeur Cloud stand out from the rest of the IoT solutions ou there.

* Grandeur Cloud is product-centered. It is much more than just a medium of communication between your app and your hardware. Focusing on expediting IoT product development, it offers an ecosystem of the most necessary tools integrated to make the most head-cracking development problems seamless.

* It is extremely simple to [get started][Get Started with Grandeur Cloud] with your IoT product development. Just create a project at [cloud dashboard][Grandeur Cloud Dashboard], put in your project's API key into the SDK and start developing.

* No need to mix and match various services. Grandeur Cloud is a single spot solution for all of your needs. It has built-in authentication of your users and devices, an integrated database, an out-of-the-box file storage system and a registry of data for all of your devices. You can manage everything from a dashboard and integrate it into your product through our SDKs.

* Simple pricing. [Start free][Grandeur Cloud Sign Up] and then pay as you go based on your resource consumption. Checkout [pricing][Grandeur Cloud Pricing] for more details.

By this time you would be like, okay! we got what motivated Grandeur to build this platform and yeah it is super great and super useful too. But how can we integrate it into our product? So here we have built these amazing SDKs for your apps and hardware to make the integration of Grandeur Cloud in your product quick and seamless. So dive straight in.

[Here][Get Started With Grandeur Cloud] is how you can create a new project on Grandeur Cloud and start using the Javascript SDK to build your IoT apps.

From here onwards, we'll look at how you can use the Hardware SDK to put your devices live and connected on Grandeur Cloud. Let's go!

# Hardware SDK

**Hardware SDK** is the official SDK for hardware that utilizes the *Apollo* API to connect your device to **[Grandeur Cloud][Grandeur Cloud]**.

***NOTE***: Currently, **Apollo Device** SDK is available only for esp8266. Support for the following microcontrollers is coming very soon:

* esp32

Follow the [get started][Get Started with Hardware SDK] guidelines to quickly get a broad context on integrating your devices to Grandeur Cloud or jump straight to an [Arduino example][Example] and make your hands dirty.

To get deeper insights and a developer reference for the hardware SDK, you can have a look at the [documentation][Documentation].

To understand the core concepts Grandeur Cloud is built upon, simply dive into the "[A Peek into Grandeur Ecosystem][Ecosystem]" section.

* [Get Started](#get-started)
  * [Installation](#installation)
  * [Initialization](#initialization)
  * [Device State](#device-state)
  * [Updating Device TCP Buffer](#updating-device-tcp-buffer)
  * [Event Listening](#event-listening)
* [Example](#example)
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

* [A peek into Grandeur Ecosystem](#A-Peek-into-Grandeur's-Ecosystem)

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

You can also attach connection-related event listeners to it. For example, to run a piece of code when the device makes a successful connection to Grandeur Cloud, just wrap that piece of code in a function and pass the function to `apolloDevice.onApolloConnected()`.

Similarly, you can pass a function as a parameter to `apolloDevice.onApolloDisconnected()` and it will be called when the device disconnects from the cloud.

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

# Grandeur Ecosystem

The purpose of writing this is to give you a glimpse into the thought process and psychologies behind designing the Grandeur Cloud Platform the way it feels now. We believe that the first very important step toward choosing a platform for your product and company is to understand the design language of developers of the platform. So we thought of writing about it in detail. We wanted to document how you can use this platform effectively to make your life as a developer or founder a bit simpler.

Here we present a case study to help you understand exactly where, how and what Grandeur Cloud can help you with. Then we explain some of the terms and keywords we use to identify and classify things that make abstraction seamless. Here we go.

## A Brief Case Study

Suppose you are a cleantech startup and want to radicalize the home appliances market to make the appliances more eco and user friendly. You analyzed the market, did user interviews and realized that there is a really big problem in the air conditioner market. Even though millions of new air conditioners are produced every year but there are so many old and inefficient ACs already in the market installed in people's homes and offices. These old air conditioners consume a huge chunk of power and are a major cause of CFCs emissions. Something has to be done because these devices are impacting both the users and the ecosystem. Upgrading each single one of them is just not feasible at all economically.

To resolve the energy efficiency issue of these old ACs, you decided to build an electronic solution that could be used as an extension with these old ACs. So we could control their power consumption without actually upgrading them. And you thought of providing your users with an interface to interact with their appliances. You wanted your users to see how much has this new extension been saving them in expenses by cutting down the power consumption. You also wanted to give your users control over how much they wanted to save through this app. In short, you decided to make your product smart (on IoT). And you decided to build a companion app for your device.

That's where the problem arose. You are a hardware startup, after all, that builds amazing electronics technology. But here you got to deal with a few more things as well. You have to build your app and figure out how to establish its communication with your hardware product. You may decide to hire more engineers. But do you know how much of them will you have to hire? To give you a perspective, you generally need 8+ engineers just to do the server-end, like one to figure out your networking, one to design and manage your database, one to develop your API (the interface layer between your users and devices), about four for building your SDKs (one for each platform android, ios, web, and hardware) and then when you start scaling up a bit, one DevOps engineer. This makes it a package of $8000+ just to figure out the backend of your system and you haven't even validated your product yet. This is exhausting for your business. You have hit a concrete wall with no idea what to do about it.

Then one day the sun of fate shown. You came across a platform that goes by the name of Grandeur Cloud. You went through its [website][Grandeur Technologies] and discovered a perfectly fitting solution for all your headaches. You wanted a solution for authentication of your users, it has the Auth feature in it. You needed online file storage to store maybe the profile pictures of your users and other stuff, it comes with a storage builtin. You were in dire need of a scalable out-of-the-box database to store power consumption logs of your device to show your users graphs on their monthly/yearly savings, it provides a cloud datastore service. And the most important of these all, you needed a realtime communication bridge between your hardware and your apps, THANK GOD, its SDKs are available for all the stacks including Arduino, web, and mobile (both android and ios).

So here you are giving it a shot. You simply [registered for the platform][Grandeur Cloud], created your first project, downloaded their SDKs and started connecting your devices and apps through Grandeur Cloud. You didn't even have to worry about the security of your users and devices, because the data on Grandeur Cloud is protected under standard security protocols. Each user and device in a project is limited in its scope. All you had to worry about was designing your product core and develop your business logic. And in a matter of weeks, your product was out in people's hands, your apps live on app stores. People loved what you built and you were getting live feedback on it. You could see how many people have paired with your devices. You made an early entry into the market and now adding a dent to the universe.

By the way, that is the story of team SolDrive. Check out their [website][SolDrive] right now and explore how they are transforming the world with Grandeur Cloud.

## Concepts

In this subsection, we will explore the Grandeur Cloud Platform in detail. We will see how it all works in depth. So let's get started.

### Projects

A project is the first thing you need to create to start working with Grandeur Cloud. A project is like a namespace, a completely isolated network of users and devices. Each project also has separate file storage and a separate datastore. While you can create an unlimited number of projects, but no two projects can interact or share anything with one other.

Each project is identified by a digital signature that we call the API key, just as your identification card or social security number identifies you as a citizen. To connect your apps or hardware to your project's network, this is what you need to provide to the SDKs. The API key is sent with every request to Grandeur Cloud and this is what defines the scope or namespace of the request. Check out the [SDK][SDK] section to read more about it.

*NOTE*: Our pricing applies separately to each project. So you get a free tier on every project and pay for each separately for what you consume when you cross your resources limit.

### SDK

Apollo is the API that exposes Grandeur Cloud to the outside world. Our SDKs utilize this API and provide each functionality through a function. We have tried our best to make the integration of our SDKs into your codebase simple. For example, while developing your web app, you simply drop in the link of JS SDK CDN in your codebase and you are done. We have developed our SDKs for each platform in coherence with each other so you could work and collaborate everywhere seamlessly.

This is how it works. In each SDK, there is a global object `apollo`. You can initialize your configurations (API Key and a couple of more stuff in case of hardware SDK) by calling `apollo.init()`. This returns you a reference to your whole project (in case of your app) or just to your device (in case of hardware because hardware scope is limited to the hardware itself). In **JS SDK**, you can interact with the authentication API, the device API, the file storage and the datastore API. In the case of **Hardware SDK** your scope is limited to just the device's data. Check out the [Authentication and Access][Authentication and Access] section to get more insight into how scope affects your development across the different platforms.

### Users and Administrator

This topic is about the relationship between you as an administrator and your users. We describe here the scope of access of you and your users.

You create a project and add some resources (users and devices) to it. You are the one who owns the project as its administrator and only you can manage it through the dashboard application after signing in. But in real world settings, you want your apps to handle a part of your responsibilities. Like obviously you would like your users to automatically sign up. Then you would want your users to automatically access their devices and some data upon authentication. Means you will have to delegate some of your authorities to our SDK, because in the end your apps are gonna communicate to our platform with SDK. You can do this by just giving our SDK your API key and configuring [allowed origins][Allowed Origins] setting through dashboard.

But who are users? Users are just entities that you can either create through the dashboard or SDK. To be more precise, a user entity defines the scope of access of a person who is using your app. By default, a user is limited to access devices paired to his own account, but can access all of your files and datastore (at the moment - you cannot define fine grain control of your files and data stored in storage and datastore respectively. While we are actively working on adding this support to our platform but currently it is very important to develop data model in a way that privacy of users could be protected.). This way a user can request the device logs or his profile picture from our platform upon authentication but cannot access devices paired to another user accounts. But we mean by accessing devices? To read more about it, checkout [device registry][Device Registry] topic.

### Device Registry

Just like users, devices are entities which essentially defines the scope of access. But unlike users, you can only register new devices through dashboard only. This is one of the key features of Grandeur Cloud. We wanted to resolve this issue. There are two types of solutions out there i) those are dealing with users part only e.g. firebase and ii) those are just dealing with devices registry part e.g. Google IoT Core. We wanted to combine best of both worlds. This is why at Grandeur Cloud, where on one hand we have added the authentication feature, then on the other hand we also maintain a devices registry for you. Where you can seamlessly register new entities and pair those entities to users.

Now how this entity defines the scope? Devices are global entities. Unlike user account entity, no body owns a device except an administrator. So a user can pair a device to his account or in order words say a user can claim a device and access data specific to it. But a user cannot delete or inherently modify a device. We designed this considering the real world relation that we develop with our things. So the devices entity in the end defines two things i) what a hardware can access in your namespace and ii) which hardware devices a user can control. Now this really matters a lot because that's the key. You obviously do not want your neighbors to control your air conditioner (that would be so horrible situation). That's what this entity has been designed for. So as mentioned earlier, this entity also limits what a hardware product can access in your namespace. So when you pair a device to an account, we generate an access token for you. Then you can provide this token to our hardware SDK in init in order to access the data (unlike app SDK, hardware SDK can access scope limited to a device only and you delegate a device access to hardware SDK by providing access token at the init. That is how you authenticate your products)

What do we mean by saying that a user can access data of devices paired to his own account only? We have made it simple for you. You won't have to handle data specific to your device yourself using datastore. We do this for you with devices registry. When you register a device to your registry, we create a new namespace for you where we save data in two objects i) Parms and ii) Summary specific to the device you just created. In other words, we maintain a special storage space where you can save data specific to device. It is like a secure contained spot and only accounts to which the device is paired can access it through the SDK, similarly, only the devices with access token of the namespace can access it through hardware SDK.

In the end, let's define what you can store in the two object i) Parms and ii) Summary. To be honest, we are very flexible in it. We just created two objects instead of a single one to help you develop understanding. Our approach is, Parms are like controllable state of a device and Summary is something that device wanted to log or display to apps. In other words, Parms are like the inputs and Summary is like the output of a device. So for example, consider an example where you have a smart light bulb. The parms can be utilized to control the bulb state, while in summary you can log that how much voltage is being consumed by the bulb or any other sensor data. We defined this because we wanted to go a step forward. We wanted to help you in building a logic. But again, we are very flexible. So you can define what should be the initial schema of these two objects but we never monitor that either the schema is being followed or not.

### Authentication and Access

In the last two sections, we have discussed in depth about who can access what. This section will revisit the topic and gives you a big picture of auth and access scopes. Let's start by outlining the relationships. There are three kinds of entities i) projects ii) users and iii) devices. You create a project and therefore have a global access of everything. You can access and manage it by using the dashboard application. End goal is to provide access of the data and devices (that you are building) to your users. You can do this by using our SDK in your apps and hardware. But for this purpose, our SDK should also have access to your namespace. So technically, you can also delegate your access.

This is where the other two entities comes. We designed these two entities to give you fine grain control over what you are delegating and how are you granting access. Users got wider scope of access. A user entity can access devices data (paired to his account), can access the files you stored in storage and can also access the data you maintained in datastore. In other words, this entity allows you to delegate access of your project to your apps through our SDK in a controlled fashion. Or in another way, we can say that the piece of code that you have written in your apps can only make a request to our platform once a user authenticate. Now I believe that you can see the big picture here! You delegate actually delegate some authorities to your users. Now your users can request to authenticate (via our SDK). As a result we will generate an auth token that your hardware can then utilize to communicate to our platform.

Now comes the devices entity. We designed this entity as a global resource. Your users can claim access to a device but cannot actually own it. Whenever a user claims a device, we first verify that an other user hasn't claimed the same device earlier and then generate an access token. The hardware SDK can use this access token to communicate to our platform and access data specific to the device only.

That is the whole picture. That is how we make sure that everyone is getting what they are allowed to access. That is how we grant you authority over your data and resources and keep in check that everything is working just the way you want them to work.

### Networking

Here we write about how the networking works on Grandeur Cloud.

We work with two communication channels in **Web SDK** i) HTTP based REST API channel and ii) Duplex based realtime API channel. We use the first to do some basic or heavy stuff like authentication or file uploading, while the latter, as its name suggests, for realtime communication like fetching or updating the device's data. The realtime channel is as fast as 200ms RTT. It is based on our custom protocol aka. Duplex. We do not allow any unauthenticated communication over this channel and therefore authenticate a connection over the REST channel first.

In the **Hardware SDK**, we only use the realtime channel. A device cannot establish a connection over this channel unless and until its access token is validated. A device access token is provided while initializing the apollo configurations through `apollo.init()`.

### Allowed Origins

This is another amazing topic and somehow related to access delegation in the end. As mentioned in the sections above that you can interact with your project's namespace through the JS SDK by initializing apollo with your API key. This returns an object referring to your project and can be used to access its namespace including its devices, datastore, and files storage. Putting this much responsibility on just the API key poses a security threat particularly in case of web apps as API Key can easily be stolen. Even though a user needs to log in first before making any request to the cloud, a hacker with having your API key can still cause some serious damage. For example, Registering bogus users to your project or creating a copycat site on your name for phishing to name a few. That's where cross-origin request sharing (CORS) policies come to play.

So to allow a web app to interact with your project using the Web SDK, you first need to whitelist the domain name your web app uses via the settings page in your dashboard. You cannot even send a request from your localhost without first whitelisting it.

*NOTE*: Keeping localhost whitelisted even in a production application is a very serious vulnerability. You should avoid that at all costs.

## The Dexterity of Hardware SDK

What makes the hardware SDK so simple and dextrous is its way of making your code look very well structured. It takes the burden of so many things off your shoulders and makes asynchronous tasks easy to handle. For example,

* **Hardware SDK** takes care of the device's WiFi connectivity by itself. It starts trying to connect to the WiFi as soon as you do `apollo.init()`.

* **Hardware SDK** also takes care of the device's connection to [Grandeur Cloud][Grandeur Cloud]. As soon as the WiFi gets connected, **Hardware SDK** begins trying to connect to *[Grandeur Cloud][Grandeur Cloud]* using the *API Key* and the *Access Token* you provide during `apollo.init()`. When it connects, only then can it request the *[cloud][Grandeur Cloud]* to fetch or update any data of the device.

* And as the device quickly flies through these states, i.e.,
  * Device is not connected to WiFi.
  * Device is connected to WiFi, but not connected to Grandeur Cloud yet.
  * Device is finally connected to Grandeur Cloud.

  **Hardware SDK** exposes the state of the device through [`getState()`][getState] function to let you make your decisions based on that.

* Not just that, you can even set event handlers on device's connection and disconnection to Grandeur Cloud using [`onApolloConnected()`][onApolloConnected] and [`onApolloDisconnected()`][onApolloDisconnected].

* There is a special type of function defined in **Hardware SDK** as [`Callback`][callback] that can be passed to some functions of `ApolloDevice` class as a parameter. This callback function is called when the running function exists or when some type of event occurs.

To see the **Hardware SDK** in action, jump to the [Get Started][Get Started with Hardware SDK] section.

```cpp
ApolloDevice apolloDevice;
void callThisWhenDeviceConnectsToGrandeurCloud(unsigned char* string) {
  /* Prints this when device makes a successful connection to Grandeur Cloud */
  puts("Device is connected to the cloud.");
}

void setup() {
  /* Initializing apollo configurations
  */
  apolloDevice = apollo.init(YourApiKey, YourAccessToken, YourWiFiSSID, YourWiFiPassphrase);
  /* Setting up onApolloConnected callback
  */
  apolloDevice.onApolloConnected(callThisWhenDeviceConnectsToGrandeurCloud);
}

void loop() {
  /* Updating the device's TCP buffer
  */
  apolloDevice.update()
}

/***RESULT**
 * Prints "Device is connected to the cloud." to the stdout when the device makes
 * a successful connection to Grandeur Cloud.
*/
```

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

## A Peek into Grandeur's Ecosystem

### Project

A *project* is the most top-level entity in **[Grandeur Cloud][Grandeur Cloud]**. It defines an isolated ecosystem for all your nodes (Users and devices), which means no node in one project can be related to or interact with any node of some other project.

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

A *consumer* is the end user that uses the *user*'s product. It lives in the ecosystem aka project created by the *user* and interacts with other nodes (devices and other consumers) of the *[project][project]*.

### Device

A *device* is the hardware product that a *consumer* can monitor and control. To be precise, there are some *device* variables that a *[consumer][consumer]* actually interacts with. These interactive variables are specified under *device*'s *[summary][summary]* and *[parms][parms]*.

All the nodes in a *project* interact with each other through *[Apollo server][apollo server]*.

### Apollo Server

*Apollo server* is the central hub for all the communications happening among all the *[consumers][consumer]* and all the *[devices][device]* in all the *[projects][project]*. *Apollo server* is what isolates *projects* from each other, maintains *[duplex connection][duplex]* with all nodes, and routes messages among them.

Each node communicates with the other through a realtime *duplex* channel.

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

*Summary* includes those device variables which *are not directly controllable*. For example, in an air conditioner, the controllable variable is its state (ON/OFF) or the temperature dial you see on its display, while its voltage, current, and power consumption would be non-controllable variables, thus opted to be under *summary*.

### Device Parms

*Parms* are *the directly controllable* variables. In the previous air conditioner example, its state and the temperature dial would opt for *parms* category.

[Grandeur Technologies]: https://grandeur.tech "Grandeur Technologies"
[Grandeur Cloud]: https://cloud.grandeur.tech "Grandeur Cloud"
[Grandeur Cloud Sign Up]: https://cloud.grandeur.tech/register "Sign up on Grandeur Cloud"
[Get Started With Grandeur Cloud]: https://github.com/grandeurtech/grandeurcloud-js-sdk#get-started "Get Started With Grandeur Cloud"
[Grandeur Cloud Pricing]: https://grandeur.tech/pricing "Pricing"

[Documentation]: #documentation "Documentation"
[Ecosystem]: #grandeur-ecosystem "Grandeur Ecosystem"

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

[getState]: #get-state
[onApolloConnected]: #on-apollo-connected
[onApolloDisconnected]: #on-apollo-disconnected

[Using Millis Instead of Delay]: https://www.norwegiancreations.com/2017/09/arduino-tutorial-using-millis-instead-of-delay/ "Using millis() instead of delay()"
