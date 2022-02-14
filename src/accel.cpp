#pragma once
#include "accel.hpp"
#include "timer.hpp"

namespace specter {

void Accel::addMesh(Mesh* newMesh) {
	if (mesh) {
		throw std::runtime_error("Only one mesh is supported");
	}
	mesh = newMesh;
}

void Accel::build() {
	specter::Timer octreeTimer;
	octree.build(mesh);
	std::cout << "Construction finished in: " << octreeTimer.elapsedTime() << " seconds.\n";
}

bool Accel::traceRay(const Ray& ray, Intersection& intersection) {
	float u, v, t;
	unsigned f;
	// Traverse through the octree
	if (octree.traverse(mesh, ray, u, v, t, f)) {
		intersection.uv = vec2f(u, v);
		intersection.t = t;
		intersection.f = f;
		return true;
	} else {
		return false;
	}
}

}