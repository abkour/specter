#pragma once
#include "mesh.hpp"
#include "timer.hpp"

#include <fstream>

namespace specter {

// This object is used to load a wavefront obj file and initialize the members of Mesh with it.
struct ObjLoader : Mesh {

	ObjLoader() = default;

	// Initializes the mesh through data in filename.obj. This method does NOT check for validity of the object file.
	// Invalid object files will lead to undefined program behavior.
	void parse(const char* filename);

};

}