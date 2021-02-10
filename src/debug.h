// Debugging macros
// #define DEBUG_PORT Serial
#define GRANDEUR_DEBUG_PORT Serial
#ifdef DEBUG_PORT
#define GRANDEUR_DEBUG_PORT DEBUG_PORT
#endif

#ifdef GRANDEUR_DEBUG_PORT
#define DEBUG_GRANDEUR(format, args...) \
    GRANDEUR_DEBUG_PORT.printf(("[GRANDEUR-DEBUG] " + String(format)).c_str(), ##args)
#else
//#define DEBUG_GRANDEUR(format, args...) os_printf(("[GRANDEUR-DEBUG] " + String(format)).c_str(), ##args)
#endif

#ifndef DEBUG_GRANDEUR
#define DEBUG_GRANDEUR(...)
#endif