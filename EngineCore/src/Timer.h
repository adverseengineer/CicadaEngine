#pragma once

#include <chrono>
#include <ctime>
#include <ratio>

namespace Cicada {

class Timer {
protected:
	std::chrono::high_resolution_clock::time_point startTime, endTime;

public:
	Timer();
	void StartTiming();
	double GetElapsedTimeInSeconds();
};

}