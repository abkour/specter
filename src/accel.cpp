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
	std::cout << "Building octree...";
	specter::Timer octreeTimer;
	octree.build(mesh);
	std::cout << " Finished in : " << octreeTimer.elapsedTime() << " seconds.\n\n";
}

bool Accel::traceRay(const Ray& ray, Intersection& intersection, bool shadowRay) {
	// Traverse through the octree
	if (shadowRay) {
		if (octree.traverseAny(mesh, ray)) {
			return true;
		}
		else {
			return false;
		}
	}

	if (octree.traverse(mesh, ray, intersection)) {
		return true;
	} else {
		return false;
	}
}

}