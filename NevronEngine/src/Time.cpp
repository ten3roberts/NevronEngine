#include "Time.h"
#include "Utility.h"
#include <string>
#include <sstream>

float Time::frameRate = 0.0;
float Time::deltaTime = 0.0;
float Time::unscaledDeltaTime = 0.0;
float Time::fixedDeltaTime = 1 / 50.0f; //Tick speed
float Time::timeScale = 1;
float Time::elapsedTime = 0.0;
float Time::unscaledElapsedTime = 0.0;
std::string Time::startDateAndTime = "";
int Time::frameCount = 0;
std::chrono::time_point<std::chrono::steady_clock> Time::startTime = std::chrono::high_resolution_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Time::prevTime = std::chrono::high_resolution_clock::now();
std::chrono::time_point<std::chrono::steady_clock> Time::currentTime = std::chrono::high_resolution_clock::now();
using namespace Utility;

void Time::Init()
{
	startDateAndTime = getDateAndTime();
}

void Time::Update()
{
	frameCount++;

	currentTime = std::chrono::high_resolution_clock::now();
	unscaledElapsedTime = (currentTime - startTime).count() / (float)GIGA;
	elapsedTime = ((currentTime - startTime).count() / (float)GIGA) * timeScale;

	unscaledDeltaTime = (currentTime - prevTime).count() / (float)GIGA;
	deltaTime = ((currentTime - prevTime).count() / (float)GIGA) * timeScale;

	frameRate = 1.0f / deltaTime;

	prevTime = currentTime;
}
std::string Time::getDateAndTime(TimePrecision precision)
{
	std::stringstream date;

	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);


	int dates[6] = { newtime.tm_year + 1900, newtime.tm_mon + 1, newtime.tm_mday, newtime.tm_hour, newtime.tm_min, newtime.tm_sec };
	if (precision == ONLY_TIME)
	{
		date << digits(newtime.tm_hour, 2) << "." << digits(newtime.tm_min, 2) << "." << digits(newtime.tm_sec, 2);
		return date.str();
	}
	if (precision == ONLY_DATE)
	{
		date << digits(newtime.tm_year, 4) << "-" << digits(newtime.tm_mon, 2) << "-" << digits(newtime.tm_mday, 2);
		return date.str();
	}
	for (int i = 0; i <= precision; i++)
		date << digits(dates[i], i == PREC_YEAR ? 4 : 2) << (i < PREC_DAY ? "-" : i == PREC_DAY ? "_" : i == precision ? "" : ".");

	return date.str();
}

std::string Time::getDateAndTime(unsigned int time, TimePrecision precision)
{
	std::stringstream date;

	struct tm newtime;
	time_t now = time;
	localtime_s(&newtime, &now);


	int dates[6] = { newtime.tm_year + 1900, newtime.tm_mon + 1, newtime.tm_mday, newtime.tm_hour, newtime.tm_min, newtime.tm_sec };
	if (precision == ONLY_TIME)
	{
		date << digits(newtime.tm_hour, 2) << "." << digits(newtime.tm_min, 2) << "." << digits(newtime.tm_sec, 2);
		return date.str();
	}
	if (precision == ONLY_DATE)
	{
		date << digits(newtime.tm_year, 2) << "-" << digits(newtime.tm_mon, 2) << "-" << digits(newtime.tm_mday, 2);
		return date.str();
	}
	for (int i = 0; i <= precision; i++)
		date << digits(dates[i], i == PREC_YEAR ? 4 : 2) << (i < PREC_DAY ? "-" : i == PREC_DAY ? "_" : i == precision ? "" : ".");

	return date.str();
}
