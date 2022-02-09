#include "timer.hpp"

namespace specter {

Timer::Timer() {
	start = std::chrono::high_resolution_clock::now();
}

double Timer::elapsedTime() {
	end = std::chrono::high_resolution_clock::now();
	double elapsed_time = std::chrono::duration<double>(end - start).count();
	return elapsed_time;
}

}