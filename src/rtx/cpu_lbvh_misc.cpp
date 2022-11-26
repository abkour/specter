#include "cpu_lbvh_helpers.hpp"

namespace specter {

AxisAlignedBoundingBox constructAABBFromPaddedTriangle(const vec3f& v0, const vec3f& v1, const vec3f& v2) {
	vec3f tmin(v0);
	vec3f tmax(v0);
	tmax.x = (v1.x > v0.x) ? ((v1.x > v2.x) ? v1.x : v2.x) : ((v2.x > v0.x) ? v2.x : v0.x);
	tmax.y = (v1.y > v0.y) ? ((v1.y > v2.y) ? v1.y : v2.y) : ((v2.y > v0.y) ? v2.y : v0.y);
	tmax.z = (v1.z > v0.z) ? ((v1.z > v2.z) ? v1.z : v2.z) : ((v2.z > v0.z) ? v2.z : v0.z);
	tmin.x = (v1.x < v0.x) ? ((v1.x < v2.x) ? v1.x : v2.x) : ((v2.x < v0.x) ? v2.x : v0.x);
	tmin.y = (v1.y < v0.y) ? ((v1.y < v2.y) ? v1.y : v2.y) : ((v2.y < v0.y) ? v2.y : v0.y);
	tmin.z = (v1.z < v0.z) ? ((v1.z < v2.z) ? v1.z : v2.z) : ((v2.z < v0.z) ? v2.z : v0.z);
	return AxisAlignedBoundingBox(tmin, tmax);
}

void radixsort(PrimitiveIdentifier* ids, std::size_t n, uint32_t mask) {
	if (n <= 1) return;

	uint32_t nElements = n;
	int i = 0;
	while (i < n) {
		if (ids[i].mortonCode & mask) {
			std::swap(ids[i], ids[n - 1]);
			--n;
		} else {
			++i;
		}
	}

	if (mask != 1) {
		radixsort(ids, i, mask >> 1);
		radixsort(&ids[i], nElements - n, mask >> 1);
	}
}

unsigned expandBits(unsigned v) {
	v = (v * 0x00010001u) & 0xFF0000FFu;
	v = (v * 0x00000101u) & 0x0F00F00Fu;
	v = (v * 0x00000011u) & 0xC30C30C3u;
	v = (v * 0x00000005u) & 0x49249249u;
	return v;
}

void computeMortonCode(PrimitiveIdentifier* pIds, AxisAlignedBoundingBox* aabbs, unsigned int idx, const vec3f& nStart, const vec3f& nEnd) {
	vec3f mappedInput = aabbs[idx].center();
	// map the bounding box centroid to the range [0, 1]
	mappedInput = (1.f / (nEnd - nStart)) * (mappedInput - nStart);

	// Furthermore, map from [0, 1] to [0, 1023]
	mappedInput.x = std::min(std::max(mappedInput.x * 1024.f, 0.f), 1023.f);
	mappedInput.y = std::min(std::max(mappedInput.y * 1024.f, 0.f), 1023.f);
	mappedInput.z = std::min(std::max(mappedInput.z * 1024.f, 0.f), 1023.f);
	
	unsigned x = expandBits((unsigned)mappedInput.x);
	unsigned y = expandBits((unsigned)mappedInput.y);
	unsigned z = expandBits((unsigned)mappedInput.z);

	pIds[idx].objectId = idx;
	pIds[idx].mortonCode = x * 4 + y * 2 + z;
}


}