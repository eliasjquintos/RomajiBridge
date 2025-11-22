#pragma once
#include <chrono>

using highResClock = std::chrono::high_resolution_clock; // alias
using timePoint = highResClock::time_point;
class SimpleTimer
{
	timePoint start;
	timePoint end;
	std::chrono::duration<double> elapsed;
public:
	SimpleTimer();
	~SimpleTimer();
};