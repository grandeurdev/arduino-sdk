/**
 * @file grandeurmacros.h
 * @date 24.03.2020
 * @author Grandeur Technologies
 *
 * Copyright (c) 2019 Grandeur Technologies LLP. All rights reserved.
 * This file is part of the Arduino SDK for Grandeur.
 *
 */

#include "debug.h"

// Connection macros
#define GRANDEUR_URL "api.grandeur.tech"
#define GRANDEUR_PORT 443
#define GRANDEUR_FINGERPRINT ""

// Strings sizes
#define FINGERPRINT_SIZE 256
#define MESSAGE_SIZE 512
#define PING_MESSAGE_SIZE 64
#define TASK_SIZE 32

// Ping interval in milliseconds
#define PING_INTERVAL 25000

// Macros for connection status
#define DISCONNECTED false
#define CONNECTED true
