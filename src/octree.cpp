#include "octree.hpp"

#include <numeric>	// For std::iota

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

Octree::Octree() {
	root = nullptr;
}

Octree::~Octree() {
	freeOctreeRec(root);
}

void Octree::build(Mesh* mesh) {
	root = new Node;
	root->nIndices = mesh->getTriangleCount();
	root->bbox = mesh->computeBoundingBox();

	std::vector<uint32_t> initialIndexList(mesh->getTriangleCount());
	std::iota(initialIndexList.begin(), initialIndexList.end(), 0);

	maxDepth = log8(mesh->getTriangleCount());
	buildRec(root, mesh->getVertices(), mesh->getFaces(), initialIndexList.data(), 0);
}

void Octree::buildRec(Node* node, const vec3f* vertices, const FaceElement* faces, const uint32_t* trianglePositions, int depth) {
	if (node == nullptr) {
		throw std::runtime_error("Node is nullptr!");
	}

	if (node->nIndices == 0) {
		return;
	}

	if (node->nIndices <= 10 || depth > maxDepth) {
		node->indices = new uint32_t[node->nIndices];
		std::memcpy(node->indices, trianglePositions, sizeof(uint32_t) * node->nIndices);
		return;
	}

	std::vector<AxisAlignedBoundingBox> subRegions(nSubRegions);
	for (int i = 0; i < nSubRegions; ++i) {
		subRegions[i] = constructSubRegion(node->bbox, i);
	}

	std::vector<std::vector<uint32_t>> subRegionTriangleCounts(nSubRegions);
	for (int i = 0; i < node->nIndices; i++) {
		uint32_t i0 = faces[trianglePositions[i] * 3 + 0].p;
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

	node->m_children = new Node*[nSubRegions];
	for (int i = 0; i < nSubRegions; ++i) {
		if (subRegionTriangleCounts[i].size() != 0) {
			node->m_children[i] = new Node;
			node->m_children[i]->bbox.min = subRegions[i].min;
			node->m_children[i]->bbox.max = subRegions[i].max;
			node->m_children[i]->nIndices = subRegionTriangleCounts[i].size();
			buildRec(node->m_children[i], vertices, faces, subRegionTriangleCounts[i].data(), depth + 1);
		}
		else {
			node->m_children[i] = nullptr;
			node->nIndices = std::numeric_limits<unsigned int>::max();
		}
	}
}

bool Octree::traverse(const Mesh* mesh, const Ray& ray, Intersection& intersection) {
	traverseRec(mesh, root, ray, intersection);
	return intersection.t != std::numeric_limits<float>::max();
}

struct IndexDistancePair {
	int index;
	float distance = std::numeric_limits<float>::max();

	friend bool operator<(const IndexDistancePair& p0, const IndexDistancePair& p1) {
		return p0.distance < p1.distance;
	}
};

// On intersection t stores the distance to the closest triangle it collided with.
// u and v store the barycentric coordinates of that intersection point
// index refers to the triangle in the indices array of the mesh class.
// The algorithm sorts the sub-regions according to their intersection distances with the ray.
// The ray then recursively travels through the sub-regions in ascending order with respect to the intersection distance.
// If any intersection with a triangle is found on leaf encounter, it is guaranteed to be the closest triangle,
// and further processing can stop.
void Octree::traverseRec(const Mesh* mesh, Node* node, const Ray& ray, Intersection& intersection, bool multipleBoxesHit) {
	if (node == nullptr) {
		return;
	}

	// Closest triangle found and further processing can stop.
	if (intersection.t != std::numeric_limits<float>::max() && multipleBoxesHit) {
		return;
	}

	if (node->indices != nullptr) {
		for (int i = 0; i < node->nIndices; ++i) {
			float u, v, t = std::numeric_limits<float>::max();
			if (mesh->rayIntersectionV2(ray, node->indices[i], u, v, t)) {
				if (intersection.t > t) {
					// Success, closest triangle found. Any further processing can stop.
					intersection.t = t;
					intersection.uv = vec2f(u, v);
					intersection.f = node->indices[i];
				}
			}
		}
	}

	else {
		if (node->m_children == nullptr) {
			return;
		}

		IndexDistancePair distanceToBoxes[nSubRegions];
		for (int i = 0; i < nSubRegions; ++i) {
			if (node->m_children[i] != nullptr) {
				auto subRegion = node->m_children[i]->bbox;
				float near, far;
				if (subRegion.rayIntersect(ray, near, far)) {
					distanceToBoxes[i].distance = near;
					distanceToBoxes[i].index = i;
				}
			}
		}

		// Sort sub-regions according to the intersection distance, in order 
		// to perform a sorted descend.
		std::sort(std::begin(distanceToBoxes), std::end(distanceToBoxes));

		for (int i = 0; i < nSubRegions; ++i) {
			if (distanceToBoxes[i].distance > 0.f && distanceToBoxes[i].distance != std::numeric_limits<float>::max()) {
				if (i != nSubRegions - 1) {
					// If a ray hits two bounding boxes at the same point, we have to traverse through both of them
					// to find the closest triangle. This is an extremely rare edge case, but for correctness sake it needs to 
					// be handled.
					if (distanceToBoxes[i].distance == distanceToBoxes[i + 1].distance) {
						traverseRec(mesh, node->m_children[distanceToBoxes[i + 0].index], ray, intersection, false);
						traverseRec(mesh, node->m_children[distanceToBoxes[i + 1].index], ray, intersection, true);	// Prevent early termination
					} else {
						traverseRec(mesh, node->m_children[distanceToBoxes[i].index], ray, intersection);
					}
				}
				else {
					traverseRec(mesh, node->m_children[distanceToBoxes[i].index], ray, intersection);
				}
			} else {
				// We can stop now, since subsequent distances will be MAX_FLOAT as well.
				break;
			}
		}
	}
}

bool Octree::traverseAny(const Mesh* mesh, const Ray& ray) {
	bool intersectionFound = false;
	traverseAnyRec(mesh, root, ray, intersectionFound);
	return intersectionFound;
}

void Octree::traverseAnyRec(const Mesh* mesh, Node* node, const Ray& ray, bool& intersectionFound) {
	if (node == nullptr) {
		return;
	}

	// Any triangle found, therefore further processing can stop.
	if (intersectionFound) {
		return;
	}

	if (node->indices != nullptr) {
		for (int i = 0; i < node->nIndices; ++i) {
			float uu, vv, tt;
			if (mesh->rayIntersectionV2(ray, node->indices[i], uu, vv, tt)) {
				// Rays with origins inside the space represented by the octree can 
				// have intersections with geometry, where the ray parameter is negative.
				// This occurs, when the ray is colliding with objects that are in opposite 
				// direction of the ray direction vector. 
				// We don't want to consider this case at all.
				if (tt <= 0.f) return;
				intersectionFound = true;
				return;
			}
		}
	}

	else {
		if (node->m_children == nullptr) {
			return;
		}

		for (int i = 0; i < nSubRegions; ++i) {
			if (node->m_children[i] != nullptr) {
				float near, far;
				if (node->m_children[i]->bbox.rayIntersect(ray, near, far)) {
					// Rays with origins inside the space represented by the octree can 
					// have intersections with geometry, where the ray parameter is negative.
					// This occurs, when the ray is colliding with objects that are in opposite 
					// direction of the ray direction vector. 
					// We don't want to consider this case at all.
					if (near > 0.f) {
						traverseAnyRec(mesh, node->m_children[i], ray, intersectionFound);
					}
				}
			}
		}
	}
}

void Octree::freeOctreeRec(Node* node) {
	if (node == nullptr) {
		std::cout << "This is null!\n";
		return;
	}

	if (node->m_children != nullptr) {
		for (int i = 0; i < 8; ++i) {
			if (node->m_children[i] != nullptr) {
				freeOctreeRec(node->m_children[i]);
			}
		}
		delete node->m_children;
	}
	delete node->indices;
	delete node;
}

}