#include "Timer.h"
#include "..\Math\Math.h"
#include "Utility.h"
#include <iostream>
#include <string>

using namespace Utility;

Timer::Timer(const std::string& name) : name(name), duration(0), itemCount(1)
{
	start = std::chrono::high_resolution_clock::now();
	result = nullptr;
}

Timer::Timer(const std::string& name, unsigned long itemCount) : name(name), duration(0), itemCount(itemCount)
{
	start = std::chrono::high_resolution_clock::now();
	result = nullptr;
}

Timer::Timer(double* result, const std::string& name) : name(name), duration(0), itemCount(1)
{
	start = std::chrono::high_resolution_clock::now();
	this->result = result;
}

Timer::Timer(double* result, const std::string& name, unsigned long itemCount) : name(name), duration(0), itemCount(itemCount)
{
	start = std::chrono::high_resolution_clock::now();
	this->result = result;
}

Timer::~Timer()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;



	float durationC = duration.count();
	std::string unit = (durationC < 0.00001 ? "ns" : durationC < 0.01 ? "ms" : "s");
	float durationU = durationC * (unit == "ns" ? GIGA : unit == "ms" ? KILO : 1);

	if (result)
		* result = duration.count();
	else if (itemCount > 1)
		LogS("Timer: " + name, "Duration: %s; time per item %s", FormatSeconds(duration.count()), FormatSeconds(duration.count() / itemCount));
	else
		LogS("Timer: " + name, "Duration: %s", FormatSeconds(duration.count()));
}



