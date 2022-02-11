#pragma once
#include "aabb.hpp"
#include "ray.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include <vector>

namespace specter {

// This will be constructed by different file loaders. For now only the .obj loader is implemented.
struct Mesh {

	// Implements the m�ller&trumbore algorithm.
	// For implementation reference: Real-time rendering 4th ed, 22.8 Ray/Triangle Intersection
	bool rayIntersection(const Ray& ray, const std::size_t index, float& u, float &v, float& t) const;
	bool rayIntersectionV2(const Ray& ray, const std::size_t index, float& u, float &v, float& t) const;

	vec3f getVertex(const std::size_t i) const;
	vec3f getNormal(const std::size_t i) const;
	vec2f getTextureCoordinate(const std::size_t i) const;
	vec3u getFace(const std::size_t i) const;

	vec3f* getVertices();
	vec3f* getNormals();
	vec2f* getTextureCoordinates();
	vec3u* getFaces();

	std::size_t getTriangleCount() const;
	std::size_t getVertexCount() const;

	AxisAlignedBoundingBox computeBoundingBox() const;

protected:

	Mesh() = default;

protected:

	std::vector<vec3f> vertices;
	std::vector<vec3f> normals;
	std::vector<vec2f> textureCoordinates;

	std::vector<vec3u> faces;

	bool areNormalsPresent = false;
};

}