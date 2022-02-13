#include "octree.hpp"

namespace specter {

static const unsigned nSubRegions = 8;

static unsigned dbg_nLeafs = 0;
static unsigned dbg_nInternals = 0;
static unsigned dbg_nDepthConditions = 0;

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
	node->indices = nullptr;
	node->nIndices = std::numeric_limits<unsigned int>::max();
	for (int i = 0; i < nChildren; ++i) {
		node->m_children[i] = new Node;
		node->m_children[i]->indices = nullptr;
		node->m_children[i]->nIndices = std::numeric_limits<unsigned int>::max();
	}
}

void buildOctree(Node* node, const vec3f* const vertices, const vec3u* const faces, const uint32_t* trianglePositions, int depth) {
	if (node == nullptr) {
		throw std::runtime_error("Node is nullptr!");
	}

	if (node->nIndices == 0) {
		return;
	}

	if (node->nIndices <= 10 || depth > 5) {
		node->indices = new uint32_t[node->nIndices];
		std::memcpy(node->indices, trianglePositions, sizeof(uint32_t) * node->nIndices);
		dbg_nLeafs++;
		if (depth > 5) {
			dbg_nDepthConditions++;
		}
		return;
	}

	std::vector<AxisAlignedBoundingBox> subRegions;
	subRegions.reserve(nSubRegions);
	for (int i = 0; i < nSubRegions; ++i) {
		subRegions.push_back(constructSubRegion(node->bbox, i));
	}

	std::vector<std::vector<uint32_t>> subRegionTriangleCounts;

	subRegionTriangleCounts.resize(nSubRegions);
	if (node->nIndices > 32) {
		for (int i = 0; i < nSubRegions; ++i) {
			subRegionTriangleCounts[i].reserve(32);
		}
	}

	for (int i = 0; i < node->nIndices; i++) {
		const uint32_t i0 = faces[trianglePositions[i] * 3].x;
		const uint32_t i1 = faces[trianglePositions[i] * 3 + 1].x;
		const uint32_t i2 = faces[trianglePositions[i] * 3 + 2].x;
		const vec3f v0 = vertices[i0], v1 = vertices[i1], v2 = vertices[i2];
		const vec3f tmin = minComponents(v0, v1, v2);
		const vec3f tmax = maxComponents(v0, v1, v2);

		const AxisAlignedBoundingBox triangleAABB(tmin, tmax);
		for (int k = 0; k < nSubRegions; ++k) {
			if (subRegions[k].overlaps(triangleAABB)) {
				subRegionTriangleCounts[k].push_back(trianglePositions[i]);
			}
		}
	}

	for (int i = 0; i < nSubRegions; ++i) {
		std::cout << "subRegion[" << i << "]: " << subRegionTriangleCounts[i].size() << '\n';
	}
	std::cout << "-------------------------------------------\n";

	dbg_nInternals++;
	reserveChildren(node, nSubRegions);
	for (int i = 0; i < nSubRegions; ++i) {
		if (subRegionTriangleCounts.size() != 0) {
			node->m_children[i]->bbox.min = subRegions[i].min;
			node->m_children[i]->bbox.max = subRegions[i].max;
			node->m_children[i]->nIndices = subRegionTriangleCounts[i].size();
			buildOctree(node->m_children[i], vertices, faces, subRegionTriangleCounts[i].data(), depth + 1);
		}
	}
}

struct IndexDistancePair {
	int index;
	float distance;

	friend bool operator<(const IndexDistancePair& p0, const IndexDistancePair& p1) {
		return p0.distance < p1.distance;
	}
};

bool compareDistance(const IndexDistancePair& p0, const IndexDistancePair& p1) {
	return p0.distance <= p1.distance;
}

void rayTraversal(const Mesh* mesh, Node* node, const Ray& ray, float& u, float& v, float& mint, uint32_t& index) {
	if (node == nullptr) {
		return;
	}

	if (node->indices != nullptr) {
		for (int i = 0; i < node->nIndices; ++i) {
			float uu, vv, tt;
			if (mesh->rayIntersectionV2(ray, node->indices[i], uu, vv, tt)) {
				if (mint > tt) {
					mint = tt;
					u = uu;
					v = vv;
					index = node->indices[i];
				}
			}
		}
	}

	else {
		if (node->m_children == nullptr) {
			return;
		}

		IndexDistancePair sortedDistances[8];
		for (int i = 0; i < 8; ++i) {
			sortedDistances[i].distance = std::numeric_limits<float>::max();
			if (node->m_children[i] != nullptr) {
				auto subRegion = node->m_children[i]->bbox;
				float near, far;
				if (subRegion.rayIntersects(ray, near, far)) {
					if (mint > near) {
						sortedDistances[i].distance = near;
						sortedDistances[i].index = i;
						//rayTraversal(mesh, node->m_children[sortedDistances[i].index], ray, u, v, mint, index);
					}
				}
			}
		}
		
		std::sort(std::begin(sortedDistances), std::end(sortedDistances));
		for (int i = 0; i < 8; ++i) {
			if (sortedDistances[i].distance < std::numeric_limits<float>::max()) {
				if (mint > sortedDistances[i].distance) {
					rayTraversal(mesh, node->m_children[sortedDistances[i].index], ray, u, v, mint, index);
				}
			}
		}
	}
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

void printOctreeNumbers() {
	std::cout << "Number of internals: " << dbg_nInternals << '\n';
	std::cout << "Number of leaves: " << dbg_nLeafs << '\n';
	std::cout << "Number of depth conditions: " << dbg_nDepthConditions << '\n';
}

}