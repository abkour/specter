#pragma once
#include "integrator.hpp"

namespace specter {

class NormalIntegrator : public IIntegrator {

public:

	NormalIntegrator() = default;

	vec3f render(const Accel& accel, const Ray& ray) override;
};

}