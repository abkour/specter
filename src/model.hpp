#pragma once

#include "aabb.hpp"
#include "material.hpp"
#include "material_emissive.hpp"
#include "material_lambertian.hpp"
#include "material_metal.hpp"
#include "ray.hpp"
#include "timer.hpp"

namespace specter {

enum class MaterialType : uint8_t {
	Metal,
	Dielectric,
	Semiconductor	// Not supported yet
};

// Auxillary structure for code readability
struct FaceElement {
	unsigned p;	// Position
	unsigned n;	// Normal
	unsigned t;	// Texture Coordinate
};

struct MeshIndexTable {
	MeshIndexTable() 
		: m(0), f(0)
	{}
	uint32_t m, f;
};

struct MeshAttributeSizes {
	MeshAttributeSizes()
		: fsize(0)
	{}
	uint32_t fsize;
};

// A model is a class that combines multiple meshes.
class Model {

public:

	Model() {}

	Model(Model&& other) noexcept {}

	~Model();

	void parse(const char* filename);

	specter::vec3f* GetVertices() {
		return vertices.data();
	}

	specter::vec3f* GetNormals() {
		return normals.data();
	}

	specter::vec2f* GetUVs() {
		return uvs.data();
	}

	FaceElement* GetFaces() {
		return faces.data();
	}

	std::shared_ptr<specter::Material>* GetMaterials() {
		return materials.data();
	}

	specter::vec3f GetVertex(const uint32_t vertex_index) const {
		return vertices[vertex_index];
	}

	specter::vec3f GetNormal(const uint32_t normal_index) const {
		return normals[normal_index];
	}

	specter::vec2f GetUV(const uint32_t uv_index) const {
		return uvs[uv_index];
	}

	FaceElement GetFace(const uint32_t face_index) const {
		return faces[face_index];
	}

	std::shared_ptr<Material> GetMaterial(const uint32_t mesh_index) const {
		uint32_t mat_index = mesh_indices[mesh_index].m;
		return materials[mat_index];
	}

	std::shared_ptr<specter::Material>& GetMaterial(const uint32_t i) {
		return materials[i];
	}

	int GetMeshIndex(const int face_index) const {
		for (int i = 0; i < mesh_indices.size(); ++i) {
			if (	face_index >= mesh_indices[i].f 
				&&	face_index <= mesh_indices[i].f + mesh_attribute_sizes[i].fsize) 
			{
				return i;
			}
		}
		return -1;
	}

	std::string GetMeshName(const int mesh_index) const {
		return meshNames[mesh_index];
	}

	FaceElement& GetFaces(const uint32_t mesh_index) {
		return faces[mesh_indices[mesh_index].f];
	}

	std::size_t GetVertexCount() const {
		return vertices.size();
	}

	std::size_t GetMaterialCount() const {
		return materials.size();
	}

	std::size_t GetFaceCount() const {
		return faces.size();
	}

	std::size_t GetFaceCountOfMesh(const uint32_t mesh_index) const {
		return mesh_attribute_sizes[mesh_index].fsize;
	}

	specter::AxisAlignedBoundingBox GetBoundingBox() {
		return bbox;
	}

	specter::AxisAlignedBoundingBox computeBoundingBox()
	{
		specter::vec3f bmin(std::numeric_limits<float>::max());
		specter::vec3f bmax(std::numeric_limits<float>::min());

		for (int i = 0; i < vertices.size(); ++i) {
			bmin.x = vertices[i].x < bmin.x ? vertices[i].x : bmin.x;
			bmin.y = vertices[i].y < bmin.y ? vertices[i].y : bmin.y;
			bmin.z = vertices[i].z < bmin.z ? vertices[i].z : bmin.z;

			bmax.x = vertices[i].x > bmax.x ? vertices[i].x : bmax.x;
			bmax.y = vertices[i].y > bmax.y ? vertices[i].y : bmax.y;
			bmax.z = vertices[i].z > bmax.z ? vertices[i].z : bmax.z;
		}

		return specter::AxisAlignedBoundingBox(bmin, bmax);
	}

	std::size_t GetNumberOfMeshes() const {
		return nMeshes;
	}

	uint32_t GetMeshIndexFromFace(uint32_t face_index) const {
		face_index *= 3;
		for (int i = 0; i < nMeshes; ++i) {
			if (face_index < mesh_indices[i].f + mesh_attribute_sizes[i].fsize) {
				return i;
			}
		}
		return std::numeric_limits<uint32_t>::infinity();
	}

	// Implements the möller&trumbore algorithm.
	// For implementation reference: Real-time rendering 4th ed, 22.8 Ray/Triangle Intersection
	bool rayIntersection(const specter::Ray& ray, const std::size_t index, float& u, float& v, float& t) const {
		const float epsilon = 0.0000001;
		const unsigned i0 = faces[index * 3 + 0].p, i1 = faces[index * 3 + 1].p, i2 = faces[index * 3 + 2].p;
		const specter::vec3f v0 = vertices[i0], v1 = vertices[i1], v2 = vertices[i2];

		specter::vec3f e0 = v1 - v0;
		specter::vec3f e1 = v2 - v0;

		specter::vec3f q = cross(ray.d, e1);
		float a = specter::dot(e0, q);

		if (a > -epsilon && a < epsilon) {
			return false;
		}

		float f = 1.f / a;
		specter::vec3f s = ray.o - v0;
		u = f * specter::dot(s, q);

		if (u < 0.f) return false;

		specter::vec3f r = cross(s, e0);
		v = f * specter::dot(ray.d, r);

		if (v < 0.f || u + v > 1.f) return false;

		t = f * specter::dot(e1, r);
		return true;
	}

protected:

	// Private implementation functions
	using MaterialMap = std::vector<std::pair<std::string, uint32_t>>;
	
	void parseMaterialLibrary(const char* filename, MaterialMap& mtl_map);

protected:

	std::size_t nMeshes;
	std::vector<MeshAttributeSizes> mesh_attribute_sizes;
	std::vector<MeshIndexTable> mesh_indices;

	std::vector<std::string> meshNames;
	std::vector<specter::vec3f> vertices;
	std::vector<specter::vec3f> normals;
	
	std::vector<specter::vec2f> uvs;
	std::vector<unsigned char*> texture_data;

	std::vector<FaceElement> faces;
	std::vector<std::shared_ptr<specter::Material>> materials;

	specter::AxisAlignedBoundingBox bbox;
};

}