#pragma once
#include "../rendering_types.hpp"
#include "../math/vec3.hpp"
#include <string>
#include <vector>

namespace specter::filesystem {

struct ObjMtlComponent {
	std::string name;
	std::vector<TextureType> texture_types;
	std::vector<std::string> texture_paths;
	std::vector<std::size_t> tp_hashes;
	float ns, ni, d, tr;
	vec3f tf, ka, kd, ke, ks;
};

using ObjMtlComponents = std::vector<ObjMtlComponent>;

inline std::ostream& operator<<(std::ostream& os, const ObjMtlComponent& mat_comp) {
	os << "name: " << mat_comp.name << '\n' 
			<< "ns: " << mat_comp.ns << '\n'
			<< "ni: " << mat_comp.ni << '\n'
			<< "d: "  << mat_comp.d << '\n'
			<< "tr: " << mat_comp.tr << '\n'
			<< "tf: " << mat_comp.tf << '\n'
			<< "ka: " << mat_comp.ka << '\n'
			<< "kd: " << mat_comp.kd << '\n'
			<< "ke: " << mat_comp.ke << '\n'
			<< "ks: " << mat_comp.ks << '\n';

	os << "---Printing texture paths---\n";
	for (int i = 0;  auto texture_type : mat_comp.texture_types) {
		os << "Type: ";
		switch(texture_type) {
		case TextureType::Bump:
			os << "Bump\t";
			break;
		case TextureType::Diffuse:
			os << "Diffuse\t";
			break;
		case TextureType::Emissive:
			os << "Emissive\t";
			break;
		case TextureType::Specular:
			os << "Specular\t";
			break;
		default:
			os << "Unknown\t";
			break;
		}
		os << "Path: " << mat_comp.texture_paths[i] << '\n';
		++i;
	}

	return os;
}

}