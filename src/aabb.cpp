#include "aabb.hpp"

namespace specter {

AxisAlignedBoundingBox::AxisAlignedBoundingBox() 
	: min(vec3f(0.f))
	, max(vec3f(0.f))
{}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const vec3f& bmin, const vec3f& bmax)
	: min(bmin)
	, max(bmax)
{}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const AxisAlignedBoundingBox& other)
	: min(other.min)
	, max(other.max)
{}

AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator=(const AxisAlignedBoundingBox& other) {
	min = other.min;
	max = other.max;
	return *this;
}

bool AxisAlignedBoundingBox::operator==(const AxisAlignedBoundingBox& other) const {
	return min == other.min && max == other.max;
}

bool AxisAlignedBoundingBox::operator!=(const AxisAlignedBoundingBox& other) const {
	return min != other.min || max != other.max;
}

bool AxisAlignedBoundingBox::isValid() const {
	return min < max;
}

bool AxisAlignedBoundingBox::isCollapsed() const {
	return min == max;
}


bool AxisAlignedBoundingBox::contains(const vec3f& point) const {
	return point < max&& point > min;
}

bool AxisAlignedBoundingBox::containsEdgeInclusive(const vec3f& point) const {
	return point <= max && point >= min;
}

bool AxisAlignedBoundingBox::overlaps(const AxisAlignedBoundingBox& other) const {
	return min < other.max && max > other.min;
}

bool AxisAlignedBoundingBox::overlapsEdgeInclusive(const AxisAlignedBoundingBox& other) const {
	return min <= other.max && max >= other.min;
}

bool AxisAlignedBoundingBox::rayIntersect(const Ray& ray, float& nearT, float& farT) const {
	nearT = -std::numeric_limits<float>::infinity();
	farT = std::numeric_limits<float>::infinity();

	for (int i = 0; i < 3; i++) {
		float origin = ray.o[i];
		float minVal = min[i];
		float maxVal = max[i];

		if (ray.d[i] == 0) {
			if (origin < minVal || origin > maxVal)
				return false;
		}
		else {
			float t1 = (minVal - origin) * ray.invd[i];
			float t2 = (maxVal - origin) * ray.invd[i];

			if (t1 > t2)
				std::swap(t1, t2);

			nearT = std::max(t1, nearT);
			farT = std::min(t2, farT);

			if (!(nearT <= farT))
				return false;
		}
	}

	return true;
}

vec3f AxisAlignedBoundingBox::center() const {
	return vec3f((min + max) / 2.f);
}

float AxisAlignedBoundingBox::surfaceArea() const {
	const vec3f axesLengths = max - min;
	return axesLengths.x * axesLengths.y * 2 + axesLengths.x * axesLengths.z * 2 + axesLengths.y * axesLengths.z * 2;
}

float AxisAlignedBoundingBox::volume() const {
	return product(max - min);
}

std::ostream& operator<<(std::ostream& os, const AxisAlignedBoundingBox& aabb) {
	return os << "bmin: (" << aabb.min << ")\tbmax: (" << aabb.max << ")\n";
}


}