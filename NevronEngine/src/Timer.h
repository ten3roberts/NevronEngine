#pragma once
#include <src/pch.h>
#include <chrono>


//Will print the duration of its lifetime aka to time a function in ms and write the results to float if provided in seconds
class Timer
{
public:
	Timer(const std::string& name = "unnammed");
	Timer(const std::string& name, unsigned long itemCount);
	//Will assign the resulting time to $result and suppres log call
	Timer(double* result, const std::string& name = "unnammed");
	Timer(double* result, const std::string& name, unsigned long itemCount);
	~Timer();
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
	std::chrono::duration<float> duration;
	double* result;
	std::string name;
	
	//If you time several similar events it can calculate an average time per event. E.g; when calculating collisions it can use the amount of entities to calculate time per entity
	unsigned long itemCount;

};




