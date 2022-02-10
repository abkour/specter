#include "mesh.hpp"

namespace specter {

// Implements the möller&trumbore algorithm.
// For implementation reference: Real-time rendering 4th ed, 22.8 Ray/Triangle Intersection
bool Mesh::rayIntersection(Ray& ray, const std::size_t index, float& u, float& v, float& t) const {
	const float epsilon = 0.0000001;
	const vec3f v0 = vertices[faces[index * 3 + 0].x];
	const vec3f v1 = vertices[faces[index * 3 + 1].x];
	const vec3f v2 = vertices[faces[index * 3 + 2].x];
	const vec3f edge0 = v1 - v0;
	const vec3f edge1 = v2 - v0;
	
	const vec3f h = cross(ray.d, edge1);
	const float a = dot(edge0, h);
	if (a > -epsilon && a < epsilon) {
		return false;
	}

	const float f = 1.f / a;
	const vec3f s = ray.o - v0;
	u = f * dot(s, h);
	if (u < 0.f || u > 1.f) {
		return false;
	}

	const vec3f q = cross(s, edge0);
	v = f * dot(ray.d, q);
	if (v < 0.f || u + v > 1.f) {
		return false;
	}

	t = f * dot(edge1, q);
	if (t > epsilon) {
		return true;
	}
	return false;
}

vec3f Mesh::getVertex(const std::size_t i) const {
	return vertices[i];
}

vec3f Mesh::getNormal(const std::size_t i) const {
	return normals[i];
}

vec2f Mesh::getTextureCoordinate(const std::size_t i) const {
	return textureCoordinates[i];
}

vec3f* Mesh::getVertices() {
	return vertices.data();
}

vec3f* Mesh::getNormals() {
	return normals.data();
}

vec2f* Mesh::getTextureCoordinates() {
	return textureCoordinates.data();
}

vec3u* Mesh::getFaces() {
	return faces.data();
}

std::size_t Mesh::getTriangleCount() const {
	return faces.size() / 3;
}

std::size_t Mesh::getVertexCount() const {
	return vertices.size();
}

AxisAlignedBoundingBox Mesh::computeBoundingBox() const {
	vec3f bmin(std::numeric_limits<float>::max());
	vec3f bmax(std::numeric_limits<float>::min());

	for (int i = 0; i < vertices.size(); ++i) {
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