/**
 * @file Duplex.h
 * @date 25.07.2019
 * @author Moiz Husnain
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Duplex for Arduino.
 *
 */

#ifndef DUPLEX_H_
#define DUPLEX_H_

#ifdef STM32_DEVICE
#include <application.h>
#define bit(b) (1UL << (b))    // Taken directly from Arduino.h
#else
#include <Arduino.h>
#include <IPAddress.h>
#endif

#ifdef ARDUINO_ARCH_AVR
#error Version 2.x.x currently does not support Arduino with AVR since there is no support for std namespace of c++.
#error Use Version 1.x.x. (ATmega branch)
#else
#include <functional>
#endif

#define DEBUG_ESP_PORT Serial

#ifndef NODEBUG_DUPLEX
#ifdef DEBUG_ESP_PORT
#define DEBUG_DUPLEX(...) DEBUG_ESP_PORT.printf(__VA_ARGS__)
#else
//#define DEBUG_DUPLEX(...) os_printf( __VA_ARGS__ )
#endif
#endif

#ifndef DEBUG_DUPLEX
#define DEBUG_DUPLEX(...)
#define NODEBUG_DUPLEX
#endif

#if defined(ESP8266) || defined(ESP32)

#define DUPLEX_MAX_DATA_SIZE (15 * 1024)
#define DUPLEX_USE_BIG_MEM
#define GET_FREE_HEAP ESP.getFreeHeap()
// moves all Header strings to Flash (~300 Byte)
//#define DUPLEX_SAVE_RAM

#elif defined(STM32_DEVICE)

#define DUPLEX_MAX_DATA_SIZE (15 * 1024)
#define DUPLEX_USE_BIG_MEM
#define GET_FREE_HEAP System.freeMemory()

#else

//atmega328p has only 2KB ram!
#define DUPLEX_MAX_DATA_SIZE (1024)
// moves all Header strings to Flash
#define DUPLEX_SAVE_RAM

#endif

#define DUPLEX_TCP_TIMEOUT (2000)

#define NETWORK_ESP8266_ASYNC (0)
#define NETWORK_ESP8266 (1)
#define NETWORK_W5100 (2)
#define NETWORK_ENC28J60 (3)
#define NETWORK_ESP32 (4)
#define NETWORK_ESP32_ETH (5)

// max size of the WS Message Header
#define DUPLEX_MAX_HEADER_SIZE (14)

#if !defined(DUPLEX_NETWORK_TYPE)
// select Network type based
#if defined(ESP8266) || defined(ESP31B)
#define DUPLEX_NETWORK_TYPE NETWORK_ESP8266
//#define DUPLEX_NETWORK_TYPE NETWORK_ESP8266_ASYNC
//#define DUPLEX_NETWORK_TYPE NETWORK_W5100

#elif defined(ESP32)
#define DUPLEX_NETWORK_TYPE NETWORK_ESP32
//#define DUPLEX_NETWORK_TYPE NETWORK_ESP32_ETH
#else
#define DUPLEX_NETWORK_TYPE NETWORK_W5100

#endif
#endif

// Includes and defined based on Network Type
#if(DUPLEX_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)

// Note:
//   No SSL/WSS support for client in Async mode
//   TLS lib need a sync interface!

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <WiFiClientSecure.h>
#elif defined(ESP31B)
#include <ESP31BWiFi.h>
#else
#error "network type ESP8266 ASYNC only possible on the ESP mcu!"
#endif

#include <ESPAsyncTCP.h>
#include <ESPAsyncTCPbuffer.h>
#define DUPLEX_NETWORK_CLASS AsyncTCPbuffer
#define DUPLEX_NETWORK_SERVER_CLASS AsyncServer

#elif(DUPLEX_NETWORK_TYPE == NETWORK_ESP8266)

