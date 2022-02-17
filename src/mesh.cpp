#include "mesh.hpp"

namespace specter {


static int _debug_nhits = 0;

// Implements the möller&trumbore algorithm.
// For implementation reference: Real-time rendering 4th ed, 22.8 Ray/Triangle Intersection
bool Mesh::rayIntersection(const Ray& ray, const std::size_t index, float& u, float& v, float& t) const {
	const float epsilon = 0.0000001;
	const unsigned i0 = faces[index * 3 + 0].p, i1 = faces[index * 3 + 1].p, i2 = faces[index * 3 + 2].p;
	const vec3f v0 = vertices[i0], v1 = vertices[i1], v2 = vertices[i2];

	vec3f e0 = v1 - v0;
	vec3f e1 = v2 - v0;

	vec3f q = cross(ray.d, e1);
	float a = dot(e0, q);

	if (a > -epsilon && a < epsilon) {
		return false;
	}

	float f = 1.f / a;
	vec3f s = ray.o - v0;
	u = f * dot(s, q);

	if (u < 0.f) return false;

	vec3f r = cross(s, e0);
	v = f * dot(ray.d, r);

	if (v < 0.f || u + v > 1.f) return false;

	t = f * dot(e1, r);
	return true;
}

vec3f Mesh::getVertex(const std::size_t i) const {
	return vertices[i];
}

vec3f Mesh::getNormal(const std::size_t i) const {
	return normals[i];
}

FaceElement Mesh::getFace(const std::size_t i) const {
	return faces[i];
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

FaceElement* Mesh::getFaces() {
	return faces.data();
}

std::size_t Mesh::getNormalCount() const {
	return normals.size();
}

std::size_t Mesh::getTextureCoordinateCount() const {
	return textureCoordinates.size();
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

std::ostream& operator<<(std::ostream& os, const FaceElement& face) {
	return os << "Pos: " << face.p << " Normal: " << face.n << " UV: " << face.t;
}

}