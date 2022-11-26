#pragma once 
#include "../math/vec4.hpp"

#include "aabb.hpp"

#include <utility>

namespace specter {

struct PrimitiveIdentifier {
	int objectId;
	unsigned mortonCode;
	friend void swap(PrimitiveIdentifier& p0, PrimitiveIdentifier& p1) {
		using std::swap;
		swap(p0.objectId, p1.objectId);
		swap(p0.mortonCode, p1.mortonCode);
	}
};

AxisAlignedBoundingBox constructAABBFromPaddedTriangle(const vec3f& v0, const vec3f& v1, const vec3f& v2);

unsigned expandBits(unsigned u);

void computeMortonCode(PrimitiveIdentifier* pIds, AxisAlignedBoundingBox* aabbs, unsigned int idx, const vec3f& nStart, const vec3f& nEnd);

void radixsort(PrimitiveIdentifier* ids, std::size_t n, uint32_t mask = 0x8000'0000);

}