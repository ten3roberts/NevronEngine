#include <src/pch.h>
#include "Logger.h"
#include "Utility.h"
#include <iostream>
#include "Time.h"

static std::ofstream logFile;


//Checks to see if the frame changes to put a divider between log calls on different frames
static int frame;
void LogS(const std::string& author, std::string format, ...)
{
	enum Flag
	{
		None, Long
	};

	va_list vl;
	va_start(vl, format);

	std::string fullMsg = '(' + (author == "" ? "Log" : author) + " @ " +
		Time::getDateAndTime(Time::ONLY_TIME) + "): " + vformat(format, vl);

	if (!logFile.is_open())
	{
		std::string logfile_name = WORKDIR + "Logs\\" + Time::startDateAndTime + ".log";
		Utility::GenerateFile(logfile_name, "");
		logFile.open(logfile_name);
		LogS("Logger", "Creating new logfile %s", logfile_name);
	}

	if (frame != Time::frameCount)
	{
		frame = Time::frameCount;
		std::string divider(fullMsg.size(), '-');
		divider += "\n";
		fullMsg.insert(fullMsg.begin(), divider.begin(), divider.end());
	}
	fullMsg += '\n';
	printf(fullMsg.c_str());
	logFile.write(fullMsg.c_str(), fullMsg.size());
	va_end(vl);
}

void LogF(std::string format, ...)
{
	va_list vl;
	va_start(vl, format);

	std::string fullMsg = "(Log @ " + Time::getDateAndTime(Time::ONLY_TIME) + "): " + vformat(format, vl);

	if (!logFile.is_open())
	{
		std::string logfile_name = WORKDIR + "Logs\\" + Time::startDateAndTime + ".log";
		Utility::GenerateFile(logfile_name, "");
		logFile.open(logfile_name);
		LogS("Logger", "Creating new logfile %s", logfile_name);
	}

	if (frame != Time::frameCount)
	{
		frame = Time::frameCount;
		std::string divider(fullMsg.size(), '-');
		divider += "\n";
		fullMsg.insert(fullMsg.begin(), divider.begin(), divider.end());
	}

	fullMsg += '\n';
	printf(fullMsg.c_str());
	logFile.write(fullMsg.c_str(), fullMsg.size());
	va_end(vl);
}

void LogE(const std::string& author, float pause, std::string format, ...)
{
	enum Flag
	{
		None, Long
	};

	va_list vl;
	va_start(vl, format);

	std::string fullMsg = '(' + (author == "" ? "Log" : author) + " @ " +
		Time::getDateAndTime(Time::ONLY_TIME) + "): " + vformat(format, vl);

	if (!logFile.is_open())
	{
		std::string logfile_name = WORKDIR + "Logs\\" + Time::startDateAndTime + ".log";
		Utility::GenerateFile(logfile_name, "");
		logFile.open(logfile_name);
		LogS("Logger", "Creating new logfile %s", logfile_name);
	}

	if (frame != Time::frameCount)
	{
		frame = Time::frameCount;
		std::string divider(fullMsg.size(), '-');
		divider += "\n";
		fullMsg.insert(fullMsg.begin(), divider.begin(), divider.end());
	}
	fullMsg += '\n';
	printf(fullMsg.c_str());
	logFile.write(fullMsg.c_str(), fullMsg.size());
	va_end(vl);
	SLEEPFOR(pause);
}
