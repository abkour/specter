#pragma once
#include <cstdint>

namespace specter {

enum class MaterialType : uint8_t {
	Metal,
	Dielectric,
	Semiconductor	// Not supported yet
};

enum class TextureType : uint8_t {
	Diffuse,
	Emissive,
	Specular,
	Bump
};

}