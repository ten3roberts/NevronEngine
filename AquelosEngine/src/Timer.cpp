#include "Timer.h"
#include "Utility.h"
#include <iostream>
#include <string>

Timer::Timer(std::string name) : name(name)
{
	start = std::chrono::high_resolution_clock::now();
	result = nullptr;
}

Timer::Timer(float * result, std::string name) : name(name)
{
	start = std::chrono::high_resolution_clock::now();
	this->result = result;
}


Timer::~Timer()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	if (result)
		*result = duration.count();

	Util::Log(name + " duration: " + std::to_string(duration.count() * 1000) + "ms");
}
