#include "Callback.h"

#define NONE 0
#define VAR 1
#define BOOLEAN 2
#define INTEGER 3
#define DOUBLE 4
#define STRING 5

const char* Callback::mapType(int type) {
  // Returns function type as string
  switch(type) {
      case NONE: return "none";
      case VAR: return "var";
      case BOOLEAN: return "boolean";
      case INTEGER: return "int";
      case DOUBLE: return "double";
      case STRING: return "string";
  }
}

void Callback::printError(String type) {
  // Prints error to Debug Port.
  DEBUG_GRANDEUR("[TYPE-ERROR] Was expecting %s and received %s\n", mapType(_type), type.c_str());
}

Callback::Callback() : _functionPtr(NULL), _nArgs(0), _type(NONE) {}

Callback::Callback(int ptr) : _functionPtr(NULL), _nArgs(0), _type(NONE) {}

Callback::Callback(void c(const char*)) : _nArgs(1), _type(NONE) {
  // Storing function pointer as void pointer for the sake of inclusiveness.
  _functionPtr = (void*) c;
}

Callback::Callback(void c(const char*, Var)) : _nArgs(2), _type(VAR) {
  // Storing function pointer as void pointer for the sake of inclusiveness.
  _functionPtr = (void*) c;
}

Callback::Callback(void (c)(const char*, bool)) : _nArgs(2), _type(BOOLEAN) {
  // Storing function pointer as void pointer for the sake of inclusiveness.
  _functionPtr = (void*) c;
}

Callback::Callback(void (c)(const char*, int)) : _nArgs(2), _type(INTEGER) {
  // Storing function pointer as void pointer for the sake of inclusiveness.
  _functionPtr = (void*) c;
}

Callback::Callback(void (c)(const char*, double)) : _nArgs(2), _type(DOUBLE) {
  // Storing function pointer as void pointer for the sake of inclusiveness.
  _functionPtr = (void*) c;
}

Callback::Callback(void (c)(const char*, const char*)) : _nArgs(2), _type(STRING) {
  // Storing function pointer as void pointer for the sake of inclusiveness.
  _functionPtr = (void*) c;
}

void Callback::operator()(const char* str, Var var) {
  // Single argument function has type NONE.
  // Types are for functions of two arguments.
  // Checking number of arguments:
  if (_nArgs == 1) {
    // Just pass the str argument to function.
    return ((void (*)(const char*)) _functionPtr)(str);
  }
  if (_nArgs == 2) {
    // Getting type of the var.
    String varType = Var::typeof(var);

    // If var is a number
    if (varType == "number") {
      // If var as double is bigger than var as int, we use double, otherwise int.
      varType = ((double) var - (int) var != 0) ? "double" : "int";
    }
    // If var is an array
    else if (varType == "array") {
      // Use it as var.
      varType = "var";
    }

    // Switch cases on function type. We cast "var" to type functionPtr accepts.
    switch(_type) {
      case VAR:
        // If functionPtr accepts Var, pass var argument as it is.
        return ((void (*)(const char*, Var)) _functionPtr)(str, var);
      case BOOLEAN:
        // If functionPtr accepts Bool, pass var argument as bool.
        if (varType != "boolean")
          // If var argument isn't boolean.
          return printError(varType);
        else
          // Call the function.
          return ((void (*)(const char*, bool)) _functionPtr)(str, (bool) var);

      case INTEGER:
        // If functionPtr accepts Int, pass var argument as int.
        if (varType != "int")
          // If var argument isn't int.
          return printError(varType);
        else 
          // Call the function.
          return ((void (*)(const char*, int)) _functionPtr)(str, (int) var);

      case DOUBLE:
        // If functionPtr accepts Double, pass var argument as double.
        if (varType != "double")
          // If var argument isn't double.
          return printError(varType);
        else 
          // Call the function.
          return ((void (*)(const char*, double)) _functionPtr)(str, (double) var);

      case STRING:
        // If functionPtr accepts String, pass var argument as string.
        if (varType != "string")
          // If var argument isn't string.
          return printError(varType);
        else 
          // Call the function.
          return ((void (*)(const char*, const char*)) _functionPtr)(str, (const char*) var);
    }

    return;
  }
}

bool Callback::operator!() {
  // Returns true if the function pointer _functionPtr is not set.
  return (!_functionPtr && _type == NONE);
}