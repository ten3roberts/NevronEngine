#pragma once
#include <chrono>
#include <string>


#define KILO 1000
#define MILLION 1000000
#define BILLION 1000000000

#define NANO 1/BILLION
#define MICRO 1/MILLION
#define MILLI 1/KILO

#define MINUTE 60
#define HOUR 3600
#define DAY 86400

#define SLEEP std::this_thread::sleep_for

class Time
{
public:
	enum TimePrecision
	{
		PREC_YEAR, PREC_MONTH, PREC_DAY, PREC_HOUR, PREC_MINUTE, PREC_SECOND, ONLY_TIME, ONLY_DATE
	};

	static void Init();

	static void Update();

	static std::string getDateAndTime(TimePrecision precision = PREC_SECOND);
	static std::string getDateAndTime(unsigned int time, TimePrecision precision = PREC_SECOND);

public:
	static float frameRate;
	static float deltaTime;
	static float elapsedTime;
	static std::string startDateAndTime;
	static int frameCount;

	static std::chrono::time_point<std::chrono::steady_clock> startTime;
	static std::chrono::time_point<std::chrono::steady_clock> prevTime;
	static std::chrono::time_point<std::chrono::steady_clock> currentTime;
};



