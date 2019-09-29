#pragma once
#include <src/pch.h>
#include <ostream>
#include <sstream>

#include <stdio.h>      /* printf */
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

constexpr auto max_loglength = 512;

//Logs a formatted message with time stamp and author/source
extern void LogS(const std::string& author, std::string format, ...);
//extern void LogS(const char* format, ...);

//Logs a formatted message
extern void LogF(std::string format, ...);

//Logs a formatted error message with author/source
extern void LogE(const std::string& author, std::string format, ...);


