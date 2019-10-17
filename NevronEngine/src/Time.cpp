#include <src/pch.h>
#include "Time.h"
#include "Utility.h"

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

std::chrono::high_resolution_clock::time_point Time::startTime = std::chrono::high_resolution_clock::now();
std::chrono::high_resolution_clock::time_point Time::prevTime = std::chrono::high_resolution_clock::now();
std::chrono::high_resolution_clock::time_point Time::currentTime = std::chrono::high_resolution_clock::now();

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
std::string Time::getDateAndTime(const std::string& format)
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[64];
	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), format.c_str(), &tstruct);

	return buf;
}

std::string Time::getDateAndTime(unsigned int timepoint, const std::string& format)
{
	time_t     now = timepoint;
	struct tm  tstruct;
	char       buf[64];
	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), format.c_str(), &tstruct);

	return buf;
}



