#pragma once
#include <src/pch.h>

#define APPNAME "NevronEngine"
#define STR(x) std::to_string(x)
#define RELEASE 0;

#define ASSERT(x) if (!(x)) abort();

#define WIN32 true
#define LINUX false
#define BIT(x) (1 << x)

#if WIN32
//Defines the / or \\ accordingly
#define SLASH '\\'
#else if LINUX
//Defines the / or \\ accordingly
#define SLASH '/'
#endif
