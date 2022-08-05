#pragma once
#include "ray.hpp"
#include "vec3.hpp"

namespace specter {

static int xx = 0;

struct AxisAlignedBoundingBox {

	// Constructs a collapsed AABB
	AxisAlignedBoundingBox();
	
	// Constructs a bounding box spanning [bmin, bmax]
	AxisAlignedBoundingBox(const vec3f& bmin, const vec3f& bmax);

	AxisAlignedBoundingBox(const AxisAlignedBoundingBox& other);
	
	AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& other);

	// Compares if two bounding boxes occupy the same space
	bool operator==(const AxisAlignedBoundingBox& other) const;

	// Compares if two bounding boxes occupy different spaces
	bool operator!=(const AxisAlignedBoundingBox& other) const;

	// Returns the validity of the bounding box. Returns false if min > max. The bounding box is ill-defined.
	bool isValid() const;
	// Returns true if the bbox has volume 0
	bool isCollapsed() const;
	// Returns true if point is confined within (min, max)
	bool contains(const vec3f& point) const;
	// Returns true if point is confined within [min, max]
	bool containsEdgeInclusive(const vec3f& point) const;
	bool containsEdgeInclusive(const AxisAlignedBoundingBox& point) const;
	// Returns true if two aabbs overlap in space.
	bool overlaps(const AxisAlignedBoundingBox& other) const;
	// Additionally, returns true if two edges/faces overlap
	bool overlapsEdgeInclusive(const AxisAlignedBoundingBox& other) const;
	
	// Returns true if the ray collides with the bounding box along it's travelling direction
	bool rayIntersect(const Ray& ray, float& nearT, float& farT) const {
		nearT = std::numeric_limits<float>::min();
		farT = std::numeric_limits<float>::max();

		for (int i = 0; i < 3; i++) {
			float origin = ray.o[i];
			float minVal = min[i];
			float maxVal = max[i];

			// If the ray is parallel to the bounding box, return early.
			if (ray.d[i] == 0) {
				//xx++;
				if (origin < minVal || origin > maxVal)
					return false;
			} else {
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

	bool rayIntersectv2(const Ray& ray, float& nearT, float& farT) const {
		vec3f t0 = (min - ray.o) * ray.invd;
		vec3f t1 = (max - ray.o) * ray.invd;
		vec3f tmin = minv(t0, t1), tmax = maxv(t0, t1);

		nearT = maxComponent(tmin);
		farT = minComponent(tmax);

		return nearT <= farT;
	}

	// Return the surface area of the bounding box
	float surfaceArea() const;
	// Return the volume of the bounding box
	float volume() const;
	// Return the center of the bounding box
	vec3f center() const;
	
	vec3f min, max;
};

std::ostream& operator<<(std::ostream& os, const AxisAlignedBoundingBox& aabb);

AxisAlignedBoundingBox combine(const AxisAlignedBoundingBox& box0, const AxisAlignedBoundingBox& box1);

void manyAABBRayIntersect(const AxisAlignedBoundingBox* bboxes, const Ray& ray, float* near, const int count);


// Experimental SoA implementation of AABB. Currently used in the Octree data structure.
struct sAABB {

	// Additionally, returns true if two edges/faces overlap
	bool overlapsEdgeInclusive(const AxisAlignedBoundingBox& other, const int i) const {
		//return min <= other.max && max >= other.min;

		return minx[i] <= other.max.x && maxx[i] >= other.min.x
			&& miny[i] <= other.max.y && maxy[i] >= other.min.y
			&& minz[i] <= other.max.z && maxz[i] >= other.min.z;
	}

	bool rayIntersect(const Ray& r, float& nearT, float& farT, const int i) const {
		vec3f bmin(minx[i], miny[i], minz[i]);
		vec3f bmax(maxx[i], maxy[i], maxz[i]);

		vec3f t0 = (bmin - r.o) * r.invd;
		vec3f t1 = (bmax - r.o) * r.invd;
		vec3f tmin = minv(t0, t1), tmax = maxv(t0, t1);

		nearT = maxComponent(tmin);
		farT = minComponent(tmax);

		return nearT <= farT;
	}

	// Performs ray-aabb intersection against 8 AABBs. AVX2 is necessary.
	// Currently specter is compiled with AVX2 enabled. If the target 
	// architecture does not feature AVX2, the program will crash.
	// In the future, I will add a switch in the accel structure, to default
	// to SSE3.
	void rayIntersect(const Ray& r, float* nearT, float* farT) const {
		// X component
		
		__m256 ROX = _mm256_broadcast_ss(&r.o.x);
		__m256 ROY = _mm256_broadcast_ss(&r.o.y);
		__m256 ROZ = _mm256_broadcast_ss(&r.o.z);
		__m256 RIDX = _mm256_broadcast_ss(&r.invd.x);
		__m256 RIDY = _mm256_broadcast_ss(&r.invd.y);
		__m256 RIDZ = _mm256_broadcast_ss(&r.invd.z);
		__m256 NEART = _mm256_load_ps(nearT);
		__m256 FART = _mm256_load_ps(farT);

		__m256 YMM8 = _mm256_load_ps(minx);
		__m256 YMM9 = _mm256_load_ps(maxx);

		__m256 T0 = _mm256_sub_ps(YMM8, ROX);
		__m256 T1 = _mm256_sub_ps(YMM9, ROX);
		T0 = _mm256_mul_ps(T0, RIDX);
		T1 = _mm256_mul_ps(T1, RIDX);

		YMM8 = _mm256_min_ps(T0, T1);
		YMM9 = _mm256_max_ps(T0, T1);

		NEART = _mm256_max_ps(YMM8, NEART);
		FART = _mm256_min_ps(YMM9, FART);

		// Y Component
		YMM8 = _mm256_load_ps(miny);
		YMM9 = _mm256_load_ps(maxy);

		T0 = _mm256_sub_ps(YMM8, ROY);
		T1 = _mm256_sub_ps(YMM9, ROY);
		T0 = _mm256_mul_ps(T0, RIDY);
		T1 = _mm256_mul_ps(T1, RIDY);

		YMM8 = _mm256_min_ps(T0, T1);
		YMM9 = _mm256_max_ps(T0, T1);

		NEART = _mm256_max_ps(YMM8, NEART);
		FART = _mm256_min_ps(YMM9, FART);

		// Z Component 
		YMM8 = _mm256_load_ps(minz);
		YMM9 = _mm256_load_ps(maxz);

		T0 = _mm256_sub_ps(YMM8, ROZ);
		T1 = _mm256_sub_ps(YMM9, ROZ);
		T0 = _mm256_mul_ps(T0, RIDZ);
		T1 = _mm256_mul_ps(T1, RIDZ);

		YMM8 = _mm256_min_ps(T0, T1);
		YMM9 = _mm256_max_ps(T0, T1);

		NEART = _mm256_max_ps(YMM8, NEART);
		FART = _mm256_min_ps(YMM9, FART);

		// Output to nearT and farT
		_mm256_store_ps(nearT, NEART);
		_mm256_store_ps(farT, FART);
	}

	struct alignas(32) {
		float minx[8], maxx[8];
		float miny[8], maxy[8];
		float minz[8], maxz[8];
	};
};

}
