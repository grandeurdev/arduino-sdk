// Connection macros
#define APOLLO_URL "192.168.10.3"
#define APOLLO_PORT 3000
#define APOLLO_FINGERPRINT ""

// Defining general macros
#define MAX_CONFIG_TRIES 30
#define SSID_SIZE 32
#define PASSPHRASE_SIZE 32
#define DEVICEID_SIZE 32
#define APIKEY_SIZE 32
#define TOKEN_SIZE 512
#define IP_SIZE 16
#define FINGERPRINT_SIZE 256

// Ping interval in milliseconds
#define PING_INTERVAL 25000

// Defining macros for Apollo states
#define WIFI_NOT_CONNECTED 0
#define WIFI_CONNECTED 1
#define APOLLO_CONNECTED 2

// Indexes for functions in eventQueue
#define LOGOUT 0

// Indexes for handlers callbacks
#define ONCONNECTED 0
#define ONDISCONNECTED 1
#define ONMESSAGE 2