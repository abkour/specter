#pragma once
#include "aabb.hpp"
#include "ray.hpp"
#include "simd_math.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include <fstream>
#include <vector>

namespace specter {

// Auxillary data structure for code readability
struct FaceElement {
	unsigned p;	// Position
	unsigned n;	// Normal
	unsigned t;	// Texture Coordinate
};

std::ostream& operator<<(std::ostream& os, const FaceElement& face);

// Represents an intersection and its corresponding triangle index.
// Also contains coefficient t for ray r(t) := o + t * d
struct Intersection {
	vec2f uv;
	unsigned f;
	float t = std::numeric_limits<float>::max();
};

// This will be constructed by different file loaders. For now only the .obj loader is implemented.
struct Mesh {

	// Implements the möller&trumbore algorithm.
	// For implementation reference: Real-time rendering 4th ed, 22.8 Ray/Triangle Intersection
	bool rayIntersection(const Ray& ray, const std::size_t index, float& u, float &v, float& t) const;
	
	// Get vertex at position i 
	vec3f getVertex(const std::size_t i) const;
	// Get normal at position i
	vec3f getNormal(const std::size_t i) const;
	// Get texture cooridnate at position i
	vec2f getTextureCoordinate(const std::size_t i) const;
	// Get face at position i
	FaceElement getFace(const std::size_t i) const;

	// Get underlying vertices pointer
	vec3f* getVertices();
	// Get underlying normals pointer
	vec3f* getNormals();
	// Get underlying texture coordinates pointer
	vec2f* getTextureCoordinates();
	// Get underlying faces pointer
	FaceElement* getFaces();

	// Get number of normals in mesh
	std::size_t getNormalCount() const;
	// Get number of texture coordinates in mesh
	std::size_t getTextureCoordinateCount() const;
	// Get number of triangles in mesh
	std::size_t getTriangleCount() const;
	// Get number of vertices in mesh
	std::size_t getVertexCount() const;

	// Compute the bounding box encompassing the mesh
	AxisAlignedBoundingBox computeBoundingBox() const;

protected:

	Mesh() = default;

protected:

	std::vector<vec3f> vertices;
	std::vector<vec3f> normals;
	std::vector<vec2f> textureCoordinates;

	std::vector<FaceElement> faces;
};

}