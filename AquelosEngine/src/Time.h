#pragma once
#include <chrono>
#include <string>

#define MILLION 1000000
#define BILLION 1000000000

#define MINUTE 60
#define HOUR 3600
#define DAY 86400

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

public:
	static double frameRate;
	static double deltaTime;
	static double elapsedTime;
	static std::string startDateAndTime;
	static int frameCount;

	static std::chrono::time_point<std::chrono::steady_clock> startTime;
	static std::chrono::time_point<std::chrono::steady_clock> prevTime;
	static std::chrono::time_point<std::chrono::steady_clock> currentTime;
};



