
#pragma once
#include "mesh.hpp"
#include "timer.hpp"

#include <fstream>

namespace specter {

struct ObjLoader : Mesh {

	// Initializes the mesh through data in filename.obj. This method does NOT check for validity of the object file.
	// Invalid object files will lead to undefined program behavior.
	ObjLoader(const char* filename);

	// Remove unnecessary whitespace from filename.obj
	// If an input file is not loading, try to run this first.
	static void sanitizeObjectFile(const char* filename);

};

}