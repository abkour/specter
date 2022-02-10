#pragma once
#include <chrono>

namespace specter {

struct Timer {

	Timer();

	double elapsedTime();

private:

	std::chrono::high_resolution_clock::time_point start, end;
};

}