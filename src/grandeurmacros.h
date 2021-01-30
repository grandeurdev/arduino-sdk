/**
 * @file grandeurmacros.h
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

// Connection macros
#define GRANDEUR_URL "api.grandeur.tech"
#define GRANDEUR_PORT 443
#define GRANDEUR_FINGERPRINT ""

// Debugging macros
#define DEBUG_PORT Serial

#ifdef DEBUG_PORT
#define DEBUG_GRANDEUR(...) DEBUG_PORT.printf(__VA_ARGS__)
#else
//#define DEBUG_DUPLEX(...) os_printf( __VA_ARGS__ )
#endif

#ifndef DEBUG_GRANDEUR
#define DEBUG_GRANDEUR(...)
#endif

// Strings sizes
#define FINGERPRINT_SIZE 256
#define PACKET_SIZE 512
#define PING_PACKET_SIZE 64
#define TASK_SIZE 32

// Ping interval in milliseconds
#define PING_INTERVAL 25000

// Macros for connection status
#define DISCONNECTED false
#define CONNECTED true
