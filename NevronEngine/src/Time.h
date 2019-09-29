#pragma once
#include <src/pch.h>
#include <chrono>

#include "..\\Math\Math.h"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

#define SLEEP(t) std::this_thread::sleep_for(t)
#define SLEEPFOR(s) std::this_thread::sleep_for(std::chrono::milliseconds((long long)(s*1000)))

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
	static float unscaledDeltaTime;
	//Tick speed
	static float fixedDeltaTime;
	static float elapsedTime;
	static float unscaledElapsedTime;
	static float timeScale;

	static std::string startDateAndTime;
	static int frameCount;


	static std::chrono::time_point<std::chrono::steady_clock> startTime;
	static std::chrono::time_point<std::chrono::steady_clock> prevTime;
	static std::chrono::time_point<std::chrono::steady_clock> currentTime;
};






