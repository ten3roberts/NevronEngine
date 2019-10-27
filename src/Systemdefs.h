#pragma once
#include <Config.h>
#include <src/pch.h>

#define STR(x) std::to_string(x)

#define ASSERT(x) if (!(x)) abort();

//#define WIN32 false
//#define LINUX true
#define BIT(x) (1 << x)

#if WIN32
    //Defines the / or \\ accordingly
    #define SLASH '\\'
#elif UNIX
    //Defines the / or \\ accordingly
    #define SLASH '/'
#endif

