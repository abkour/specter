#pragma once
#include "accel.hpp"
#include "timer.hpp"

namespace specter {

void Accel::dbg_print() {
	octree.dbg_print();
}

void Accel::addMesh(Mesh* newMesh) {
	if (mesh) {
		throw std::runtime_error("Only one mesh is supported");
	}
	mesh = newMesh;
}

void Accel::build() {
	std::cout << "Building octree...";
	specter::Timer octreeTimer;
	octree.build(mesh);
	std::cout << " Finished in : " << octreeTimer.elapsedTime() << " seconds.\n\n";
}

bool Accel::traceRay(const Ray& ray, Intersection& intersection) const {
	return octree.traverse(mesh, ray, intersection);
}

bool Accel::traceShadowRay(const Ray& ray) const {
	return octree.traverseAny(mesh, ray);
}

bool Accel::traceShadowRayTmax(const Ray& ray, const float t_max) const {
	return octree.traverseAnyTmax(mesh, ray, t_max);
}


}