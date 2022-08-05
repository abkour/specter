#pragma once
#include "accel.hpp"
#include "timer.hpp"
#include "dev/cpu_lbvh.hpp"

namespace specter {

void Accel::addModel(std::shared_ptr<Model>& model) {
	this->model = model;
}

void Accel::build() {
	std::cout << "Building octree...";
	specter::Timer octreeTimer;
	accel_struct = std::make_unique<CPU_LBVH>();
	accel_struct->build(model);	
	std::cout << " Finished in : " << octreeTimer.elapsedTime() << " seconds.\n\n";
}

bool Accel::traceRay(const Ray& ray, Intersection& intersection) const {
	return accel_struct->traverse(model.get(), ray, intersection);
}

}