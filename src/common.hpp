/*
	Global constants and common program functions go here.
	Keep the included headers to a strict minimum.
*/

#pragma once
#include <cstdint>

// These hash values were computed via the hash function in common_math.hpp
constexpr uint64_t SPECTER_AMBIENT_LIGHT = 872128645;
constexpr uint64_t SPECTER_AREA_LIGHT = 2090087070;
constexpr uint64_t SPECTER_DIRECTIONAL_LIGHT = 2623544947;
constexpr uint64_t SPECTER_POINT_LIGHT = 271082479;
constexpr uint64_t SPECTER_INVALID = 0;
constexpr uint64_t SPECTER_DEBUG_DISPLAY_NORMALS = 578727905;
constexpr uint64_t SPECTER_DEBUG_INVALID_MODE = 0;