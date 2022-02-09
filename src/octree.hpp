#pragma once
#include "vec3.hpp"

#include <vector>

namespace specter {

struct Node {
	std::vector<unsigned> indices;
	std::vector<Node*> children;
};

}