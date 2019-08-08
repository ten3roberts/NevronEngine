#pragma once
#include <ostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"
#include "../Math/Matrix.h"

#include <stdio.h>      /* printf */
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#define max_loglenght 512
extern void LogS(const std::string& author, std::string format, ...);
//extern void LogS(const char* format, ...);
extern void LogS(const std::string& author, const char* format, ...);

