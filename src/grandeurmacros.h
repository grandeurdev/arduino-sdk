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
#define GRANDEUR_URL "192.168.10.3"
#define GRANDEUR_PORT 8000
#define GRANDEUR_FINGERPRINT ""

// Strings sizes
#define FINGERPRINT_SIZE 256
#define PACKET_SIZE 512
#define PING_PACKET_SIZE 64
#define TASK_SIZE 32
#define TOPIC_SIZE 64

// Send Queue Size
#define SENDQUEUE_SIZE 16

// Ping interval in milliseconds
#define PING_INTERVAL 25000

// Macros for connection status
#define DISCONNECTED false
#define CONNECTED true

// Indexes for update handler callbacks in subscriptions array
#define NUMBER_OF_TOPICS 1
#define DATAUPDATE 0
