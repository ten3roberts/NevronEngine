#include "Timer.h"
#include "Utility.h"
#include <iostream>
#include <string>

using namespace Utility;

Timer::Timer(std::string name) : name(name), duration(0)
{
	start = std::chrono::high_resolution_clock::now();
	result = nullptr;
}

Timer::Timer(double* result, std::string name) : name(name), duration(0)
{
	start = std::chrono::high_resolution_clock::now();
	this->result = result;
}


Timer::~Timer()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	if (result)
		* result = duration.count();
	else
		logger << author << "Timer: " + name << "Duration: " + std::to_string(duration.count() * 1000) + "ms" << lend;
}
