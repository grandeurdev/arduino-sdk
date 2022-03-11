#include "debug.h"
#include "Var.h"

#ifndef CALLBACK_H_
#define CALLBACK_H_

class Callback {
	private:
    // Stores pointer to the function.
		void* _functionPtr;
    // Stores the number of function arguments.
    int _nArgs;
    // Stores the type of function arguments.
    int _type;
    // Returns function type as string.
    const char* mapType(int type);
    // Prints error to the debug port.
    void printError(String type);

	public:
    // Default constructor
    Callback();
    Callback(int ptr);
    // For string argument
    Callback(void c(const char*));
    // For string and Var arguments
    Callback(void c(const char*, Var));
    // For string and bool arguments
    Callback(void c(const char*, bool));
    // For string and int arguments
    Callback(void c(const char*, int));
    // For string and double arguments
    Callback(void c(const char*, double));
    // For string and string arguments
    Callback(void c(const char*, const char*));

		// This overrides the function call operator to pass data to the function our _functionPtr
    // points to.
    void operator()(const char* str, Var packet);

    // This overrides not operator: !callback.
    bool operator!();
};

#endif