#if !defined(ESP8266) && !defined(ESP31B)
#error "network type ESP8266 only possible on the ESP mcu!"
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <ESP31BWiFi.h>
#endif
#define DUPLEX_NETWORK_CLASS WiFiClient
#define DUPLEX_NETWORK_SSL_CLASS WiFiClientSecure
#define DUPLEX_NETWORK_SERVER_CLASS WiFiServer

#elif(DUPLEX_NETWORK_TYPE == NETWORK_W5100)

#ifdef STM32_DEVICE
#define DUPLEX_NETWORK_CLASS TCPClient
#define DUPLEX_NETWORK_SERVER_CLASS TCPServer
#else
#include <Ethernet.h>
#include <SPI.h>
#define DUPLEX_NETWORK_CLASS EthernetClient
#define DUPLEX_NETWORK_SERVER_CLASS EthernetServer
#endif

#elif(DUPLEX_NETWORK_TYPE == NETWORK_ENC28J60)

#include <UIPEthernet.h>
#define DUPLEX_NETWORK_CLASS UIPClient
#define DUPLEX_NETWORK_SERVER_CLASS UIPServer

#elif(DUPLEX_NETWORK_TYPE == NETWORK_ESP32)

#include <WiFi.h>
#include <WiFiClientSecure.h>
#define DUPLEX_NETWORK_CLASS WiFiClient
#define DUPLEX_NETWORK_SSL_CLASS WiFiClientSecure
#define DUPLEX_NETWORK_SERVER_CLASS WiFiServer

#elif(DUPLEX_NETWORK_TYPE == NETWORK_ESP32_ETH)

#include <ETH.h>
#define DUPLEX_NETWORK_CLASS WiFiClient
#define DUPLEX_NETWORK_SERVER_CLASS WiFiServer

#else
#error "no network type selected!"
#endif

#ifdef DUPLEX_NETWORK_SSL_CLASS
#define HAS_SSL
#endif

// moves all Header strings to Flash (~300 Byte)
#ifdef DUPLEX_SAVE_RAM
#define DUPLEX_STRING(var) F(var)
#else
#define DUPLEX_STRING(var) var
#endif

typedef enum {
    WSC_NOT_CONNECTED,
    WSC_HEADER,
    WSC_CONNECTED
} WSclientsStatus_t;

typedef enum {
    WStype_ERROR,
    WStype_DISCONNECTED,
    WStype_CONNECTED,
    WStype_TEXT,
    WStype_BIN,
    WStype_FRAGMENT_TEXT_START,
    WStype_FRAGMENT_BIN_START,
    WStype_FRAGMENT,
    WStype_FRAGMENT_FIN,
    WStype_PING,
    WStype_PONG,
} WStype_t;

typedef enum {
    WSop_continuation = 0x00,    ///< %x0 denotes a continuation frame
    WSop_text         = 0x01,    ///< %x1 denotes a text frame
    WSop_binary       = 0x02,    ///< %x2 denotes a binary frame
                                 ///< %x3-7 are reserved for further non-control frames
    WSop_close = 0x08,           ///< %x8 denotes a connection close
    WSop_ping  = 0x09,           ///< %x9 denotes a ping
    WSop_pong  = 0x0A            ///< %xA denotes a pong
                                 ///< %xB-F are reserved for further control frames
} WSopcode_t;

typedef struct {
    bool fin;
    bool rsv1;
    bool rsv2;
    bool rsv3;

    WSopcode_t opCode;
    bool mask;

    size_t payloadLen;

    uint8_t * maskKey;
} WSMessageHeader_t;

