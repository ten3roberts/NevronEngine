#pragma once
#include <chrono>
#include <string>

//Will print the duration of its lifetime aka to time a function in ms and write the results to float if provided in seconds
class Timer
{
public:
	Timer(std::string name = "");
	Timer(float* result, std::string name = "");
	~Timer();
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
	std::chrono::duration<float> duration;
	float* result;
	std::string name;

};

