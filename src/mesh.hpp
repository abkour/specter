#pragma once
#include "aabb.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include <vector>

namespace specter {

// This will be constructed by different file loaders. For now only the .obj loader is implemented.
struct Mesh {

	virtual ~Mesh();

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

	std::size_t nVertices;
	std::size_t nFaces;

	std::vector<vec3f> vertices;
	std::vector<vec3f> normals;
	std::vector<vec2f> textureCoordinates;

	std::vector<vec3u> faces;
};

}