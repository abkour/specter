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

void Octree::build(std::shared_ptr<Model>& model) {
	root = new Node;
	// This creates an invalid bbox that is then corrected in the loop
	root->bbox = model->computeBoundingBox();
	root->nTriangles = model->GetFaceCount() / 3;

	std::vector<uint32_t> initialIndexList(root->nTriangles);
	std::iota(initialIndexList.begin(), initialIndexList.end(), 0);

	maxDepth = log8(root->nTriangles);
	buildRec(root, model->GetVertices(), model->GetFaces(), initialIndexList.data(), 0);
}

void Octree::buildRec(Node* node, const vec3f* vertices, const FaceElement* faces, const uint32_t* trianglePositions, int depth) {
	if (node == nullptr) {
		throw std::runtime_error("Node is nullptr!");
	}

	if (node->nTriangles == 0) {
		return;
	}

	if (node->nTriangles <= 10 || depth > maxDepth) {
		node->tri_indices = new uint32_t[node->nTriangles];
		std::memcpy(node->tri_indices, trianglePositions, sizeof(uint32_t) * node->nTriangles);
		return;
	}

	std::vector<AxisAlignedBoundingBox> subRegions(nSubRegions);
	for (int i = 0; i < nSubRegions; ++i) {
		subRegions[i] = constructSubRegion(node->bbox, i);
	}

	std::vector<std::vector<uint32_t>> subRegionTriangleCounts(nSubRegions);
	for (int i = 0; i < node->nTriangles; i++) {
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
	
	tbb::parallel_for(tbb::blocked_range<int>(0, nSubRegions),
		[&](const tbb::blocked_range<int>& r) {
			for(int i = r.begin(); i != r.end(); ++i) {
				if (subRegionTriangleCounts[i].size() != 0) {
					node->m_children[i] = new Node;
					node->m_children[i]->bbox.min = subRegions[i].min;
					node->m_children[i]->bbox.max = subRegions[i].max;
					node->m_children[i]->nTriangles = subRegionTriangleCounts[i].size();
					buildRec(node->m_children[i], vertices, faces, subRegionTriangleCounts[i].data(), depth + 1);
				}
				else {
					node->m_children[i] = nullptr;
					node->nTriangles = std::numeric_limits<unsigned int>::max();
				}
			}
		});
}

bool Octree::traverse(const Model* model, const Ray& ray, Intersection& intersection) const {
	traverseRec(model, root, ray, intersection);
	return intersection.t != std::numeric_limits<float>::max();
}

struct IndexDistancePair {
	int index;
	float distance = std::numeric_limits<float>::max();

	friend bool operator<(const IndexDistancePair& p0, const IndexDistancePair& p1) {
		return p0.distance < p1.distance;
	}
};

static float avgNumIndices = 0;
static float nLeafsVisited = 0;

// On intersection t stores the distance to the closest triangle it collided with.
// u and v store the barycentric coordinates of that intersection point
// index refers to the triangle in the indices array of the mesh class.
// The algorithm sorts the sub-regions according to their intersection distances with the ray.
// The ray then recursively travels through the sub-regions in ascending order with respect to the intersection distance.
// If any intersection with a triangle is found on leaf encounter, it is guaranteed to be the closest triangle,
// and further processing can stop.
void Octree::traverseRec(const Model* model, Node* node, const Ray& ray, Intersection& intersection, bool multipleBoxesHit) const {
	if (node == nullptr) {
		return;
	}

	// Closest triangle found and further processing can stop.
	if (intersection.t != std::numeric_limits<float>::max() && multipleBoxesHit) {
		return;
	}

	if (node->tri_indices != nullptr) {
		for (int i = 0; i < node->nTriangles; ++i) {
			float u, v, t = std::numeric_limits<float>::max();
			if (model->rayIntersection(ray, node->tri_indices[i], u, v, t)) {
				if (intersection.t > t) {
					// Success, closest triangle found. Any further processing can stop.
					const unsigned i0 = model->GetFace(node->tri_indices[i] * 3).p;
					const unsigned i1 = model->GetFace(node->tri_indices[i] * 3 + 1).p;
					const unsigned i2 = model->GetFace(node->tri_indices[i] * 3 + 2).p;
					const vec3f& v0 = model->GetVertex(i0);
					const vec3f& v1 = model->GetVertex(i1);
					const vec3f& v2 = model->GetVertex(i2);
					const vec3f e0 = v1 - v0;
					const vec3f e1 = v2 - v0;
					intersection.n = normalize(cross(e0, e1));
					intersection.t = t;
					intersection.u = u;
					intersection.v = v;
					intersection.f = node->tri_indices[i];
					intersection.p = ray.o + t * ray.d;
					auto meshIndex = model->GetMeshIndexFromFace(node->tri_indices[i]);
					intersection.mat_ptr = model->GetMaterial(meshIndex);
				}
			}
		}
		avgNumIndices += node->nTriangles;
		nLeafsVisited += 1.f;
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
						traverseRec(model, node->m_children[distanceToBoxes[i + 0].index], ray, intersection, false);
						traverseRec(model, node->m_children[distanceToBoxes[i + 1].index], ray, intersection, true);	// Prevent early termination
					} else {
						traverseRec(model, node->m_children[distanceToBoxes[i].index], ray, intersection);
					}
				} else {
					traverseRec(model, node->m_children[distanceToBoxes[i].index], ray, intersection);
				}
			} else {
				// We can stop now, since subsequent distances will be MAX_FLOAT as well.
				break;
			}
		}
	}
}

