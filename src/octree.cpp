#include "octree.hpp"

namespace specter {

static const unsigned nSubRegions = 8;

static vec3f minComponents(const vec3f& p0, const vec3f& p1, const vec3f& p2) {
	vec3f minResult;
	minResult[0] = std::min(std::min(p0[0], p1[0]), std::min(p0[0], p2[0]));
	minResult[1] = std::min(std::min(p0[1], p1[1]), std::min(p0[1], p2[1]));
	minResult[2] = std::min(std::min(p0[2], p1[2]), std::min(p0[2], p2[2]));
	return minResult;
}

static vec3f maxComponents(const vec3f& p0, const vec3f& p1, const vec3f& p2) {
	vec3f minResult;
	minResult[0] = std::max(std::max(p0[0], p1[0]), std::max(p0[0], p2[0]));
	minResult[1] = std::max(std::max(p0[1], p1[1]), std::max(p0[1], p2[1]));
	minResult[2] = std::max(std::max(p0[2], p1[2]), std::max(p0[2], p2[2]));
	return minResult;
}

static AxisAlignedBoundingBox constructSubRegion(const AxisAlignedBoundingBox& superRegion, int subRegionIndex) {
	auto bmin = superRegion.min;
	auto bmax = superRegion.max;
	vec3f hd = (bmax - bmin) / 2;	// Half-way inbetween min and max
	AxisAlignedBoundingBox subregion;
	switch (subRegionIndex) {
	case 0:
		subregion.min = bmin;
		subregion.max = bmin + hd;
		break;
	case 1:
		subregion.min = vec3f(bmin[0] + hd[0], bmin[1], bmin[2]);
		subregion.max = vec3f(bmax[0], bmin[1] + hd[1], bmin[2] + hd[2]);
		break;
	case 2:
		subregion.min = vec3f(bmin[0], bmin[1] + hd[1], bmin[2]);
		subregion.max = vec3f(bmin[0] + hd[0], bmax[1], bmin[2] + hd[2]);
		break;
	case 3:
		subregion.min = vec3f(bmin[0] + hd[0], bmin[1] + hd[1], bmin[2]);
		subregion.max = vec3f(bmax[0], bmax[1], bmin[2] + hd[2]);
		break;
	case 4:
		subregion.min = vec3f(bmin[0], bmin[1], bmin[2] + hd[2]);
		subregion.max = vec3f(bmin[0] + hd[0], bmin[1] + hd[1], bmax[2]);
		break;
	case 5:
		subregion.min = vec3f(bmin[0] + hd[0], bmin[1], bmin[2] + hd[2]);
		subregion.max = vec3f(bmax[0], bmin[1] + hd[1], bmax[2]);
		break;
	case 6:
		subregion.min = vec3f(bmin[0], bmin[1] + hd[1], bmin[2] + hd[2]);
		subregion.max = vec3f(bmin[0] + hd[0], bmax[1], bmax[2]);
		break;
	case 7:
		subregion.min = vec3f(bmin[0] + hd[0], bmin[1] + hd[1], bmin[2] + hd[2]);
		subregion.max = vec3f(bmax[0], bmax[1], bmax[2]);
		break;
	default:
		throw std::runtime_error("Subregion index outside range [0, 7]!");
		break;
	}
	return subregion;
}

static void reserveChildren(Node* node, int nChildren) {
	node->m_children = new Node*[nChildren];
	node->nIndices = std::numeric_limits<unsigned int>::max();
	for (int i = 0; i < nChildren; ++i) {
		node->m_children[i] = new Node;
		node->m_children[i]->nIndices = std::numeric_limits<unsigned int>::max();
	}
}

static const unsigned depthMax = static_cast<unsigned>(log8(600'000) + 0.5);

static int nInteriorNodes = 0;
static int nleafNodes = 0;
std::vector<int> totalIndicesAtDepths(depthMax);

static int totalIndices = 0;

void buildOctree(Node* node, const vec3f* const vertices, const FaceElement* const faces, const uint32_t* trianglePositions, int depth) {
	if (node == nullptr) {
		throw std::runtime_error("Node is nullptr!");
	}

	if (node->nIndices == 0) {
		return;
	}

	if (node->nIndices <= 10 || depth > depthMax) {
		totalIndices += node->nIndices;
		node->indices = new uint32_t[node->nIndices];
		std::memcpy(node->indices, trianglePositions, sizeof(uint32_t) * node->nIndices);
		nleafNodes++;
		return;
	}

	if (depth < depthMax) {
		totalIndicesAtDepths[depth] += node->nIndices;
	}

	std::vector<AxisAlignedBoundingBox> subRegions;
	subRegions.reserve(nSubRegions);
	for (int i = 0; i < nSubRegions; ++i) {
		subRegions.push_back(constructSubRegion(node->bbox, i));
	}

	std::vector<std::vector<uint32_t>> subRegionTriangleCounts;
	subRegionTriangleCounts.resize(nSubRegions);

	for (int i = 0; i < node->nIndices; i++) {
		uint32_t i0 = faces[trianglePositions[i] * 3].p;
		uint32_t i1 = faces[trianglePositions[i] * 3 + 1].p;
		uint32_t i2 = faces[trianglePositions[i] * 3 + 2].p;
		vec3f v0 = vertices[i0], v1 = vertices[i1], v2 = vertices[i2];
		const vec3f tmin = minComponents(v0, v1, v2);
		const vec3f tmax = maxComponents(v0, v1, v2);

		const AxisAlignedBoundingBox triangleAABB(tmin, tmax);
		for (int k = 0; k < nSubRegions; ++k) {
			if (subRegions[k].overlapsEdgeInclusive(triangleAABB)) {
				subRegionTriangleCounts[k].push_back(trianglePositions[i]);
			}
		}
	}

	nInteriorNodes++;
	node->m_children = new Node*[nSubRegions];
	for (int i = 0; i < nSubRegions; ++i) {
		if (subRegionTriangleCounts[i].size() != 0) {
			node->m_children[i] = new Node;
			node->m_children[i]->bbox.min = subRegions[i].min;
			node->m_children[i]->bbox.max = subRegions[i].max;
			node->m_children[i]->nIndices = subRegionTriangleCounts[i].size();
			buildOctree(node->m_children[i], vertices, faces, subRegionTriangleCounts[i].data(), depth + 1);
		} else {
			node->m_children[i] = nullptr;
			node->nIndices = std::numeric_limits<unsigned int>::max();
		}
	}
}

struct IndexDistancePair {
	int index;
	float distance = std::numeric_limits<float>::max();

	friend bool operator<(const IndexDistancePair& p0, const IndexDistancePair& p1) {
		return p0.distance < p1.distance;
	}
};

bool compareDistance(const IndexDistancePair& p0, const IndexDistancePair& p1) {
	return p0.distance <= p1.distance;
}

void rayTraversal2(const Mesh* mesh, Node* node, const Ray& ray, float& u, float& v, float& t, uint32_t& index) {
	if (node == nullptr || t != std::numeric_limits<float>::max()) {
		return;
	}

	if (node->indices == nullptr) {
		if (node->m_children != nullptr) {
			for (int i = 0; i < 8; ++i) {
				if (node->m_children[i] != nullptr) {
					if (node->m_children[i]->bbox.rayIntersects(ray)) {
						rayTraversal2(mesh, node->m_children[i], ray, u, v, t, index);
					}
				}
			}
		}
	}

	else {
		for (int i = 0; i < node->nIndices; ++i) {
			float uu, vv, tt = std::numeric_limits<float>::max();
			if(mesh->rayIntersectionV2(ray, node->indices[i], uu, vv, tt)) {
				if (t > tt) {
					t = tt;
					index = node->indices[i];
				}
			}
		}
	}
}

static float closestIntersectionDistance = std::numeric_limits<float>::max();

static unsigned totalTriangleTests = 0;
static unsigned totalAABBTests = 0;

float rayTraversal(const Mesh* mesh, Node* node, const Ray& ray, uint32_t& index) {
	if (node == nullptr) {
		return std::numeric_limits<float>::max();;
	}

	if (node->indices != nullptr) {
		for (int i = 0; i < node->nIndices; ++i) {
			float uu;
			float vv;
			float tt;
			totalTriangleTests++;
			if (mesh->rayIntersectionV2(ray, node->indices[i], uu, vv, tt)) {
				if (closestIntersectionDistance > tt) {
					closestIntersectionDistance = tt;
					index = node->indices[i];
				}
			}
		}
	}

	else {
		if (node->m_children == nullptr) {
			return std::numeric_limits<float>::max();;
		}

		for (int i = 0; i < 8; ++i) {
			if (node->m_children[i] != nullptr) {
				auto subRegion = node->m_children[i]->bbox;
				float near, far;
				totalAABBTests++;
				if (subRegion.rayIntersects(ray, near, far)) {
					if (closestIntersectionDistance > near) {
						rayTraversal(mesh, node->m_children[i], ray, index);
					}
				}
			}
		}
	}

	return closestIntersectionDistance;
}


float rayTraversal_sorted(const Mesh* mesh, Node* node, const Ray& ray, uint32_t& index) {
	if (node == nullptr) {
		return std::numeric_limits<float>::max();;
	}

	if (closestIntersectionDistance != std::numeric_limits<float>::max()) {
		return closestIntersectionDistance;
	}

	if (node->indices != nullptr) {
		for (int i = 0; i < node->nIndices; ++i) {
			float uu;
			float vv;
			float tt;
			totalTriangleTests++;
			if (mesh->rayIntersectionV2(ray, node->indices[i], uu, vv, tt)) {
				if (closestIntersectionDistance > tt) {
					closestIntersectionDistance = tt;
					index = node->indices[i];
					return closestIntersectionDistance;
				}
			}
		}
	}

	else {
		if (node->m_children == nullptr) {
			return std::numeric_limits<float>::max();;
		}

		IndexDistancePair distanceToBoxes[8];

		for (int i = 0; i < 8; ++i) {
			if (node->m_children[i] != nullptr) {
				auto subRegion = node->m_children[i]->bbox;
				float near, far;
				totalAABBTests++;
				if (subRegion.rayIntersect(ray, near, far)) {
					distanceToBoxes[i].distance = near;
					distanceToBoxes[i].index = i;
				}
			}
		}

		std::sort(std::begin(distanceToBoxes), std::end(distanceToBoxes));
		for (int i = 0; i < 8; i++) {
			if (distanceToBoxes[i].distance != std::numeric_limits<float>::max()) {
				rayTraversal_sorted(mesh, node->m_children[distanceToBoxes[i].index], ray, index);
			}
			else {
				break;
			}
		}
	}

	return closestIntersectionDistance;
}

void reinit() {
	closestIntersectionDistance = std::numeric_limits<float>::max();
}

void freeOctree(Node* node) {
	if (node == nullptr) {
		std::cout << "This is null!\n";
		return;
	}

	if (node->m_children != nullptr) {
		for (int i = 0; i < 8; ++i) {
			if (node->m_children[i] != nullptr) {
				freeOctree(node->m_children[i]);
			}
		}
		delete node->m_children;
	}
	delete node->indices;
	delete node;
}

void printStatistics() {
	std::cout << "Number of interior nodes: " << nInteriorNodes << '\n';
	std::cout << "Number of leaf nodes: " << nleafNodes << '\n';
	for(int i = 0; i < depthMax; ++i) {
		std::cout << "Indices at depth[" << i << "]: " << totalIndicesAtDepths[i] << '\n';
	}
	std::cout << "Total indices: " << totalIndices << '\n';
	std::cout << "Avg indices per leaf: " << (float)totalIndices / (float)nleafNodes << '\n';
}

void printTraversal(vec2u screenResolution) {
	std::cout << "Total Ray-Triangle intersections tested: " << totalTriangleTests << '\n';
	std::cout << "Total Ray-AABB intersections tested: " << totalAABBTests << '\n';
	std::cout << "Average ray-triangle intersections tested: " << totalTriangleTests / product(screenResolution) << '\n';
	std::cout << "Average ray-AABB intersections tested: " << totalAABBTests / product(screenResolution) << '\n';
}

}