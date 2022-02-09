#pragma once
#include "aabb.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

namespace specter {

struct Mesh {

	vec3f* getVertices();
	vec3f* getNormals();
	vec2f* getTextureCoordinates();
	vec3f* getFaces();

	std::size_t getTriangleCount() const;
	std::size_t getVertexCount() const;

	AxisAlignedBoundingBox computeBoundingBox() const;

protected:

	std::size_t nVertices;
	std::size_t nFaces;

	vec3f* vertices;
	vec3f* normals;
	vec2f* textureCoordinates;

	vec3f* faces;
};

vec3f* Mesh::getVertices() {
	return vertices;
}

vec3f* Mesh::getNormals() {
	return normals;
}

vec2f* Mesh::getTextureCoordinates() {
	return textureCoordinates;
}

vec3f* Mesh::getFaces() {
	return faces;
}

std::size_t Mesh::getTriangleCount() const {
	return nFaces;
}

std::size_t Mesh::getVertexCount() const {
	return nVertices;
}

AxisAlignedBoundingBox Mesh::computeBoundingBox() const {
	vec3f bmin(std::numeric_limits<float>::max());
	vec3f bmax(std::numeric_limits<float>::min());
	
	for (int i = 0; i < nVertices; ++i) {
		bmin.x = vertices[i].x < bmin.x ? vertices[i].x : bmin.x;
		bmin.y = vertices[i].y < bmin.y ? vertices[i].y : bmin.y;
		bmin.z = vertices[i].z < bmin.z ? vertices[i].z : bmin.z;

		bmax.x = vertices[i].x > bmax.x ? vertices[i].x : bmax.x;
		bmax.y = vertices[i].y > bmax.y ? vertices[i].y : bmax.y;
		bmax.z = vertices[i].z > bmax.z ? vertices[i].z : bmax.z;
	}

	return AxisAlignedBoundingBox(bmin, bmax);
}


}