void Octree::dbg_print() {
	std::cout << "average number of indices: " << avgNumIndices / nLeafsVisited << '\n';
}

bool Octree::traverseAny(const Model* model, const Ray& ray) const {
	bool intersectionFound = false;
	traverseAnyRec(model, root, ray, intersectionFound);
	return intersectionFound;
}

void Octree::traverseAnyRec(const Model* model, Node* node, const Ray& ray, bool& intersectionFound) const {
	if (node == nullptr) {
		return;
	}

	// Any triangle found, therefore further processing can stop.
	if (intersectionFound) {
		return;
	}

	if (node->tri_indices != nullptr) {
		for (int i = 0; i < node->nTriangles; ++i) {
			float uu, vv, tt;
			if (model->rayIntersection(ray, node->tri_indices[i], uu, vv, tt)) {
				// Rays with origins inside the space represented by the octree can 
				// have intersections with geometry, where the ray parameter is negative.
				// This occurs, when the ray is colliding with objects that are in opposite 
				// direction of the ray direction vector. 
				// We don't want to consider this case at all.
				if (tt > 0.f) {
					intersectionFound = true;
				}
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
					traverseAnyRec(model, node->m_children[i], ray, intersectionFound);
				}
			}
		}
	}
}

bool Octree::traverseAnyTmax(const Model* model, const Ray& ray, const float t_max) const {
	bool intersectionFound = false;
	traverseAnyTmaxRec(model, root, ray, t_max, intersectionFound);
	return intersectionFound;
}

void Octree::traverseAnyTmaxRec(const Model* model, Node* node, const Ray& ray, const float t_max, bool& intersectionFound) const {
	if (node == nullptr) {
		return;
	}

	// Any triangle found, therefore further processing can stop.
	if (intersectionFound) {
		return;
	}

	if (node->tri_indices != nullptr) {
		for (int i = 0; i < node->nTriangles; ++i) {
			float uu, vv, tt;
			if (model->rayIntersection(ray, node->tri_indices[i], uu, vv, tt)) {
				// Rays with origins inside the space represented by the octree can 
				// have intersections with geometry, where the ray parameter is negative.
				// This occurs, when the ray is colliding with objects that are in opposite 
				// direction of the ray direction vector. 
				// We don't want to consider this case at all.
				if (tt > 0.f && tt < t_max) {
					intersectionFound = true;
				}
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
					traverseAnyTmaxRec(model, node->m_children[i], ray, t_max, intersectionFound);
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
	delete node->tri_indices;
	delete node;
}

}