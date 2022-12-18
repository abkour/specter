#include "model.hpp"

namespace specter {

// Implements the m�ller&trumbore algorithm.
// For implementation reference: Real-time rendering 4th ed, 22.8 Ray/Triangle Intersection
bool Model::rayIntersection(const specter::Ray& ray, const std::size_t index, float& u, float& v, float& t) const {
    const float epsilon = 1e-7;
    const unsigned i0 = faces[index * 3 + 0].p;
    const unsigned i1 = faces[index * 3 + 1].p;
    const unsigned i2 = faces[index * 3 + 2].p;
    const specter::vec3f v0 = vertices[i0];
    const specter::vec3f v1 = vertices[i1];
    const specter::vec3f v2 = vertices[i2];

    const specter::vec3f e0 = v1 - v0;
    const specter::vec3f e1 = v2 - v0;

    const specter::vec3f q = specter::cross(ray.d, e1);
    const float a = specter::dot(e0, q);

    if (a > -epsilon && a < epsilon) {
        return false;
    }

    const float f = 1.f / a;
    const specter::vec3f s = ray.o - v0;
    u = f * specter::dot(s, q);

    if (u < 0.f) return false;

    const specter::vec3f r = cross(s, e0);
    v = f * specter::dot(ray.d, r);

    if (v < 0.f || u + v > 1.f) return false;

    t = f * specter::dot(e1, r);
    return true;
}

std::vector<float> Model::get_interleaved_data() {
    std::vector<float> interleaved_data;
    std::set<unsigned> unique_vertex_indices;
    const int a = 14;
    interleaved_data.resize(vertices.size() * a);
    for(auto& f : faces) {
        auto [pos, accept] = unique_vertex_indices.emplace(f.p);
        if(accept) {
            interleaved_data[f.p * a] = vertices[f.p].x;
            interleaved_data[f.p * a + 1] = vertices[f.p].y;
            interleaved_data[f.p * a + 2] = vertices[f.p].z;
            interleaved_data[f.p * a + 3] = normals[f.n].x;
            interleaved_data[f.p * a + 4] = normals[f.n].y;
            interleaved_data[f.p * a + 5] = normals[f.n].z;
            interleaved_data[f.p * a + 6] = tangent_vectors[f.n].x;
            interleaved_data[f.p * a + 7] = tangent_vectors[f.n].y;
            interleaved_data[f.p * a + 8] = tangent_vectors[f.n].z;
            interleaved_data[f.p * a + 9] = bitangent_vectors[f.n].x;
            interleaved_data[f.p * a + 10] = bitangent_vectors[f.n].y;
            interleaved_data[f.p * a + 11] = bitangent_vectors[f.n].z;
            interleaved_data[f.p * a + 12] = uvs[f.t].x;
            interleaved_data[f.p * a + 13] = uvs[f.t].y;
        }
    }

    return interleaved_data;
}

}