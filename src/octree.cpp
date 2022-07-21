#include "octree.hpp"

#include <numeric>	// For std::iota

namespace specter {

#define nMaxRayAABBIntersections (4)
#define nSubRegions (8)

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

static void constructSubRegion(const AxisAlignedBoundingBox& superRegion, sAABB* minorRegion, int i) {
	
	auto bmin = superRegion.min;
	auto bmax = superRegion.max;
	
	vec3f hd = (bmax - bmin) / 2;	// Half-way inbetween min and max
	AxisAlignedBoundingBox subregion;
	switch (i) {
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

	minorRegion->minx[i] = subregion.min.x;
	minorRegion->miny[i] = subregion.min.y;
	minorRegion->minz[i] = subregion.min.z;

	minorRegion->maxx[i] = subregion.max.x;
	minorRegion->maxy[i] = subregion.max.y;
	minorRegion->maxz[i] = subregion.max.z;
}

static void constructSubRegion(const sAABB* superRegion, int i, sAABB* minorRegion, int k) {

	auto bmin = vec3f(superRegion->minx[i], superRegion->miny[i], superRegion->minz[i]);
	auto bmax = vec3f(superRegion->maxx[i], superRegion->maxy[i], superRegion->maxz[i]);

	vec3f hd = (bmax - bmin) / 2;	// Half-way inbetween min and max
	AxisAlignedBoundingBox subregion;
	switch (k) {
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

	minorRegion->minx[k] = subregion.min.x;
	minorRegion->miny[k] = subregion.min.y;
	minorRegion->minz[k] = subregion.min.z;

	minorRegion->maxx[k] = subregion.max.x;
	minorRegion->maxy[k] = subregion.max.y;
	minorRegion->maxz[k] = subregion.max.z;
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
	
	AxisAlignedBoundingBox modelbox = model->computeBoundingBox();
	
	root->nTriangles = model->GetFaceCount() / 3;
	root->subboxes = new sAABB;
	for (int i = 0; i < nSubRegions; ++i) {
		constructSubRegion(modelbox, root->subboxes, i);
	}

	std::vector<uint32_t> initialIndexList(root->nTriangles);
	std::iota(initialIndexList.begin(), initialIndexList.end(), 0);

	maxDepth = log8(root->nTriangles);
	buildRec(root, model->GetVertices(), model->GetFaces(), initialIndexList.data(), 0);
}

void Octree::buildRec(Node* node, const vec3f* vertices, const FaceElement* faces, const uint32_t* trianglePositions, int depth) {
	if (node->nTriangles == 0) {
		return;
	}

	if (node->nTriangles <= 10 || depth > maxDepth) {
		node->tri_indices = new uint32_t[node->nTriangles];
		std::memcpy(node->tri_indices, trianglePositions, sizeof(uint32_t) * node->nTriangles);
		return;
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
			if (node->subboxes->overlapsEdgeInclusive(triangleAABB, k)) {
				subRegionTriangleCounts[k].push_back(trianglePositions[i]);
			}
		}
	}

	node->m_children = new Node*[nSubRegions];
	for (int i = 0; i < 8; ++i) {
		if (subRegionTriangleCounts[i].size() != 0) {
			node->m_children[i] = new Node;
			node->m_children[i]->subboxes = new sAABB;
			for (int k = 0; k < 8; ++k) {
				constructSubRegion(node->subboxes, i, node->m_children[i]->subboxes, k);
			}
			node->m_children[i]->nTriangles = subRegionTriangleCounts[i].size();
			buildRec(node->m_children[i], vertices, faces, subRegionTriangleCounts[i].data(), depth + 1);
		} else {
			node->m_children[i] = nullptr;
			node->nTriangles = std::numeric_limits<unsigned int>::max();
		}
	}
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

// Implements insertion sort to sort the array
static void sortIDP(IndexDistancePair* ptr, const uint32_t num)
{
	for (int i = 1; i < num; ++i) {
		float d = ptr[i].distance;
		int j = i - 1;
		for (; j >= 0 && ptr[j].distance > d; --j) {
			ptr[j + 1].distance = ptr[j].distance;
			ptr[j + 1].index = ptr[j].index;
		}
		ptr[j + 1].distance = d;
		ptr[j + 1].index = ptr[i].index;
	}
}

static void sortIDP_swap(IndexDistancePair* ptr, const uint32_t num)
{
	for (int i = 1; i < num; ++i) {
		int j = i;
		for (; j > 0 && ptr[j - 1].distance > ptr[j].distance; --j) {
			std::swap(ptr[j], ptr[j - 1]);
		}
	}
}

void Octree::computeTriangleIntersections(const Model* model, Node* node, const Ray& ray, Intersection& its) const {
	float u, v, t = std::numeric_limits<float>::max();
	for (int i = 0; i < node->nTriangles; ++i) {
		uint32_t triIndex = node->tri_indices[i];
		if (model->rayIntersection(ray, triIndex, u, v, t)) {
			if (its.t > t && t > 0.f) {
				its.n = model->GetNormal(model->GetFace(node->tri_indices[i] * 3).n);
				its.t = t;
				its.u = u;
				its.v = v;
				its.f = node->tri_indices[i];
				its.p = ray.o + t * ray.d;
				auto meshIndex = model->GetMeshIndexFromFace(node->tri_indices[i]);
				its.mat_ptr = model->GetMaterial(meshIndex);
			}
		}
	}
}

void Octree::traverseRec(const Model* model, Node* node, const Ray& ray, Intersection& intersection, bool multipleBoxesHit) const {
	IndexDistancePair distanceToBoxes[nSubRegions];
	alignas(32) float nearT[8];
	alignas(32) float farT[8];
	for (int i = 0; i < 8; ++i) {
		nearT[i] = std::numeric_limits<float>::min();
		farT[i] = std::numeric_limits<float>::max();
	}

	node->subboxes->rayIntersect(ray, nearT, farT);

	for (int i = 0; i < 8; ++i) {
		if (nearT[i] <= farT[i]) {
			distanceToBoxes[i].distance = nearT[i];
			distanceToBoxes[i].index = i;
		}
	}

	// Sort sub-regions according to the intersection distance, in order 
	// to perform a sorted descend.
	std::sort(std::begin(distanceToBoxes), std::end(distanceToBoxes));

	for (int i = 0; i < nMaxRayAABBIntersections; ++i) {
		if (distanceToBoxes[i].distance != std::numeric_limits<float>::max()) {

			if (node->m_children[distanceToBoxes[i].index] == nullptr) {
				continue;
			}

			// In case a ray intersects two bounding boxes at the same point, the ray will 
			// only exit one of the boxes. We can find that out by repeating the 
			// intersection test on either of the boxes and reading the far value.
			// If the far value is not FLOAT_MAX, then that means the ray has entered
			// and exited the bounding box. The other bounding box doesn't have to be considered
			// at that point.
			if (distanceToBoxes[i].distance == distanceToBoxes[i + 1].distance) {
				float near, far;
				node->subboxes->rayIntersect(ray, near, far, i);
				if (far != std::numeric_limits<float>::max()) {
					if (node->m_children[distanceToBoxes[i].index]->tri_indices != nullptr) {
						computeTriangleIntersections(model, node->m_children[distanceToBoxes[i].index], ray, intersection);
					} else {
						traverseRec(model, node->m_children[distanceToBoxes[i].index], ray, intersection);
					}
				} else {
					if (node->m_children[distanceToBoxes[i + 1].index]->tri_indices != nullptr) {
						computeTriangleIntersections(model, node->m_children[distanceToBoxes[i + 1].index], ray, intersection);
					} else {
						traverseRec(model, node->m_children[distanceToBoxes[i + 1].index], ray, intersection);
					}
				}
				// We can skip the bounding box the ray is not entering by incrementing the 
				// loop counter.
				++i;
			} else {
				if (node->m_children[distanceToBoxes[i].index]->tri_indices != nullptr) {
					computeTriangleIntersections(model, node->m_children[distanceToBoxes[i].index], ray, intersection);
				} else {
					traverseRec(model, node->m_children[distanceToBoxes[i].index], ray, intersection);
				}
			}
		} else {
			// We can stop now, since subsequent distances will be MAX_FLOAT as well.
			break;
		}
	}
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
				if (node->subboxes->rayIntersect(ray, near, far, i)) {
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
				if (node->subboxes->rayIntersect(ray, near, far, i)) {
					traverseAnyTmaxRec(model, node->m_children[i], ray, t_max, intersectionFound);
				}
			}
		}
	}
}

void Octree::printNodesPerLayer() const {
	unsigned maxDepth = GetMaxDepth();
	std::unique_ptr<unsigned[]> breadths = std::make_unique<unsigned[]>(maxDepth + 1);
	MaxBreadthTraversal(root, 0, breadths.get());
	std::cout << "Output format: [Layer index, Nodes per layer]\n";
	for (int i = 0; i < maxDepth; ++i) {
		std::cout << "Layer " << i << ": " << breadths[i] << '\n';
	}
}

std::pair<Octree::layerIndex, int> Octree::GetMaxBreadth() const {
	unsigned maxDepth = GetMaxDepth();
	std::unique_ptr<unsigned[]> breadths = std::make_unique<unsigned[]>(maxDepth + 1);
	MaxBreadthTraversal(root, 0, breadths.get());
	unsigned widestLayer = 0;
	unsigned maxBreadth = 0;
	for (int i = 0; i < maxDepth; ++i) {
		if (std::max(maxBreadth, breadths[i]) > maxBreadth) {
			maxBreadth = breadths[i];
			widestLayer = i;
		}
	}
	return { widestLayer, maxBreadth };
}

void Octree::MaxBreadthTraversal(Node* node, unsigned layer, unsigned* breadth) const {
	breadth[layer]++;
	for (int i = 0; i < nSubRegions; ++i) {
		if (node->m_children == nullptr) break;
		if (node->m_children[i] != nullptr) {
			MaxBreadthTraversal(node->m_children[i], layer + 1, breadth);
		}
	}
}

unsigned Octree::GetMaxDepth() const {
	return MaxDepthTraversal(root, 0);
}

unsigned Octree::MaxDepthTraversal(Node* node, unsigned maxDepth) const {
	unsigned localMaxDepth = maxDepth;
	for (int i = 0; i < nSubRegions; ++i) {
		if (node->m_children == nullptr) {
			break;
		}
		if (node->m_children[i] != nullptr) {
			localMaxDepth = std::max(localMaxDepth, MaxDepthTraversal(node->m_children[i], maxDepth + 1));
		}
	}
	return localMaxDepth;
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
		delete node->subboxes;
		delete node->m_children;
	}
	delete node->tri_indices;
	delete node;
}

}