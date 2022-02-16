#pragma once
#include <chrono>

namespace specter {

// Auxillary object used to benchmark code with minimal code footprint
struct Timer {

	// Starts the timer
	Timer();
	// Ends the timer and returns the time elapsed.
	double elapsedTime();

private:

	std::chrono::high_resolution_clock::time_point start, end;
};

}