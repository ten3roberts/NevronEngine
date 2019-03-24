#include "Time.h"

#include <string>
#include <sstream>
double Time::frameRate = 0.0;
double Time::deltaTime = 0.0;
double Time::elapsedTime = 0.0;
std::string Time::startDateAndTime = "";
int Time::frameCount = 0;
std::chrono::time_point<std::chrono::steady_clock> Time::startTime = std::chrono::high_resolution_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Time::prevTime = std::chrono::high_resolution_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Time::currentTime = std::chrono::high_resolution_clock::now();

void Time::Init()
{
	startDateAndTime = getDateAndTime();
}

void Time::Update()
{
	frameCount++;

	currentTime = std::chrono::high_resolution_clock::now();
	elapsedTime = (currentTime - startTime).count()/(double)BILLION;

	deltaTime = (currentTime - prevTime).count() / (double)BILLION;

	frameRate = 1.0f / deltaTime;

	prevTime = currentTime;
}
std::string Time::getDateAndTime(TimePrecision precision)
{
	std::stringstream date;

	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	//date << newtime.tm_year + 1900 << "-" << newtime.tm_mon + 1 << "-" << newtime.tm_mday << "-" << newtime.tm_hour << std::string(".") << newtime.tm_min << std::string(".") << newtime.tm_sec;

	int dates[6] = { newtime.tm_year + 1900, newtime.tm_mon + 1, newtime.tm_mday, newtime.tm_hour, newtime.tm_min, newtime.tm_sec };
	if (precision == ONLY_TIME)
	{
		date << newtime.tm_hour << "." << newtime.tm_min << "." << newtime.tm_sec;
		return date.str();
	}
	if (precision == ONLY_DATE)
	{
		date << newtime.tm_year << "-" << newtime.tm_mon << "-" << newtime.tm_mday;
		return date.str();
	}
	for (int i = 0; i <= precision; i++)
		date << dates[i] << (i < PREC_DAY ? "-" : i == PREC_DAY ? "_" : i == precision ? "" : ".");

	return date.str();
}