#pragma once
#include "../timer.hpp"

#include "accel.hpp"
#include "cpu_lbvh.hpp"

namespace specter {

void Accel::addModel(std::shared_ptr<Model>& model) {
	this->model = model;
}

void Accel::build() {
	std::cout << "Building accelerating structure...\n";
	specter::Timer octreeTimer;
	accel_struct = std::make_shared<CPU_LBVH>();
	accel_struct->build(model);
	if (!accel_struct->isValid()) {
		std::cerr << "SpacePartitioner wrong!\n";
	}
	std::cout << " Finished in : " << octreeTimer.elapsedTime() << " seconds.\n\n";
}

bool Accel::traceRay(const Ray& ray, Intersection& intersection) const {
	return accel_struct->traverse(model.get(), ray, intersection);
}

}