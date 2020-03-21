/**
 * @file DuplexClient.h
 * @date 25.07.2019
 * @author Moiz Husnain
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Duplex for Arduino.
 *
 */

#ifndef DUPLEXCLIENT_H_
#define DUPLEXCLIENT_H_

#include <Duplex.h>

class DuplexClient: protected Duplex {
  public:
#ifdef __AVR__
    typedef void (*DuplexClientEvent)(WStype_t type, uint8_t * payload, size_t length);
#else
    typedef std::function<void(WStype_t type, uint8_t * payload, size_t length)> DuplexClientEvent;
#endif

    DuplexClient(void);
    virtual ~DuplexClient(void);

    void begin(const char * host, uint16_t port, const char * url = "/", const char * protocol = "arduino");
    void begin(String host, uint16_t port, String url = "/", String protocol = "arduino");
    void begin(IPAddress host, uint16_t port, const char * url = "/", const char * protocol = "arduino");

#if defined(HAS_SSL)
    void beginSSL(const char * host, uint16_t port, const char * url = "/", const char * = "", const char * protocol = "arduino");
    void beginSSL(String host, uint16_t port, String url = "/", String fingerprint = "", String protocol = "arduino");
    void beginSslWithCA(const char * host, uint16_t port, const char * url = "/", const char * CA_cert = NULL, const char * protocol = "arduino");
#endif

    void beginSocketIO(const char * host, uint16_t port, const char * url = "/socket.io/?EIO=3", const char * protocol = "arduino");
    void beginSocketIO(String host, uint16_t port, String url = "/socket.io/?EIO=3", String protocol = "arduino");

#if defined(HAS_SSL)
    void beginSocketIOSSL(const char * host, uint16_t port, const char * url = "/socket.io/?EIO=3", const char * protocol = "arduino");
    void beginSocketIOSSL(String host, uint16_t port, String url = "/socket.io/?EIO=3", String protocol = "arduino");
    void beginSocketIOSSLWithCA(const char * host, uint16_t port, const char * url = "/socket.io/?EIO=3", const char * CA_cert = NULL, const char * protocol = "arduino");
#endif

#if(DUPLEX_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    void loop(void);
#else
    // Async interface not need a loop call
    void loop(void) __attribute__((deprecated)) {}
#endif

    void onEvent(DuplexClientEvent cbEvent);

    bool sendTXT(uint8_t * payload, size_t length = 0, bool headerToPayload = false);
    bool sendTXT(const uint8_t * payload, size_t length = 0);
    bool sendTXT(char * payload, size_t length = 0, bool headerToPayload = false);
    bool sendTXT(const char * payload, size_t length = 0);
    bool sendTXT(String & payload);
    bool sendTXT(char payload);

    bool sendBIN(uint8_t * payload, size_t length, bool headerToPayload = false);
    bool sendBIN(const uint8_t * payload, size_t length);

    bool sendPing(uint8_t * payload = NULL, size_t length = 0);
    bool sendPing(String & payload);

    void disconnect(void);

    void setAuthorization(const char * user, const char * password);
    void setAuthorization(const char * auth);

    void setExtraHeaders(const char * extraHeaders = NULL);

    void setReconnectInterval(unsigned long time);

    void enableHeartbeat(uint32_t pingInterval, uint32_t pongTimeout, uint8_t disconnectTimeoutCount);
    void disableHeartbeat();

  protected:
    String _host;
    uint16_t _port;

    bool isConnected(void);

#if defined(HAS_SSL)
    String _fingerprint;
    const char * _CA_cert;
#endif
    WSclient_t _client;

    DuplexClientEvent _cbEvent;

    unsigned long _lastConnectionFail;
    unsigned long _reconnectInterval;

    void messageReceived(WSclient_t * client, WSopcode_t opcode, uint8_t * payload, size_t length, bool fin);

    void clientDisconnect(WSclient_t * client);
    bool clientIsConnected(WSclient_t * client);

#if(DUPLEX_NETWORK_TYPE != NETWORK_ESP8266_ASYNC)
    void handleClientData(void);
#endif

    void sendHeader(WSclient_t * client);
    void handleHeader(WSclient_t * client, String * headerLine);

    void connectedCb();
    void connectFailedCb();

    void handleHBPing();    // send ping in specified intervals

#if(DUPLEX_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)
    void asyncConnect();
#endif

    /**
         * called for sending a Event to the app
         * @param type WStype_t
         * @param payload uint8_t *
         * @param length size_t
         */
    virtual void runCbEvent(WStype_t type, uint8_t * payload, size_t length) {
        if(_cbEvent) {
            _cbEvent(type, payload, length);
        }
    }
};

#endif /*DUPLEX_H_ */