typedef struct {
    uint8_t num;    ///< connection number

    WSclientsStatus_t status;

    DUPLEX_NETWORK_CLASS * tcp;

    bool isSocketIO;    ///< client for socket.io server

#if defined(HAS_SSL)
    bool isSSL;    ///< run in ssl mode
    DUPLEX_NETWORK_SSL_CLASS * ssl;
#endif

    String cUrl;       ///< http url
    uint16_t cCode;    ///< http code

    bool cIsClient = false;    ///< will be used for masking
    bool cIsUpgrade;           ///< Connection == Upgrade
    bool cIsDuplex;         ///< Upgrade == Duplex

    String cSessionId;     ///< client Set-Cookie (session id)
    String cKey;           ///< client Sec-Duplex-Key
    String cAccept;        ///< client Sec-Duplex-Accept
    String cProtocol;      ///< client Sec-Duplex-Protocol
    String cExtensions;    ///< client Sec-Duplex-Extensions
    uint16_t cVersion;     ///< client Sec-Duplex-Version

    uint8_t cWsRXsize;                                ///< State of the RX
    uint8_t cWsHeader[DUPLEX_MAX_HEADER_SIZE];    ///< RX WS Message buffer
    WSMessageHeader_t cWsHeaderDecode;

    String base64Authorization;    ///< Base64 encoded Auth request
    String plainAuthorization;     ///< Base64 encoded Auth request

    String extraHeaders;

    bool cHttpHeadersValid;           ///< non-Duplex http header validity indicator
    size_t cMandatoryHeadersCount;    ///< non-Duplex mandatory http headers present count

    bool pongReceived;
    uint32_t pingInterval;             // how often ping will be sent, 0 means "heartbeat is not active"
    uint32_t lastPing;                 // millis when last pong has been received
    uint32_t pongTimeout;              // interval in millis after which pong is considered to timeout
    uint8_t disconnectTimeoutCount;    // after how many subsequent pong timeouts discconnect will happen, 0 means "do not disconnect"
    uint8_t pongTimeoutCount;          // current pong timeout count

#if(DUPLEX_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
    String cHttpLine;    ///< HTTP header lines
#endif

} WSclient_t;

class Duplex {
  protected:
#ifdef __AVR__
    typedef void (*WSreadWaitCb)(WSclient_t * client, bool ok);
#else
    typedef std::function<void(WSclient_t * client, bool ok)> WSreadWaitCb;
#endif

    virtual void clientDisconnect(WSclient_t * client)  = 0;
    virtual bool clientIsConnected(WSclient_t * client) = 0;

    void clientDisconnect(WSclient_t * client, uint16_t code, char * reason = NULL, size_t reasonLen = 0);

    virtual void messageReceived(WSclient_t * client, WSopcode_t opcode, uint8_t * payload, size_t length, bool fin) = 0;

    uint8_t createHeader(uint8_t * buf, WSopcode_t opcode, size_t length, bool mask, uint8_t maskKey[4], bool fin);
    bool sendFrameHeader(WSclient_t * client, WSopcode_t opcode, size_t length = 0, bool fin = true);
    bool sendFrame(WSclient_t * client, WSopcode_t opcode, uint8_t * payload = NULL, size_t length = 0, bool fin = true, bool headerToPayload = false);

    void headerDone(WSclient_t * client);

    void handleDuplex(WSclient_t * client);

    bool handleDuplexWaitFor(WSclient_t * client, size_t size);
    void handleDuplexCb(WSclient_t * client);
    void handleDuplexPayloadCb(WSclient_t * client, bool ok, uint8_t * payload);

    String acceptKey(String & clientKey);
    String base64_encode(uint8_t * data, size_t length);

    bool readCb(WSclient_t * client, uint8_t * out, size_t n, WSreadWaitCb cb);
    virtual size_t write(WSclient_t * client, uint8_t * out, size_t n);
    size_t write(WSclient_t * client, const char * out);

    void enableHeartbeat(WSclient_t * client, uint32_t pingInterval, uint32_t pongTimeout, uint8_t disconnectTimeoutCount);
    void handleHBTimeout(WSclient_t * client);
};

#ifndef UNUSED
#define UNUSED(var) (void)(var)
#endif
#endif /* DUPLEX_H_ */
