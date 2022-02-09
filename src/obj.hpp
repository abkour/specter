#pragma once
#include "mesh.hpp"
#include "timer.hpp"

#include <fstream>

namespace specter {

struct ObjLoader : Mesh {

	ObjLoader(const char* filename);

};

}