#pragma once
#include "accel.hpp"
#include "ray.hpp"

namespace specter {

class IIntegrator {

public:

	IIntegrator() = default;

	virtual vec3f render(const Accel& accel, const Ray& ray) = 0;
};

}