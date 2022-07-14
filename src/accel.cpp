#pragma once
#include "accel.hpp"
#include "timer.hpp"

namespace specter {

void Accel::dbg_print() {
	octree.dbg_print();
}

void Accel::addModel(std::shared_ptr<Model>& model) {
	this->model = model;
}

void Accel::build() {
	std::cout << "Building octree...";
	specter::Timer octreeTimer;
	octree.build(model);	
	std::cout << " Finished in : " << octreeTimer.elapsedTime() << " seconds.\n\n";
}

bool Accel::traceRay(const Ray& ray, Intersection& intersection) const {
	return octree.traverse(model.get(), ray, intersection);
}

bool Accel::traceShadowRay(const Ray& ray) const {
	return octree.traverseAny(model.get(), ray);
}

bool Accel::traceShadowRayTmax(const Ray& ray, const float t_max) const {
	return octree.traverseAnyTmax(model.get(), ray, t_max);
}


}